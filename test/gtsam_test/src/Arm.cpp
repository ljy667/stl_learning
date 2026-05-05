

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
                用途：最终计算末端执行器位姿、构建雅可比矩阵的核心数据 
                */
            std::vector<gtsam::Matrix4> Ho(dof()+1);

            
            /*  基坐标系 → 连杆 / 关节 全局齐次变换矩阵
                jv：布尔标志，true 表示需要计算雅可比
                分配 dof() 个 3×dof() 矩阵：3 行 (x/y/z 线速度)，dof()列 (对应每个关节)
                来源：由你之前的 getJvj(...) 函数逐列填充
                用途：建立「关节速度 → 末端线速度」的映射，是运动控制 / 轨迹优化的核心 
                
                每个自由度都需要一个雅可比矩阵，
                */

            std::vector<gtsam::Matrix> J ; //雅可比
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
            for(std::size_t i; i <= dof(); i++ ){
                
                //计算i-1关节，相对于jp关节配置后的变化矩阵
                H[i-1] = getH(i-1 , jp(i-1));
                Ho[i] = Ho[i] * H[i-1];  

                if(jv){
                    for(std::size_t j = 0 ; j < i ; j++)
                    {   
                        // 基于变换矩阵的雅可比矩阵
                        
                        //  J[i-1] = 第 i 根连杆的线速度雅可比矩阵
                        //  雅可比第 j 列的物理意义：
                        //  第 j 个关节转动时，对第 i 根连杆线速度的唯一贡献
                        
                        //  只会是之前的连杆的关节的转动 对之后的关节产生影响
                        J[i-1].col(j) = getJvj(Ho[i], Ho[j]) ; 
                    }
                }

                if(J_jpx_jp){

                    //缓存：单关节DH矩阵对关节角的导数
                    dh[i-1] = getdh(i-1 , jp(i-1)) ; 
                    //缓存：单关节DH矩阵对关节角的导数的逆
                    Hoinv[i] = Ho[i].inverse() ;
                }
            }
            // diff Jvj to vJp
            std::vector<std::vector<gtsam::Matrix4>> dho_dq(dof() , std::vector<gtsam::Matrix4>(dof()));
            //创建并初始化了一个 DOF × DOF 大小的二维动态数组（矩阵）
            //https://www.doubao.com/thread/w15472d13f70150a0
            
            // J_jpx_jp：雅可比矩阵 对 关节位置 的导数
            // J_jvx_jp：雅可比矩阵 对 关节速度 的导数
            if(J_jpx_jp || J_jvx_jv ){
                for(std::size_t i = 0 ; i < dof() ; i++)
                for(std::size_t j=0 ; j <= i ; j++ )
                if(i > j)
                //情况1：j<i (非末端关节，需要递推)
                
                // Ho[i+1]=Ho[0]⋅H[0]⋅H[1]⋅⋯⋅H[i]
                
                // 只有 H[j] 与关节角 q_j 相关，对 q_j 求导时：
                //https://www.doubao.com/thread/w150bd2bcc03f928f
                dho_dq[i][j] = Ho[j] * dh[j] * Hoinv[j+1] * Ho[i+1];
                else
                dho_dq[i][j] = Ho[j] * dh[j];    
            }
            
            //开始计算位置/速度正运动学 / 雅可比
            for(std::size_t i = 0 ; i < dof() ;i++){
                //基于变换矩阵，jpx的i对应 ho[i+1],因为ho[i+1]数组的首元素存储了世界坐标系 → 机械臂底座的旋转矩阵
                
                jpx[i] = gtsam::Pose3(gtsam::Rot3(Ho[i+1].block<3,3>(0,0)),
                    gtsam::Point3(Ho[i+1].col(3).head<3>()));

                if(jv && jvx)
                {
                    //std::optional<std::reference_wrapper<std::vector<gtsam::Vector3>>> jvx,
                    (*jvx).get()[i] = J[i] * (*jv).get()[i] ;
                }

                //体雅可比矩阵，SE(3)李群
                //https://www.doubao.com/thread/wb81035be68e0f10d

                //https://www.doubao.com/thread/w773ea478b4328489

                if(J_jpx_jp){ 
                gtsam::Matrix Jp =  (*J_jpx_jp).get()[i];
                const gtsam::Matrix4 inv_jpx_i = jpx[i].inverse().matrix();
                for(std::size_t j = 0 ; j<= i ; j++) 
                {   
                    //从李代数中提取雅可比矩阵列
                    const gtsam::Matrix4 sym_se3 = inv_jpx_i * dho_dq[i][j];
                    Jp.col(j) = (gtsam::Vector6() << gtsam::Vector3(sym_se3(2,1), sym_se3(0,2),
                        sym_se3(1,0)), sym_se3.col(3).head<3>()).finished();   
                    }
                }
            
            // diff Jvj to vJp
            if (J_jvx_jp) {
            gtsam::Matrix& Jv = (*J_jvx_jp).get()[i];
            //Jv.setZero();
            // Jv each col <= j (j <= i)
            // Jv.col(j) = dvxi_dq.col(j) = d_Ji_qj * vi
            for (size_t j = 0; j <= i; j++) {
                    gtsam::Matrix d_Ji_qj = gtsam::Matrix::Zero(3, dof());

                    // for d_Ji_qj only first i cols have values
                    // d_Ji_qj.col(k) = d(getJvj(i, k-1))_dqj  (k <= i)
                    d_Ji_qj.col(0) = getdJvj(Ho[i+1], Ho[0], dho_dq[i][j], gtsam::Matrix4::Zero());    // k-1 < 0, use zero for dH-1
                    for (size_t k = 1; k <= i; k++) {
                    if (k-1 >= j)
                        d_Ji_qj.col(k) = getdJvj(Ho[i+1], Ho[k], dho_dq[i][j], dho_dq[k-1][j]);
                    else
                        d_Ji_qj.col(k) = getdJvj(Ho[i+1], Ho[k], dho_dq[i][j],gtsam::Matrix4::Zero());    // zero dHo_dq when j > k-1
                    }
                    Jv.col(j) = d_Ji_qj * (*jv).get();
                }
            }

            // vJv is simply J
            if (J_jvx_jv)
            (*J_jvx_jv).get()[i] = J[i];
        }
            
        }
}