#include <gtsam/geometry/Pose2.h>
#include <gtsam/geometry/Pose3.h>
#include <gtsam/base/Matrix.h>
#include <gtsam/base/Vector.h>
#include <iostream>

#include "ForwardKinematics.hpp"

int main() {
    gtsam::Pose2 p(1.0, 2.0, 0.3);
    std::cout << p.x() << ", " << p.y() << '\n';
    return 0;
}
