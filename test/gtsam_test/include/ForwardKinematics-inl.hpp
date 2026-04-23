

#include "ForwardKinematics.hpp"

namespace gpmp2 {

    template<class POSE  , class VELOCITY>
    gtsam::Matrix ForwardKinematics<POSE,VELOCITY>::forwardKinematicsPose(const Pose& jp) const {
        /* 
        virtual void forwardKinematics(const Pose& jp , std::optional<const velocity&> jv , 
        std::vector<gtsam::Pose3>& jpx , std::optional<std::vector<gtsam::Vector3>&> jvx , 
        std::optional<std::vector<gtsam::Matrix>&> J_jpx_jp = std::nullopt,
        std::optional<std::vector<gtsam::Matrix>&> J_jpv_jp = std::nullopt,
        std::optional<std::vector<gtsam::Matrix>&> J_jvx_jp = std::nullopt
        ) const  = 0 ; 
        
        #@param jpx link poses in 3D work space
        */
        

        std::vector<gtsam::Pose3> jpx;  // 用于存放运动学输出
        forwardKinematics(jp , std::nullopt , jpx , std::nullopt);

        //矩阵，每列为每个连杆的运动学信息，

        //===============================================
        // jpx_mat => 每列前三个欧拉角，后三个坐标
        //===============================================

        gtsam::Matrix jpx_mat(6 , this->nr_links_); 
        for(std::size_t i = 0 ; i < this->nr_links_ ; i++) 
            // 每列元素配置
            // gtsam::vector6()具有<<操作符
            // jpx[i] 
            jpx_mat.col(i) = (gtsam::Vector6() << gtsam::Vector3(
                    jpx[i].rotation().yaw(),
                    jpx[i].rotation().pitch(),
                    jpx[i].rotation().roll()),
                    jpx[i].translation()
                ).finished();
            
                /* Eigen 中所有 **<< 流式初始化 **，最后都必须： .finished()
                    << 开始往里面流式填充数据
                填充完后，必须用 .finished() 收尾，才能得到一个合法向量 */

        return jpx_mat ; 
    }

    template <class POSE , class VELOCITY>
    gtsam::Matrix ForwardKinematics<POSE,VELOCITY>::forwardKinematicsPosition(const Pose& jp) const {

        std::vector<gtsam::Pose3> jpx;
        forwardKinematics(jp , std::nullopt , jpx ,std::nullopt);

        // convert vector in matrix
        gtsam::Matrix jpx_mat(3, this->nr_links_);
        
        for(std::size_t i = 0; i < this->nr_links_ ; i++)
            jpx_mat.col(i) = jpx[i].translation();
        return jpx_mat ; 
    }

    template <class POSE, class VELOCITY>
        gtsam::Matrix ForwardKinematics<POSE, VELOCITY>::forwardKinematicsVel(const Pose& jp,
            const Velocity& jv) const {

        std::vector<gtsam::Pose3> jpx;
        std::vector<gtsam::Vector3> jvx;
        forwardKinematics(jp, std::cref(jv), jpx, std::ref(jvx));

        // convert vector in matrix
        gtsam::Matrix jpv_mat(3, this->nr_links_);
        for (std::size_t i = 0; i < this->nr_links_; i++)
            jpv_mat.col(i) = jvx[i];
        return jpv_mat;
        }

}
