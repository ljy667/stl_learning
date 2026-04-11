#include <iostream>
#include <Eigen/Dense>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
const double EPS = 1e-6;
const double RAD_TO_DEG = 180.0 / M_PI;
const double Deg_TO_RAD =  M_PI / 180.0;


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
    
    std::vector<double> xyz = {position.x(), position.y(), position.z(), roll, pitch, yaw};
    //C++ 编译器会直接在「函数调用方的内存」里构造 vector
    //return {position.x(), position.y(), position.z(), roll, pitch, yaw};

    return xyz;
}


int main() {
    Eigen::Matrix3d m;
    m << 1, 2, 3,
         4, 5, 6,
         7, 8, 9;

    Eigen::Vector3d v(1, 0, -1);
    Eigen::Vector3d r = m * v;

    std::cout << "Eigen OK. Result: " << r.transpose() << std::endl;
    return 0;
}
