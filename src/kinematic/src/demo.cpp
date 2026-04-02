#include <iostream>
#include <Eigen/Dense>

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
