#include <cmath>    // 包含 std::atan2 和 std::abs
#include <iostream> // 用于测试和输出
#include <algorithm> // for std::max_element
#include <Eigen/Dense>
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h> 

namespace py = pybind11;


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const double EPS = 1e-6;
const double RAD_TO_DEG = 180.0 / M_PI;
const double Deg_TO_RAD =  M_PI / 180.0;


// 数值稳定的 atan2 实现
// std::atan2(y, x) 是计算机求角度的标准函数 
double safe_atan2(double y, double x) {
    if (std::abs(x) < EPS && std::abs(y) < EPS) {
        return 0.0; // 避免除零或不稳定情况
    }
    return std::atan2(y, x);  //计算角度的函数，输出弧度制
}

// 修正接近 0、1 或 -1 的值
double fix_small_value(double x) {
    if (std::abs(x) < EPS) {
        return 0.0;
    }
    if (std::abs(x - 1.0) < EPS) {
        return 1.0;
    }
    if (std::abs(x + 1.0) < EPS) {
        return -1.0;
    }
    return x;
}

Eigen::Matrix4d xyzwpr_to_T(double x, double y, double z, double roll_deg, double pitch_deg, double yaw_deg) {
    // Convert degrees to radians
    double roll = roll_deg * M_PI / 180.0;    //横滚   绕X 轴旋转
    double pitch = pitch_deg * M_PI / 180.0;    // 俯仰  绕Y 轴旋转
    double yaw = yaw_deg * M_PI / 180.0;        // 偏航   绕Z 轴旋转

    // 2. 计算三角函数值 + 数值修正（消除浮点精度误差）
    // https://www.doubao.com/thread/w254c954a30392a1e
    double cr = fix_small_value(std::cos(roll));
    double sr = fix_small_value(std::sin(roll));
    double cp = fix_small_value(std::cos(pitch));
    double sp = fix_small_value(std::sin(pitch));
    double cy = fix_small_value(std::cos(yaw));
    double sy = fix_small_value(std::sin(yaw));
    
    //固定的Z-Y-X 外旋的数学公式
    Eigen::Matrix3d R;
    R << cy*cp, cy*sp*sr - sy*cr, cy*sp*cr + sy*sr,
         sy*cp, sy*sp*sr + cy*cr, sy*sp*cr - cy*sr,
         -sp,   cp*sr,            cp*cr;

    Eigen::Matrix4d T = Eigen::Matrix4d::Identity();
    T.block<3,3>(0,0) = R;     //T.block<3,3> : 取 T 矩阵左上角 3×3 子块（存放旋转矩阵 R）
    T.block<3,1>(0,3) = Eigen::Vector3d(x, y, z);  //取 T 矩阵第 0 行第 4 列 存放平移向量 

    return T;
}

bool isWithinLimits(const std::vector<double>& sol, Eigen::MatrixXd& Joints_limits) {
    for (int i = 0; i < 6; ++i) {
        if (sol[i] < Joints_limits(i, 0) || sol[i] > Joints_limits(i, 1))
            return false;
    }
    return true;
}

// Inverse Kinematics Function
// 目标位姿是否在机械臂工作空间内、是否有可行逆解
// 逆解前置校验函数
double car_dist(const std::vector<double>& xyzwpr, const std::vector<double>& tcp_offset = {0, 0, 0, 0, 0, 0}) {
    // Define DH Parameters (these values are taken from your Python code)
    double a1 = 75, a2 = 840, a3 = 215, d4 = -890, d6 = -90;
    
    Eigen::MatrixXd Joints_limits(6, 2);
    Joints_limits << -2.9671,2.9671, -1.7453,2.7925, -1.5708,3.8397, -3.4907,3.4907, 
                    -2.4435,2.4435, -4.7124,4.7124;

    //tcp_offset 是 法兰盘 → TCP 的 6 自由度位姿偏移
    Eigen::Matrix4d T_tool = xyzwpr_to_T(xyzwpr[0], xyzwpr[1], xyzwpr[2], xyzwpr[3], xyzwpr[4], xyzwpr[5]);
    Eigen::Matrix4d T_tcp_offset = xyzwpr_to_T(tcp_offset[0], tcp_offset[1], tcp_offset[2], tcp_offset[3], tcp_offset[4], tcp_offset[5]);
    
    //T_tool  = T06 × T_tcp_offset
    //https://www.doubao.com/thread/wd7edf70be5abf030

    Eigen::Matrix4d T06 = T_tool * T_tcp_offset.inverse();  // Compute T06
    
    //获取T06 第0行，第3列开始的三行一列子块
    Eigen::Vector3d position = T06.block<3, 1>(0, 3);
    //获取T06 第0行，第3列开始的 三行三列子块
    Eigen::Matrix3d R06 = T06.block<3, 3>(0, 0);
    //从旋转矩阵中提取三个列向量

    //a即为末端z轴方向，即为工具朝向
    Eigen::Vector3d n = R06.col(0), o = R06.col(1), a = R06.col(2);
    //从平移矩阵中提取坐标->法兰中心点
    double px = position(0), py = position(1), pz = position(2);

    // Theta 1 Calculation
    //https://www.doubao.com/thread/w3e4cf0deba4240e8
    //这个角 = 底座必须旋转的角度 theta1

    //DH中的d6即为 法兰->腕心点的固定距离，此处为-90
    //腕心点坐标 = 法兰坐标 - |d6|*a
    // (a(1) * d6 + py)  ,  (a(0) * d6 + px)即为腕心点的y坐标 和 x坐标
    double theta1_tmp = safe_atan2((a(1) * d6 + py), (a(0) * d6 + px));

    //正手，反手两种姿态都可让腕心点到达同一位置，因此记录两个角度，例如30°或-150°
    std::vector<double> theta1s = {theta1_tmp};
    if (theta1_tmp < 0) theta1s.push_back(theta1_tmp + M_PI);
    else if (theta1_tmp > 0) theta1s.push_back(theta1_tmp - M_PI);
    else {
        theta1s.push_back(M_PI);
        theta1s.push_back(-M_PI);
    }

    std::vector<double> theta3s = {};
    for (double theta1 : theta1s) {
        if (theta1 < Joints_limits(0, 0) || theta1 > Joints_limits(0, 1)) continue;
        double c1 = fix_small_value(std::cos(theta1)), s1 = fix_small_value(std::sin(theta1));
        //得到c1，s1，即可求cosθ3，余弦定理
        Eigen::Vector3d pm1(px + a(0) * d6, py + a(1) * d6, -a1);
        Eigen::Vector3d xm1(c1, s1, 1);
        Eigen::Vector3d pm2(0, 0, -1);
        Eigen::Vector3d xm2(0, 0, pz + a(2) * d6);
        //上面4个向量是6 轴球形腕机械臂逆运动学的标准解析解推导出来的中间计算量
        // Calculate theta3
        // cm3 = cosθ3 * 小臂长
        double cm3 = (pm1.dot(xm1) * pm1.dot(xm1) + pm2.dot(xm2) * pm2.dot(xm2) - a2 * a2 - a3 * a3 - d4 * d4) / (2 * a2);
        //delta:第3轴theta3的实数解判别式，这是判断[可达/不可达]的唯一标准

        // |cosθ3|=<1 ,
        double delta = (a3 * a3 + d4 * d4) - cm3 * cm3;

        theta3s.push_back(delta);
    }

    // 获取最大值
    if (theta3s.empty()) {
        return -1.0; // 无有效解，表示不可达
    }
    double max_val = *std::max_element(theta3s.begin(), theta3s.end());
    return max_val;
}