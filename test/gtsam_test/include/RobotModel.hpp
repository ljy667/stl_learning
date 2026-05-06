
#pragma once

#include <gtsam/geometry/Pose3.h>
#include <gtsam/base/Matrix.h>
#include <gtsam/base/Vector.h>
#include <vector>

namespace gpmp2 {

    struct BodySphere{
        std::size_t link_id;
        double radius;
        gtsam::Point3 center ; 

        // construct
        BodySphere(std::size_t id , double r , const gtsam::Point3& c)
            : link_id(id) ,radius(r) ,center(c) {}
        ~BodySphere(){}
    };

    typedef std::vector<BodySphere> BodySphereVector;


    template <class FK>
    class RobotModel {
    
        public:
            typedef FK FKModel;
            typedef typename FKModel::Pose Pose;
            typedef typename FKModel::Velocity Velocity;

        private:
            FKModel fk_model_;   //fk
            BodySphereVector body_spheres_(){} ;

        public:
            //default constructor
            RobotModel():fk_model_(),body_spheres_(){}

            RobotModel(const FKModel& fk_model , const BodySphereVector& body_spheres)
                : fk_model_(fk_model), body_spheres_(body_spheres_){}

            virtual ~RobotModel(){}

            void sphereCenters(const Pose& jp, std::vector<gtsam::Point3>& sph_centers,
                std::optional<std::reference_wrapper<std::vector<gtsam::Matrix>>> J_point_conf = std::nullopt) const ;
            
            /// 根据构型空间中的给定位姿，求解工作空间内单个球体中心坐标
            /// 用于快速调用单个球体计算
            /// 可附带输出位姿对应的雅可比矩阵（可选）
            gtsam::Point3 sphereCenters(std::size_t sph_idx , const Pose& jp,
            std::optional<std::reference_wrapper<gtsam::Matrix>> J_point_conf = std::nullopt) const ;

            
            const FKModel& fk_model() const {return this->fk_model_;}
            std::size_t dof() const {return fk_model_.dof();}

            std::size_t nr_body_spheres() const {return body_spheres_.size();}
            std::size_t sphere_link_id(std::size_t i) const {return body_spheres_[i].link_id ;}
            double sphere_radius(std::size_t i) const {return body_spheres_[i].radius;}
            const gtsam::Point3& sphere_center_wrt_link(std::size_t i) const { return body_spheres_[i].center; }


    };      

}

#include  "RobotModel-inl.hpp"