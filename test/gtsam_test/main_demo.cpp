#include <gtsam/base/Matrix.h>
#include <gtsam/base/Vector.h>
#include <gtsam/geometry/Pose3.h>

#include <cmath>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "Arm.hpp"

namespace {

void assertNear(double actual, double expected, double tol, const std::string& msg) {
  if (std::fabs(actual - expected) > tol) {
    throw std::runtime_error(
        msg + " | actual=" + std::to_string(actual) +
        " expected=" + std::to_string(expected));
  }
}

void assertVector3Near(
    const gtsam::Vector3& actual, const gtsam::Vector3& expected, double tol, const std::string& msg) {
  for (int i = 0; i < 3; ++i) {
    assertNear(actual(i), expected(i), tol, msg + " (idx=" + std::to_string(i) + ")");
  }
}

void runArmForwardKinematicsTest() {
  const gtsam::Vector2 a(1.0, 1.0);
  const gtsam::Vector2 alpha(0.0, 0.0);
  const gtsam::Vector2 d(0.0, 0.0);
  const gtsam::Pose3 base_pose(gtsam::Rot3::Ypr(M_PI / 4.0, 0.0, 0.0), gtsam::Point3(2.0, 1.0, -1.0));
  gpmp2::Arm arm(2, a, alpha, d, base_pose);

  const gtsam::Vector2 q(0.0, 0.0);
  const gtsam::Vector2 qdot(1.0, 0.0);

  std::vector<gtsam::Pose3> poses;
  std::vector<gtsam::Vector3> velocities;
  std::vector<gtsam::Matrix> J_pose_q;
  std::vector<gtsam::Matrix> J_vel_qdot;
  std::vector<gtsam::Matrix> J_vel_q;

  arm.forwardKinematics(
      q,
      std::cref(static_cast<const gtsam::Vector&>(qdot)),
      poses,
      std::ref(velocities),
      std::ref(J_pose_q),
      std::ref(J_vel_qdot),
      std::ref(J_vel_q));

  if (poses.size() != 2 || velocities.size() != 2) {
    throw std::runtime_error("Unexpected link output size");
  }

  assertVector3Near(
      poses[0].translation(),
      gtsam::Vector3(2.707106781186548, 1.707106781186548, -1.0),
      1e-9,
      "Link0 position");
  assertVector3Near(
      poses[1].translation(),
      gtsam::Vector3(3.414213562373095, 2.414213562373095, -1.0),
      1e-9,
      "Link1 position");

  assertVector3Near(
      velocities[0],
      gtsam::Vector3(-0.707106781186548, 0.707106781186548, 0.0),
      1e-9,
      "Link0 velocity");
  assertVector3Near(
      velocities[1],
      gtsam::Vector3(-1.414213562373095, 1.414213562373095, 0.0),
      1e-9,
      "Link1 velocity");

  if (J_pose_q.size() != 2 || J_vel_qdot.size() != 2 || J_vel_q.size() != 2) {
    throw std::runtime_error("Unexpected Jacobian output size");
  }
  if (J_pose_q[0].rows() != 6 || J_pose_q[0].cols() != 2 ||
      J_vel_qdot[0].rows() != 3 || J_vel_qdot[0].cols() != 2 ||
      J_vel_q[0].rows() != 3 || J_vel_q[0].cols() != 2) {
    throw std::runtime_error("Unexpected Jacobian shape");
  }

  std::vector<gtsam::Pose3> poses_no_vel;
  arm.forwardKinematics(q, std::nullopt, poses_no_vel, std::nullopt);
  if (poses_no_vel.size() != 2) {
    throw std::runtime_error("Unexpected pose-only output size");
  }
  assertVector3Near(
      poses_no_vel[1].translation(),
      poses[1].translation(),
      1e-12,
      "Pose-only and full FK should match");
}

}  // namespace

int main() {
  try {
    runArmForwardKinematicsTest();
    std::cout << "[PASS] Arm forward kinematics test passed.\n";
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "[FAIL] " << e.what() << '\n';
    return 1;
  }
}
