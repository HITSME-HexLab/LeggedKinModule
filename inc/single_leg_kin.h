#pragma once

/* related header files */

/* c system header files */

/* c++ standard library header files */
#include <vector>
#include <string>
/* external project header files */
#include "Eigen/Dense"
// Pinocchio
#include "pinocchio/parsers/urdf.hpp"
// #include "pinocchio/algorithm/joint-configuration.hpp"
// #include "pinocchio/algorithm/geometry.hpp"
// #include <pinocchio/algorithm/rnea.hpp>
#include <pinocchio/algorithm/jacobian.hpp>
#include <pinocchio/algorithm/frames.hpp>
/* internal project header files */
#include "ikfast_leg_lib.h"

using namespace ikfast_leg;
using namespace std;
class SingleLegKin
{
private:
    pinocchio::Model model_;
    pinocchio::Data data_;
    string end_effector_name_ = "RF_FOOT";
    // Origin Calib
    // urdf origin may not be [0,0,0]
    Eigen::Vector3d origin_calib_ = Eigen::Vector3d::Zero();
    // Offsets (in order: mirror, pos, rot)
    Eigen::Vector3d mirror_offset_ = Eigen::Vector3d::Ones();
    Eigen::Vector3d pos_offset_ = Eigen::Vector3d::Zero();
    Eigen::Matrix3d rot_offset_ = Eigen::Matrix3d::Identity();
    // Joint limits (default: [-pi, pi], order: [min, max, min, max, min, max])
    Eigen::VectorXd joint_limits_ = Eigen::VectorXd::Zero(6);

    // IK Approx point
    Eigen::Vector3d ik_approx_point_ = Eigen::Vector3d::Zero();

public:
    SingleLegKin(string urdf_file_path)
    {
        pinocchio::urdf::buildModel(urdf_file_path, model_);
        data_ = pinocchio::Data(model_);
        joint_limits_ << -M_PI, M_PI, -M_PI, M_PI, -M_PI, M_PI;
    };
    ~SingleLegKin() = default;
    // Setters
    void setEndEffectorName(const string &end_effector_name) { end_effector_name_ = end_effector_name; };
    void setOriginCalib(const Eigen::Vector3d &origin_calib) { origin_calib_ = origin_calib; };
    void setMirrorOffset(const Eigen::Vector3d &mirror_offset) { mirror_offset_ = mirror_offset; };
    void setPosOffset(const Eigen::Vector3d &pos_offset) { pos_offset_ = pos_offset; };
    void setRotOffset(const Eigen::Matrix3d &rot_offset) { rot_offset_ = rot_offset; };
    void setJointLimits(const Eigen::VectorXd &joint_limits) { joint_limits_ = joint_limits; };
    void setIKApproxPoint(const Eigen::Vector3d &ik_approx_point) { ik_approx_point_ = ik_approx_point; };

    bool checkJointLimits(const Eigen::Vector3d &joints);
    // Kinematics
    bool inverseKin(const Eigen::Vector3d &pos, vector<Eigen::Vector3d> &sols);
    bool inverseKinConstraint(const Eigen::Vector3d &pos, vector<Eigen::Vector3d> &sols);
    bool forwardKin(const Eigen::Vector3d &joints, Eigen::Vector3d &pos);
    bool forwardKinConstraint(const Eigen::Vector3d &joints, Eigen::Vector3d &pos);
};
