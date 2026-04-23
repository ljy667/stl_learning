

#pragma once

#include <gtsam/geometry/Pose3.h>
#include <gtsam/base/Matrix.h>
#include <gtsam/base/Vector.h>

#include <vector>

#include <functional>
#include <optional>

namespace gpmp2{
    
template<class POSE , class VELOCITY>
class ForwardKinematics {
    private:
        std::size_t dof_ ;      //控制链自由度
        std::size_t nr_links_;   //连杆数


    public:
        typedef POSE Pose;
        typedef VELOCITY Velocity;

    ForwardKinematics() : dof_(0) , nr_links_(0) {} ;
    ForwardKinematics(std::size_t dof , std::size_t nr_links) : dof_(dof) , nr_links_(nr_links){}

    virtual ~ForwardKinematics(){} ; 


    //可选：jv - 关节速度（cosnt Velocity& 类型，std::optional 包装）
    // virtual

/**
   *  Forward kinematics: poses from configuration space to 3D workspace
   *  Velocity kinematics: optional velocities from configuration space to 3D workspace, no angular rate
   *  pure virtual method, need implementation in derived class
   *
   *  @param jp robot pose in config space
   *  @param jv robot velocity in config space
   *  @param jpx link poses in 3D work space
   *  @param jvx link velocities in 3D work space, no angular rate
   *  @param J_jpx_jp et al. optional Jacobians
   **/
    virtual void forwardKinematics(
            const Pose& jp , 
            std::optional<std::reference_wrapper<const Velocity>> jv , 
            std::vector<gtsam::Pose3>& jpx , 
            std::optional<std::reference_wrapper<std::vector<gtsam::Vector3>>> jvx , 
            std::optional<std::reference_wrapper<std::vector<gtsam::Matrix>>> J_jpx_jp = std::nullopt,
            std::optional<std::reference_wrapper<std::vector<gtsam::Matrix>>> J_jpv_jp = std::nullopt,
            std::optional<std::reference_wrapper<std::vector<gtsam::Matrix>>> J_jvx_jp = std::nullopt
    ) const  = 0 ;
    
    gtsam::Matrix forwardKinematicsPose(const Pose& jp) const;
    gtsam::Matrix forwardKinematicsPosition(const Pose& jp) const ;
    gtsam::Matrix forwardKinematicsVel(const Pose& jp ,const Velocity& jv) const ;

    ///accesses
    std::size_t dof() const {return this -> dof_;}
    std::size_t nr_links() const {return this -> nr_links_ ;}

};

}

#include "ForwardKinematics-inl.hpp"


