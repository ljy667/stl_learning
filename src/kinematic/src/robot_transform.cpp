#include <cmath>
#include <iostream>
#include <vector>
#include <Eigen/Dense>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
const double EPS = 1e-6;
const double RAD_TO_DEG = 180.0 / M_PI;
const double DEG_TO_RAD = M_PI / 180.0;

double safe_atan2(double y, double x) {
    if (std::abs(x) < EPS && std::abs(y) < EPS) {
        return 0.0;
    }
    return std::atan2(y, x);
}

double fix_small_value(double x) {
    if (std::abs(x) < EPS) return 0.0;
    if (std::abs(x - 1.0) < EPS) return 1.0;
    if (std::abs(x + 1.0) < EPS) return -1.0;
    return x;
}

Eigen::Matrix4d xyzwpr_to_T(double x, double y, double z, double roll_deg, double pitch_deg, double yaw_deg) {
    double roll = roll_deg * DEG_TO_RAD;
    double pitch = pitch_deg * DEG_TO_RAD;
    double yaw = yaw_deg * DEG_TO_RAD;

    double cr = fix_small_value(std::cos(roll));
    double sr = fix_small_value(std::sin(roll));
    double cp = fix_small_value(std::cos(pitch));
    double sp = fix_small_value(std::sin(pitch));
    double cy = fix_small_value(std::cos(yaw));
    double sy = fix_small_value(std::sin(yaw));

    Eigen::Matrix3d R;
    R << cy*cp, cy*sp*sr - sy*cr, cy*sp*cr + sy*sr,
         sy*cp, sy*sp*sr + cy*cr, sy*sp*cr - cy*sr,
         -sp,   cp*sr,            cp*cr;

    Eigen::Matrix4d T = Eigen::Matrix4d::Identity();
    T.block<3,3>(0,0) = R;
    T.block<3,1>(0,3) = Eigen::Vector3d(x, y, z);
    return T;
}

std::vector<double> T_to_xyzrpy(const Eigen::Matrix4d& T, bool isdegrees = true) {
    //取矩阵 T，起始行 0、起始列 3，截取 3 行 1 列子块
    Eigen::Vector3d position = T.block<3,1>(0,3);  
    Eigen::Matrix3d R = T.block<3,3>(0,0);

    double sp = -fix_small_value(R(2,0));
    double pitch = std::asin(sp);
    double cp = std::cos(pitch);

    double roll, yaw;
    if (std::abs(cp) > EPS) {
        roll = std::atan2(R(2,1), R(2,2));
        yaw = std::atan2(R(1,0), R(0,0));
    } else {
        roll = 0.0;
        yaw = std::atan2(-R(0,1), R(1,1));
    }

    if (isdegrees) {
        roll *= RAD_TO_DEG;
        pitch *= RAD_TO_DEG;
        yaw *= RAD_TO_DEG;
    }
    return {position.x(), position.y(), position.z(), roll, pitch, yaw};
}

// 抓取面类型：决定"零异常"时箱子本体轴对应基座标的哪个方向。
// TOP(正抓): 箱子本体 X/Y/Z 与基座标 X/Y/Z 完全重合。
// SIDE(侧抓): 本体X->基座+Z, 本体Y->基座+Y, 本体Z->基座-X（已与业务确认）。
enum class GraspFaceType { TOP, SIDE };
Eigen::Matrix3d referenceRotation(GraspFaceType graspType) {
    if (graspType == GraspFaceType::SIDE) {
        Eigen::Matrix3d R_ref;
        R_ref.col(0) = Eigen::Vector3d(0, 0, 1);
        R_ref.col(1) = Eigen::Vector3d(0, 1, 0);
        R_ref.col(2) = Eigen::Vector3d(-1, 0, 0);
        return R_ref;
    }
    return Eigen::Matrix3d::Identity();
}

// 从 targetInBase 里减掉抓取面类型对应的基准姿态 R_ref，剩下的 R_anomaly
// 就是"纯粹绕基座标Z轴(yaw)、再绕基座标X轴(roll)"的外旋异常量——
// 跟 xyzwpr_to_T 里固定Z-Y-X外旋的公式是同一套东西(这里pitch=0)，
// 所以直接复用 T_to_xyzrpy 反解，顺序不再有歧义。
// 返回 {yaw_deg, roll_deg, pitch_residual_deg}：pitch_residual 理论上应为0，
// 明显偏离0说明这个箱子的异常已经超出了"只绕Z/X"的业务假设，可用于报警/剔除。
std::vector<double> extractBoxYawRoll(const Eigen::Matrix4d& targetInBase, GraspFaceType graspType, bool isdegrees = true) {
    Eigen::Matrix3d R_box = targetInBase.block<3,3>(0,0);
    Eigen::Matrix3d R_ref = referenceRotation(graspType);
    Eigen::Matrix3d R_anomaly = R_box * R_ref.transpose();

    Eigen::Matrix4d T_anomaly = Eigen::Matrix4d::Identity();
    T_anomaly.block<3,3>(0,0) = R_anomaly;

    std::vector<double> rpy = T_to_xyzrpy(T_anomaly, isdegrees); // [x,y,z,roll,pitch,yaw]
    return {rpy[5], rpy[3], rpy[4]}; // yaw, roll, pitch_residual
}


// 用提取出的 yaw/roll 重新构造异常量(丢弃噪声跑到pitch上的分量)，
// 再叠加回基准姿态 R_ref，得到"只含Z/X两个自由度"的干净箱子姿态。
Eigen::Matrix3d rebuildCleanBoxRotation(const Eigen::Matrix4d& targetInBase, GraspFaceType graspType) {
    std::vector<double> yr = extractBoxYawRoll(targetInBase, graspType, true);
    double yaw_deg = yr[0];
    double roll_deg = yr[1];

    Eigen::Matrix4d T_anomaly_clean = xyzwpr_to_T(0, 0, 0, roll_deg, 0.0, yaw_deg);
    Eigen::Matrix3d R_anomaly_clean = T_anomaly_clean.block<3,3>(0,0);
    Eigen::Matrix3d R_ref = referenceRotation(graspType);

    return R_anomaly_clean * R_ref;
}

// 吸盘抓取姿态：工具Z轴要扎进箱子内部，与抓取面法线反向（已与业务确认）。
// 即把干净箱子姿态绕本体X轴翻转180°：X不变，Y、Z同时取反。
// 这个翻转是相对箱子本体轴定义的，跟抓取面是TOP还是SIDE无关，
// 因为R_clean的列本身就是"抓取面局部坐标系"在基座标下的表达。
Eigen::Matrix4d computeSuctionGraspPose(const Eigen::Matrix4d& targetInBase, GraspFaceType graspType) {
    Eigen::Matrix3d R_clean = rebuildCleanBoxRotation(targetInBase, graspType);

    Eigen::Matrix3d R_tool;
    R_tool.col(0) = R_clean.col(0);
    R_tool.col(1) = -R_clean.col(1);
    R_tool.col(2) = -R_clean.col(2);

    Eigen::Matrix4d T_tool = Eigen::Matrix4d::Identity();
    T_tool.block<3,3>(0,0) = R_tool;
    T_tool.block<3,1>(0,3) = targetInBase.block<3,1>(0,3);
    return T_tool;
}

// 输出的 xyzwpr 是吸盘TCP在基座标下的目标位姿，跟 car_control.cpp 里
// xyzwpr_to_T(xyzwpr...) 消费的语义完全一致，可直接送入逆解（法兰->TCP的
// 额外偏移仍走现有的 tcp_offset 机制，这里不处理）。
std::vector<double> computeSuctionGraspWPR(const Eigen::Matrix4d& targetInBase, GraspFaceType graspType, bool isdegrees = true) {
    Eigen::Matrix4d T_tool = computeSuctionGraspPose(targetInBase, graspType);
    return T_to_xyzrpy(T_tool, isdegrees);
}

int main() {
    // TOP: 构造一个带噪声的正抓异常姿态：yaw=25°, roll=15°, 外加5°的pitch噪声
    Eigen::Matrix4d topTarget = xyzwpr_to_T(500, 200, 300, 15, 5, 25);
    std::vector<double> topYR = extractBoxYawRoll(topTarget, GraspFaceType::TOP);
    std::cout << "[TOP] yaw=" << topYR[0] << " roll=" << topYR[1] << " pitch_residual=" << topYR[2] << std::endl;
    std::vector<double> topWPR = computeSuctionGraspWPR(topTarget, GraspFaceType::TOP);
    std::cout << "[TOP] suction xyzwpr = ";
    for (double v : topWPR) std::cout << v << " ";
    std::cout << std::endl;

    // SIDE: 先构造侧抓的理想基准姿态(本体X->基座Z, Y->基座Y, Z->基座-X)，
    // 再叠加yaw=25°/roll=15°的外旋异常，验证特化后的提取是否能还原出同样的角度
    Eigen::Matrix4d anomaly = xyzwpr_to_T(0, 0, 0, 15, 0, 25);
    Eigen::Matrix4d sideTarget = Eigen::Matrix4d::Identity();
    sideTarget.block<3,3>(0,0) = anomaly.block<3,3>(0,0) * referenceRotation(GraspFaceType::SIDE);
    sideTarget.block<3,1>(0,3) = Eigen::Vector3d(500, 200, 300);

    std::vector<double> sideYR = extractBoxYawRoll(sideTarget, GraspFaceType::SIDE);
    std::cout << "[SIDE] yaw=" << sideYR[0] << " roll=" << sideYR[1] << " pitch_residual=" << sideYR[2] << std::endl;
    std::vector<double> sideWPR = computeSuctionGraspWPR(sideTarget, GraspFaceType::SIDE);
    std::cout << "[SIDE] suction xyzwpr = ";
    for (double v : sideWPR) std::cout << v << " ";
    std::cout << std::endl;

    return 0;
}
