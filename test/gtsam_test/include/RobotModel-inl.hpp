#include "RobotModel.hpp"



  namespace gpmp2{

      /* const Pose& jp：机器人关节位姿（输入，引用传递无拷贝）
      sph_centers：输出参数，存储所有碰撞球的世界中心
      J_point_conf：可选输出参数，需要雅可比才计算，不需要则为空 */

    template <class FK>
    void RobotModel<FK>::sphereCenters(const Pose& jp,
                std::vector<gtsam::Point3>& sph_centers,
        std::optional<std::reference_wrapper<std::vector<gtsam::Matrix>>> J_point_conf = std::nullopt) const 
        
        {
          std::vector<gtsam::Pose3> link_poses;
          std::vector<gtsam::Matrix> J_pose_jp;


          //fk_model_ 构造函数中传入的参数
          if(J_point_conf)
              fk_model_.forwardKinematics(jp, std::nullopt ,link_poses, std::nullopt , J_pose_jp);
          else
              fk_model_.forwardKinematics(jp , std::nullopt , link_poses,std::nullopt )
            
          sph_centers.resize(nr_body_spheres());
          if(J_point_conf) J_point_conf->resize(nr_body_spheres());

          for(std::size_t sph_idx = 0 ; sph_idx < nr_body_spheres() ; sph_idx++)
          {
              if(J_point_conf){
                  gtsam::Matrix36 J_point_pose; //存储变换对位姿的雅克比矩阵（3×6 维度）

                  //基于球体结构中获取的link_id索引，获取对应link_poses, 为Pose3类型
                  sph_centers[sph_idx] = link_poses[body_spheres_[sph_idx].link_id].transform_from(
                      body_spheres_[sph_idx].center , J_point_pose)

                  (*J_point_conf).get()[sph_idx] =  J_point_pose  *  J_pose_jp[body_spheres_[sph_idx].link_id];

              }  else {
                sph_centers[sph_idx] = link_poses[body_spheres_[sqh_id].link_id].transform_from(
                  body_spheres_[sph_idx].center
                )
              }
          }

        }
            
  }
    