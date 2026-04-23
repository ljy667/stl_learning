
#pragma once

#include "ForwardKinematics.hpp"
//#include <gpmp2/config.h>

#include <gtsam/geometry/Pose3.h>
#include <gtsam/base/Matrix.h>
#include <gtsam/base/Vector.h>


#include <vector>
#include <cmath>


namespace gpmp2 {

/**
 * Abstract arm class use DH parameters, without any physical model representation
 * Inherited from ForwardKinematics
 */

class Arm : public ForwardKinematics<gtsam::Vector , gtsam::Vector> {
  private:

    typedef ForwardKinematics<gtsam::Vector , gtsam::Vector> Base ;

    gtsam::Vector a_ , alpha_ , d_ ;
    mutable gtsam::Pose3 base_pose_;
    gtsam::Vector theta_bias_ ;      //关节角偏差

    std::vector<gtsam::Pose3> link_trans_notheta_;

  public:
    Arm(){} 

    Arm(std::size_t dof , const gtsam::Vector& a, const gtsam::Vector& alpha , 
      const gtsam::Vector& d) :
      Arm(dof, a , alpha , d , gtsam::Pose3(gtsam::Rot3() , gtsam::Point3(0,0,0)) , gtsam::Vector::Zero(dof)){
      }
    
    Arm(std::size_t dof , const gtsam::Vector& a , const gtsam::Vector& alpha , const gtsam::Vector& d , 
      const gtsam::Pose3& base_pose) :
      Arm(dof , a , alpha , d , base_pose , gtsam::Vector::Zero(dof)){}

    Arm(std::size_t dof , const gtsam::Vector& a, const gtsam::Vector& alpha ,
    const gtsam::Vector& d , const gtsam::Pose3& base_pose , const gtsam::Vector& theta_bias);

    virtual ~Arm() {} ;


    void forwardKinematics(
        const gtsam::Vector& jp,
        std::optional<std::reference_wrapper<const gtsam::Vector>> jv,
        std::vector<gtsam::Pose3>& jpx,
        std::optional<std::reference_wrapper<std::vector<gtsam::Vector3>>> jvx,
        std::optional<std::reference_wrapper<std::vector<gtsam::Matrix>>> J_jpx_jp = std::nullopt,
        std::optional<std::reference_wrapper<std::vector<gtsam::Matrix>>> J_jpv_jp = std::nullopt,
        std::optional<std::reference_wrapper<std::vector<gtsam::Matrix>>> J_jvx_jp = std::nullopt
    ) const override;  

    ///updata base pose in const

    void updateBasePose(const gtsam::Pose3& p) const
    { base_pose_ = p ;}

    ///access
    const gtsam::Vector& a() const { return a_;}
    const gtsam::Vector& d() const { return d_;}
    const gtsam::Vector& alpha() const { return alpha_; }
    const gtsam::Pose3& base_pose() const { return base_pose_;}

  private:

    ///
    
    /* 输入：第i个关节 + 该关节转动的角度theta
    输出：这个关节转动后的 3D 位姿（旋转 + 平移） */

    gtsam::Pose3 getJointTrans(std::size_t i , double theta) const {
      //触发条件：i >= dof() 时程序直接终止并报错；
      assert(i< dof());

      // 2. 核心计算：返回 第i个关节 旋转后的【总位姿变换】
      // 公式：关节自身的旋转变换 × 连杆固定几何变换
      return gtsam::Pose3(
                          // 第一部分：关节自身的旋转（绕Z轴转动）
                          
                          // 各个连杆的坐标系 z轴与旋转平面垂直 
                          // 连杆都是绕z轴旋转

                          gtsam::Rot3::Rz(theta + theta_bias_(i)),
                          // 平移：关节旋转中心不动，所以是(0,0,0)
                          gtsam::Point3(0,0,0)) 
                          * link_trans_notheta_[i] ;
                          // 第二部分：第i个连杆的【固定结构变换】（机械臂硬件决定，永远不变）         
    }

    gtsam::Matrix4 getH(size_t i , double theta) const {
      return getJointTrans(i,theta).matrix(); 
    }

    //https://www.doubao.com/thread/wf2b36e84e310d6fa
    gtsam::Matrix4 getdh(std::size_t i , double theta) const {
      assert(i < dof());
      const double c = cos(theta + theta_bias_(i)) ,s = sin(theta + theta_bias_(i));
      //涉及的公式，仅与z相关相关的旋转矩阵导数， 因为theta仅会影响z
      const gtsam::Matrix4 dRot = (gtsam::Matrix4() << 
        -s, -c, 0,  0,
        c,  -s, 0,  0,
        0,  0,  0,  0,
        0,  0,  0,  0
        ).finished();

      return dRot * link_trans_notheta_[i].matrix() ;

    }

    //https://www.doubao.com/thread/wd6c047d0a8c1dbcb

    // Hoi,基座标系 -> 要控制连杆（连杆i）位置的旋转矩阵
    // Hoj 关节j 到基座标的旋转矩阵 ， 
    // 即要计算关节j转动 对连杆i的影响  

    // v = ω × r ,  Jvj = z_j × ( P_i − P_j )
    /*  ω：角速度向量（方向 = 旋转轴方向，大小 = 转速）
        r：从旋转中心指向运动点的向量（r = Pi − Pj）
        ×：叉乘 */

    gtsam::Vector3 getJvj(const gtsam::Matrix4& Hoi , const gtsam::Matrix4& Hoj) const{
      // ×乘无直接运算符，反对称矩阵（skewSymmetric） 完成运算，即a×b = skewSymmetric(a) * b。

        return gtsam::skewSymmetric(Hoj.col(2).head<3>())*
        (Hoi.col(3).head<3>() - Hoj.col(3).head<3>());
    }

    //https://www.doubao.com/thread/wb80015725e01058d

    /// 计算雅可比矩阵（线速度部分）第 j 列的导数
    //关节角 θ 发生微小变化时，雅可比矩阵的变化速率

    /* 参数	    全称	           数学定义	            来源函数	      代码中提取的核心数据
        Hoi	  基→连杆 i   变换	    无导数的齐次矩阵	getH(i, θ)	      Pi​ (位置)
        Hoj	  基→关节 j   变换	    无导数的齐次矩阵	getH(j, θ)	      zj​ (旋转轴)、Pj​ (位置)
        dHoi	基→连杆 i   变换的导数	dHoi​/dθ	      getdH(i, θ)	      P˙i​ (位置导数)
        dHoj	基→关节 j   变换的导数	dHoj​/dθ	      getdH(j, θ)	      z˙j​ (旋转轴导数)、P˙j​ (位置导数) */

    gtsam::Vector3 getdJvj(const gtsam::Matrix4& Hoi, const gtsam::Matrix4& Hoj,
      const gtsam::Matrix4& dHoi, const gtsam::Matrix4& dHoj) const {
    // 公式：z_j×(dP_i-dP_j) + dz_j×(P_i-P_j)
    return gtsam::skewSymmetric(Hoj.col(2).head<3>()) *
        (dHoi.col(3).head<3>() - dHoj.col(3).head<3>()) +
        gtsam::skewSymmetric(dHoj.col(2).head<3>()) *
        (Hoi.col(3).head<3>() - Hoj.col(3).head<3>());
}

};

  


}


