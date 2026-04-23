

#include "Arm.hpp"


namespace gpmp2 {
    // typedef ForwardKinematics<gtsam::Vector , gtsam::Vector> Base ;
    //  ForwardKinematics(std::size_t dof , std::size_t nr_links) : dof_(dof) , nr_links_(nr_links){}

    Arm::Arm(std::size_t dof , const gtsam::Vector& a , const gtsam::Vector& alpha,const gtsam::Vector& d ,
    const gtsam::Pose3& base_pose  , const gtsam::Vector& theta_bias) :
    Base(dof,dof) , a_(a) , alpha_(alpha) , d_(d) ,base_pose_(base_pose),
    theta_bias_(theta_bias){

    //固定参数的坐标变换
    for(std::size_t i = 0 ; i < dof ; i++){
        link_trans_notheta_.push_back(
            gtsam::Pose3(gtsam::Rot3() , gtsam::Point3(0,0,d_(i))) *
            gtsam::Pose3(gtsam::Rot3() , gtsam::Point3(a_(i) , 0 ,0))*
            gtsam::Pose3(gtsam::Rot3::Rx(alpha_(i)) , gtsam::Point3(0,0,0)));
        }
    }

    void Arm::forwardKinematics(
        const gtsam::Vector& jp, 
        std::optional<std::reference_wrapper<const gtsam::Vector>> jv,
        std::vector<gtsam::Pose3>& jpx, 
        std::optional<std::reference_wrapper<std::vector<gtsam::Vector3>>> jvx,
        std::optional<std::reference_wrapper<std::vector<gtsam::Matrix>>> J_jpx_jp,
        std::optional<std::reference_wrapper<std::vector<gtsam::Matrix>>> J_jvx_jp,
        std::optional<std::reference_wrapper<std::vector<gtsam::Matrix>>> J_jvx_jv
        ) const {
            jpx.resize(this->dof());
            if(jvx) (*jvx).get().resize(dof());

            //每个连杆对应一个雅可比矩阵
            if(J_jpx_jp) (*J_jpx_jp).get().assign(dof() , gtsam::Matrix::Zero(6, dof()));
            if(J_jvx_jp) (*J_jvx_jp).get().assign(dof() , gtsam::Matrix::Zero(6, dof()));
            if(J_jvx_jv) (*J_jvx_jv).get().assign(dof() , gtsam::Matrix::Zero(6, dof()));


            /*  存储每个关节的局部DH齐次变换矩阵
                来源：由你之前的 getH(i, theta) 函数计算得到
                用途：存储 关节i 局部坐标系下的变换（仅关节旋转 + 连杆固定偏移） 
            */
            std::vector<gtsam::Matrix4> H(dof());
            
            /*  Ho[0]：机械臂底座在世界坐标系的位姿
                Ho[1] ~ Ho[dof()]：基坐标系 → 第 1~ 最后 1 个关节 / 连杆的全局位姿
                来源：Ho[i] = Ho[i-1] * H[i-1]（前向递推，FK 核心公式）
                用途：最终计算末端执行器位姿、构建雅可比矩阵的核心数据 */
            std::vector<gtsam::Matrix4> Ho(dof()+1);

            
            /*  基坐标系 → 连杆 / 关节 全局齐次变换矩阵
                jv：布尔标志，true 表示需要计算雅可比
                分配 dof() 个 3×dof() 矩阵：3 行 (x/y/z 线速度)，dof()列 (对应每个关节)
                来源：由你之前的 getJvj(...) 函数逐列填充
                用途：建立「关节速度 → 末端线速度」的映射，是运动控制 / 轨迹优化的核心 */

            std::vector<gtsam::Matrix> J ;
            if(jv) J.assign(dof() ,gtsam::Matrix::Zero(3,dof()));
            
            /*  DH 矩阵对关节角的偏导数
                代码意义：长度 = dof()，和 H 一一对应
                来源：由你之前的 getdH(i, theta) 函数计算得到 */

            std::vector<gtsam::Matrix4> dh(dof());


            /*  代码意义：Ho 矩阵的逆，长度和 Ho 完全一致
            来源：Hoinv[i] = Ho[i].inverse() */
            std::vector<gtsam::Matrix4> Hoinv(dof() +1);
            

            // 前向运动学递推的【起点】：世界坐标系 → 机械臂底座
            Ho[0] = base_pose_.matrix();
            // 缓存底座变换的逆矩阵
            Hoinv[0] = Ho[0].inverse();
            //https://www.doubao.com/thread/w206e26b524c01819
        }
}