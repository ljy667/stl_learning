#include <cmath>    // 包含 std::atan2 和 std::abs
#include <iostream> // 用于测试和输出
#include <Eigen/Dense>
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h> 

namespace py = pybind11;

// 如果没有定义 M_PI，则定义它
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
const double EPS = 1e-6;
const double RAD_TO_DEG = 180.0 / M_PI;
const double Deg_TO_RAD =  M_PI / 180.0;

// 数值稳定的 atan2 实现
double safe_atan2(double y, double x) {
    if (std::abs(x) < EPS && std::abs(y) < EPS) {
        return 0.0; // 避免除零或不稳定情况
    }
    return std::atan2(y, x);
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

// 自定义排序比较函数
bool compareSolutions(const std::vector<double>& sol1, const std::vector<double>& sol2) {
    // 比较两个解的每个元素，直到有不相等的元素为止
    for (int i = 0; i < sol1.size(); ++i) {
        if (sol1[i] != sol2[i]) {
            return sol1[i] < sol2[i];  // 如果第i个元素不相等，按该元素排序
        }
    }
    return false;  // 如果所有元素都相等，则认为它们是相等的
}
// Convert radians to degrees

// 将每个解的元素从弧度转换为角度
void convertRadiansToDegrees(std::vector<std::vector<double>>& solutions) {
    for (auto& sol : solutions) {
        for (auto& angle : sol) {
            angle *= RAD_TO_DEG;  // 弧度转换为角度
        }
    }
}

void convert2Faunc(std::vector<std::vector<double>>& solutions) {
    for (auto& sol : solutions) {
        sol[2] -= sol[1];
    }
}


Eigen::Matrix4d xyzwpr_to_T(double x, double y, double z, double roll_deg, double pitch_deg, double yaw_deg) {
    // Convert degrees to radians
    double roll = roll_deg * M_PI / 180.0;
    double pitch = pitch_deg * M_PI / 180.0;
    double yaw = yaw_deg * M_PI / 180.0;

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
        // Gimbal lock
        roll = 0.0;
        yaw = std::atan2(-R(0,1), R(1,1));
    }

    if (isdegrees) {
        roll *= 180.0 / M_PI;
        pitch *= 180.0 / M_PI;
        yaw *= 180.0 / M_PI;
    }

    return {position.x(), position.y(), position.z(), roll, pitch, yaw};
}

// 数值稳定的 DH 变换实现
Eigen::Matrix4d dh_transform(double d, double a, double alpha, double theta) {
    double ct = fix_small_value(std::cos(theta));
    double st = fix_small_value(std::sin(theta));
    double ca = fix_small_value(std::cos(alpha));
    double sa = fix_small_value(std::sin(alpha));

    Eigen::Matrix4d T;
    T << ct, -st*ca,  st*sa, a*ct,
         st,  ct*ca, -ct*sa, a*st,
         0,     sa,     ca,    d,
         0,      0,      0,    1;
    return T;
}

// 前向运动学：输入关节角（弧度），返回末端位姿（xyzrpy 或 4x4矩阵）
py::object forward_kinematics(const std::vector<double>& joints, bool wpr=true, const std::vector<double>& tcp_offset = {0,0,0,0,0,0}) {
    if (joints.size() != 6 || tcp_offset.size() != 6)
        throw std::runtime_error("Expecting 6 joint angles and 6 TCP offsets");
    bool fanuc = true;

    std::vector<double> modified_joints = joints;
    for (int i = 0; i < 6; ++i) {
        modified_joints[i] *= Deg_TO_RAD;  // 角度转换为弧度
    }

    if (fanuc) modified_joints[2] += modified_joints[1];

    modified_joints[1] -= M_PI / 2;  // J2 补偿

    /* 工业机器人统一建模标准，每个连杆对应 4 个参数：
    d：连杆偏移
    a：连杆长度
    alpha：连杆扭转角
    theta：关节旋转角（就是输入的关节值）
    
    基于关节角的不同 => 可推出的末端基于基座的不同的旋转矩阵
    */

    // DH 参数
    std::vector<double> d     = {0, 0, 0, -890, 0, -90};
    std::vector<double> a     = {75, 840, 215, 0, 0, 0};
    std::vector<double> alpha = {-M_PI/2, M_PI, -M_PI/2, M_PI/2, -M_PI/2, M_PI};

    // 初始单位矩阵
    Eigen::Matrix4d T = Eigen::Matrix4d::Identity();

    // 累乘每一节的变换
    for (int i = 0; i < 6; ++i) {
        T = T * dh_transform(d[i], a[i], alpha[i], modified_joints[i]);
    }
    // TCP offset 处理
    T = T * xyzwpr_to_T(tcp_offset[0], tcp_offset[1], tcp_offset[2],
                        tcp_offset[3], tcp_offset[4], tcp_offset[5]);
    
    if (wpr) {
        return py::cast(T_to_xyzrpy(T, true));
    } else {
        return py::cast(T);
    }
}

//关节2、3无耦合版本
Eigen::Matrix4d forward_kinematics2(const std::vector<double>& joints, bool wpr=true, const std::vector<double>& tcp_offset = {0,0,0,0,0,0}) {
    if (joints.size() != 6 || tcp_offset.size() != 6)
        throw std::runtime_error("Expecting 6 joint angles and 6 TCP offsets");

    std::vector<double> modified_joints = joints;
    modified_joints[1] -= M_PI / 2;  // J2 补偿

    // DH 参数
    std::vector<double> d     = {0, 0, 0, -890, 0, -90};
    std::vector<double> a     = {75, 840, 215, 0, 0, 0};
    std::vector<double> alpha = {-M_PI/2, M_PI, -M_PI/2, M_PI/2, -M_PI/2, M_PI};

    // 初始单位矩阵
    Eigen::Matrix4d T = Eigen::Matrix4d::Identity();

    // 累乘每一节的变换
    
    for (int i = 0; i < 6; ++i) {
        T = T * dh_transform(d[i], a[i], alpha[i], modified_joints[i]);
    }

    // TCP offset 处理
    T = T * xyzwpr_to_T(tcp_offset[0], tcp_offset[1], tcp_offset[2],
                        tcp_offset[3], tcp_offset[4], tcp_offset[5]);

    return T;
}

bool isWithinLimits(const std::vector<double>& sol, Eigen::MatrixXd& Joints_limits) {
    for (int i = 0; i < 6; ++i) {
        if (sol[i] < Joints_limits(i, 0) || sol[i] > Joints_limits(i, 1))
            return false;
    }
    return true;
}

// 判断两个解之间的差异是否小于给定的阈值
bool isSimilar(const std::vector<double>& sol1, const std::vector<double>& sol2, double threshold) {
    double diff = 0.0;
    for (int i = 0; i < sol1.size(); ++i) {
        diff += std::abs(sol1[i] - sol2[i]);
    }
    return std::sqrt(diff) < threshold;
}

// 去除重复解
std::vector<std::vector<double>> removeSimilarSolutions(std::vector<std::vector<double>>& solutions, double threshold) {
    if (solutions.empty()) return {};

    // 对解进行排序，通常按第一个关节的值进行排序
    sort(solutions.begin(), solutions.end(), [](const std::vector<double>& a, const std::vector<double>& b) {
        return a[0] < b[0];  // 根据第一个元素进行排序
    });

    std::vector<std::vector<double>> unique_solutions;
    unique_solutions.push_back(solutions[0]);  // 将第一个解添加到结果中

    // 遍历剩余解，去掉相似解
    for (int i = 1; i < solutions.size(); ++i) {
        bool is_duplicate = false;
        for (const auto& sol : unique_solutions) {
            if (isSimilar(solutions[i], sol, threshold)) {
                is_duplicate = true;
                break;
            }
        }
        if (!is_duplicate) {
            unique_solutions.push_back(solutions[i]);
        }
    }

    return unique_solutions;
}

// Inverse Kinematics Function
std::vector<std::vector<double>> inverse_kinematics(const std::vector<double>& xyzwpr, const std::vector<double>& tcp_offset = {0, 0, 0, 0, 0, 0}) {
    // Define DH Parameters (these values are taken from your Python code)
    double a1 = 75, a2 = 840, a3 = 215, d4 = -890, d6 = -90;
    bool fanuc = true;
    bool FK_valid = true;
    bool multi_solution = true;
    bool flip_wrist = true;
    double threshold = 1;
    bool sorted = true;

    bool flip_rotate = true;
    Eigen::MatrixXd Joints_limits(6, 2);
    Joints_limits << -2.9671,2.9671, -1.7453,2.7925, -1.5708,3.8397, -3.4907,3.4907, 
                    -2.4435,2.4435, -4.7124,4.7124;

    std::vector<std::vector<double>> solutions;

    Eigen::Matrix4d T_tool = xyzwpr_to_T(xyzwpr[0], xyzwpr[1], xyzwpr[2], xyzwpr[3], xyzwpr[4], xyzwpr[5]);
    Eigen::Matrix4d T_tcp_offset = xyzwpr_to_T(tcp_offset[0], tcp_offset[1], tcp_offset[2], tcp_offset[3], tcp_offset[4], tcp_offset[5]);

    Eigen::Matrix4d T06 = T_tool * T_tcp_offset.inverse();  // Compute T06
    Eigen::Vector3d position = T06.block<3, 1>(0, 3);
    Eigen::Matrix3d R06 = T06.block<3, 3>(0, 0);
    Eigen::Vector3d n = R06.col(0), o = R06.col(1), a = R06.col(2);

    double px = position(0), py = position(1), pz = position(2);

    // Theta 1 Calculation
    double theta1_tmp = safe_atan2((a(1) * d6 + py), (a(0) * d6 + px));
    std::vector<double> theta1s = {theta1_tmp};
    if (theta1_tmp < 0) theta1s.push_back(theta1_tmp + M_PI);
    else if (theta1_tmp > 0) theta1s.push_back(theta1_tmp - M_PI);
    else {
        theta1s.push_back(M_PI);
        theta1s.push_back(-M_PI);
    }

    for (double theta1 : theta1s) {
        if (theta1 < Joints_limits(0, 0) || theta1 > Joints_limits(0, 1)) continue;
        double c1 = fix_small_value(std::cos(theta1)), s1 = fix_small_value(std::sin(theta1));

        Eigen::Vector3d pm1(px + a(0) * d6, py + a(1) * d6, -a1);
        Eigen::Vector3d xm1(c1, s1, 1);
        Eigen::Vector3d pm2(0, 0, -1);
        Eigen::Vector3d xm2(0, 0, pz + a(2) * d6);

        // Calculate theta3
        double cm3 = (pm1.dot(xm1) * pm1.dot(xm1) + pm2.dot(xm2) * pm2.dot(xm2) - a2 * a2 - a3 * a3 - d4 * d4) / (2 * a2);
        if ((a3 * a3 + d4 * d4) < cm3 * cm3) {
            //std::cout << "No solution" << std::endl;
            continue;
        }
        
        double sdelta = std::sqrt(a3 * a3 + d4 * d4 - cm3 * cm3);
        double angle1 = safe_atan2(sdelta, cm3);
        double angle2 = safe_atan2(d4, a3);

        std::vector<double> theta3s = {-angle1 - angle2, angle1 - angle2};
    
        // Filter theta3 solutions within the joint limits
        for (double theta3 : theta3s) {
            if (theta3 < Joints_limits(2, 0) || theta3 > Joints_limits(2, 1)) continue;

            // Calculate theta2 based on the previous approach
            double c3 = fix_small_value(std::cos(theta3)), s3 = fix_small_value(std::sin(theta3));
            double h_m1 = d4 * c3 + a3 * s3;
            double h_m2 = a3 * c3 - d4 * s3 + a2;
            double theta2_tmp = safe_atan2((h_m1 * pm2.dot(xm2) - h_m2 * pm1.dot(xm1)), (h_m2 * pm2.dot(xm2) + h_m1 * pm1.dot(xm1)));
            
            std::vector<double> theta2s = {theta2_tmp};
            if (theta2_tmp < 0) theta2s.push_back(theta2_tmp + M_PI);
            else if (theta2_tmp > 0) theta2s.push_back(theta2_tmp - M_PI);
            else {
                theta2s.push_back(M_PI);
                theta2s.push_back(-M_PI);
            }

            for (double theta2 : theta2s) {
                if (theta2 < Joints_limits(1, 0) || theta2 > Joints_limits(1, 1)) continue;
                double theta23 = theta2 - theta3;
                double c23 = fix_small_value(std::cos(theta23)), s23 = fix_small_value(std::sin(theta23));
                
                Eigen::Vector3d zl(c1*c23, s1*c23, -s23);
                double theta6_tmp = safe_atan2(zl.dot(o), zl.dot(n));
                std::vector<double> theta6s = {theta6_tmp};
                if (theta6_tmp < 0) theta6s.push_back(theta6_tmp + M_PI);
                else if (theta6_tmp > 0) theta6s.push_back(theta6_tmp - M_PI);
                else {
                    theta6s.push_back(M_PI);
                    theta6s.push_back(-M_PI);
                }

                for (double theta6 : theta6s) {
                    if (theta6 < Joints_limits(5, 0) || theta6 > Joints_limits(5, 1)) continue;
                    
                    double c6 = fix_small_value(std::cos(theta6)), s6 = fix_small_value(std::sin(theta6));

                    Eigen::Vector3d hn1(s1,-c1,0);
                    Eigen::Vector3d hn2(c1*s23, s1*s23, c23);
                    Eigen::Vector3d hn3 = -o*c6 + n*s6;

                    double q4 = - hn2.dot(hn3);
                    double p4 = hn1.dot(hn3);
                    double angle3 = 0;
                    if (std::abs(q4*q4+p4*p4 - 1) < EPS) {
                        angle3 = 0;
                    }
                    else if (q4*q4+p4*p4 - 1 < EPS) {
                        //std::cout << "No solution" << std::endl;
                        continue;
                    }
                    else {
                        double sdelta2 = std::sqrt(q4*q4+p4*p4 - 1);
                        angle3 = safe_atan2(sdelta2,1);
                    }

                    double angle4 = safe_atan2(q4,p4);
                    std::vector<double> theta4s = {angle3 - angle4,-angle3 - angle4};

                    for (double theta4 : theta4s) {
                        if (theta4 < Joints_limits(3, 0) || theta4 > Joints_limits(3, 1)) continue;
                        
                        double q5 = zl.dot(o)*s6 + zl.dot(n)*c6;
                        double p5 = zl.dot(a);
                        double angle5 = 0;
                        if (std::abs(q5*q5+p5*p5 - 1) < EPS) {
                            angle5 = 0;
                        }
                        else if (q5*q5+p5*p5 - 1 < EPS) {
                            //std::cout << "No solution" << std::endl;
                            continue;
                        }
                        else {
                            double sdelta3 = std::sqrt(q5*q5+p5*p5 - 1);
                            angle5 = safe_atan2(sdelta3,1);
                        }

                        double angle6 = safe_atan2(q5,p5);
                        std::vector<double> theta5s = {angle5 + angle6,-angle5 + angle6};

                        for (double theta5 : theta5s) {
                            if (theta5 < Joints_limits(4, 0) || theta5 > Joints_limits(4, 1)) continue;
                            theta6 = std::fmod(theta6 + 2*M_PI, 2 * M_PI) - M_PI;
                            // if (fanuc) {
                            //     theta3 -= theta2;
                            // }
                            solutions.push_back({theta1, theta2, theta3, 
                                theta4, theta5, theta6});
                        }
                    }
                }
            }
        }
    }

    if (solutions.size() > 0) {
        solutions = removeSimilarSolutions(solutions,threshold);
        std::vector<std::vector<double>> new_solutions;
        if (FK_valid) {
            for (const auto& sol : solutions) {
                Eigen::Matrix4d Tk_res = forward_kinematics2(sol, false, tcp_offset);
                double pos_err = (T_tool.block<3,1>(0,3) - Tk_res.block<3,1>(0,3)).norm();
                double wpr_err = (T_tool.block<3,3>(0,0) - Tk_res.block<3,3>(0,0)).norm();
                if (pos_err < 10 && wpr_err < 1)
                    new_solutions.push_back(sol);
            }
        } else {
            new_solutions = solutions;
        }

        if (multi_solution) {
            // 翻腕解
            if (flip_wrist) {
                std::vector<std::vector<double>> temp = new_solutions;
                for (const auto& sol : temp) {
                    std::vector<double> sol_new = sol;
                    sol_new[5] += M_PI;
                    sol_new[4] = -sol_new[4];
                    sol_new[3] -= M_PI;
                    if (isWithinLimits(sol_new, Joints_limits))
                        new_solutions.push_back(sol_new);

                    sol_new = sol;
                    sol_new[5] -= M_PI;
                    sol_new[4] = -sol_new[4];
                    sol_new[3] += M_PI;
                    if (isWithinLimits(sol_new, Joints_limits))
                        new_solutions.push_back(sol_new);
                }
            }
            new_solutions = removeSimilarSolutions(new_solutions,threshold);

            // 翻转解
            if (flip_rotate) {
                std::vector<std::vector<double>>  temp = new_solutions;
                for (const auto& sol : temp) {
                    if (sol[5] > M_PI/2 && sol[5] < M_PI) {
                        std::vector<double> sol_new = sol;
                        sol_new[5] -= 2 * M_PI;
                        new_solutions.push_back(sol_new);
                    }
                    if (sol[5] < -M_PI/2 && sol[5] > -M_PI) {
                        std::vector<double> sol_new = sol;
                        sol_new[5] += 2 * M_PI;
                        new_solutions.push_back(sol_new);
                    }
                }
                new_solutions = removeSimilarSolutions(new_solutions,threshold);
            }
        }
  
        // Return solutions
        if (solutions.size() > 0) {
            //初步选择解
            // 过滤条件：第6个元素在 -pi 到 pi 之间
            std::vector<std::vector<double>> filtered_solutions;
            for (const auto& solution : new_solutions) {
                double first_element = solution[0]; //-90~90
                double second_element = solution[5]; //-180~180
                double thrid_element = solution[3]; //-180~180
                double fouth_element = solution[1]; //-90~90
                double fifth_element = solution[2]; //下极限~90
                if (first_element >= -M_PI/2 && first_element <= M_PI/2 && second_element >= -M_PI && second_element <= M_PI 
                    && thrid_element >= -M_PI && thrid_element <= M_PI && fouth_element >= -M_PI/2 && fouth_element <= M_PI/2
                    && fifth_element <= M_PI/2) {
                    filtered_solutions.push_back(solution);
                }
            }
            new_solutions = filtered_solutions;

            if (sorted) sort(new_solutions.begin(), new_solutions.end(), compareSolutions);
            convertRadiansToDegrees(new_solutions); // 转为角度

            if (fanuc) convert2Faunc(new_solutions);
    
            return new_solutions;
        }
        else {
            std::cout << "No solution Found!" << std::endl;
            return {};
        }
        
    }
    else {
        std::cout << "No solution Found!" << std::endl;
        return {};
    }

    
    
}



PYBIND11_MODULE(M20id35_FK_IK, m) {
    m.def("xyzwpr_to_T", &xyzwpr_to_T, "Convert xyz + rpy to 4x4 transform matrix");
    m.def("T_to_xyzrpy", &T_to_xyzrpy, "Convert 4x4 transform matrix to xyz + rpy");
    m.def("dh_transform", &dh_transform, "Compute DH transformation matrix");
    m.def("safe_atan2", &safe_atan2, "atan2 fafe");
    m.def("fix_small_value", &fix_small_value, "limit value process");
    m.def("forward_kinematics", &forward_kinematics,
          py::arg("joints"), py::arg("wpr") = true, py::arg("tcp_offset") = std::vector<double>{0,0,0,0,0,0},
          "Compute forward kinematics using DH parameters");
    m.def("inverse_kinematics", &inverse_kinematics, "Inverse Kinematics Solver",
            py::arg("xyzwpr"), py::arg("tcp_offset") = std::vector<double>{0, 0, 0, 0, 0, 0});
}
