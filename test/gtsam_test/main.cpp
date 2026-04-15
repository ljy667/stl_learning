#include <gtsam/geometry/Pose2.h>
#include <iostream>

int main() {
    gtsam::Pose2 p(1.0, 2.0, 0.3);
    std::cout << p.x() << ", " << p.y() << '\n';
    return 0;
}