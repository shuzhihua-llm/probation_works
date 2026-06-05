#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>

// ============================================================
// 应用场景 1: 机器人/SLAM - 坐标变换与位姿估计
// ============================================================
// 核心概念:
//   - 刚体变换 = 旋转 (3x3矩阵) + 平移 (3x1向量)
//   - 齐次变换矩阵 (4x4) 统一表示旋转和平移
//   - 四元数避免万向节死锁
// ============================================================

// 1. 齐次变换矩阵: 统一表示旋转和平移
void demo_homogeneous_transform() {
    std::cout << "=== 齐次变换矩阵 ===" << std::endl;

    // 从基坐标系到相机坐标系的变换
    Eigen::Matrix4d T_base_camera = Eigen::Matrix4d::Identity();

    // 设置旋转部分 (绕Z轴旋转45度)
    double angle = M_PI / 4.0;
    T_base_camera.block<3, 3>(0, 0) = Eigen::AngleAxisd(angle, Eigen::Vector3d::UnitZ()).toRotationMatrix();

    // 设置平移部分 (x=1, y=2, z=0.5)
    T_base_camera.block<3, 1>(0, 3) << 1.0, 2.0, 0.5;

    std::cout << "基坐标系到相机坐标系的变换矩阵:\n" << T_base_camera << std::endl;

    // 将点从基坐标系变换到相机坐标系
    Eigen::Vector4d point_base(1.0, 0.0, 0.0, 1.0);
    Eigen::Vector4d point_camera = T_base_camera * point_base;
    std::cout << "基坐标系中的点: " << point_base.head(3).transpose() << std::endl;
    std::cout << "相机坐标系中的点: " << point_camera.head(3).transpose() << std::endl;
    std::cout << std::endl;
}

// 2. 四元数表示旋转 (机器人姿态)
void demo_quaternion_rotation() {
    std::cout << "=== 四元数旋转 ===" << std::endl;

    // 使用四元数表示机器人姿态 (避免万向节死锁)
    Eigen::Quaterniond q_roll(Eigen::AngleAxisd(M_PI / 6, Eigen::Vector3d::UnitX()));  // 横滚角
    Eigen::Quaterniond q_pitch(Eigen::AngleAxisd(M_PI / 4, Eigen::Vector3d::UnitY())); // 俯仰角
    Eigen::Quaterniond q_yaw(Eigen::AngleAxisd(M_PI / 3, Eigen::Vector3d::UnitZ()));   // 偏航角

    // 复合旋转 (ZYX欧拉角顺序)
    Eigen::Quaterniond q_total = q_yaw * q_pitch * q_roll;
    q_total.normalize();  // 四元数必须归一化

    std::cout << "四元数 (w, x, y, z): " << q_total.coeffs().transpose() << std::endl;

    // 转换为旋转矩阵
    Eigen::Matrix3d R = q_total.toRotationMatrix();
    std::cout << "旋转矩阵:\n" << R << std::endl;

    // 应用旋转到向量
    Eigen::Vector3d v(1.0, 0.0, 0.0);
    Eigen::Vector3d v_rotated = q_total * v;
    std::cout << "原始向量: " << v.transpose() << std::endl;
    std::cout << "旋转后向量: " << v_rotated.transpose() << std::endl;
    std::cout << std::endl;
}

// 3. 卡尔曼滤波 (目标跟踪)
void demo_kalman_filter() {
    std::cout << "=== 卡尔曼滤波 (2D目标跟踪) ===" << std::endl;

    // 状态向量: [x, y, vx, vy]
    Eigen::Vector4d state = Eigen::Vector4d::Zero();
    Eigen::Matrix4d P = Eigen::Matrix4d::Identity() * 10;  // 状态协方差

    // 状态转移矩阵 (恒速模型)
    double dt = 0.1;
    Eigen::Matrix4d F = Eigen::Matrix4d::Identity();
    F(0, 2) = dt;  // x += vx * dt
    F(1, 3) = dt;  // y += vy * dt

    // 观测矩阵 (只观测位置)
    Eigen::Matrix<double, 2, 4> H = Eigen::Matrix<double, 2, 4>::Zero();
    H(0, 0) = 1;
    H(1, 1) = 1;

    // 过程噪声协方差
    Eigen::Matrix4d Q = Eigen::Matrix4d::Identity() * 0.01;

    // 观测噪声协方差
    Eigen::Matrix2d R = Eigen::Matrix2d::Identity() * 1.0;

    // 模拟观测数据
    for (int i = 0; i < 5; ++i) {
        // 预测步骤
        state = F * state;
        P = F * P * F.transpose() + Q;

        // 生成模拟观测 (带噪声)
        Eigen::Vector2d z;
        z << i * 0.5 + (rand() % 10 - 5) * 0.1, i * 0.3 + (rand() % 10 - 5) * 0.1;

        // 更新步骤
        Eigen::Matrix2d S = H * P * H.transpose() + R;
        Eigen::Matrix<double, 4, 2> K = P * H.transpose() * S.inverse();

        state = state + K * (z - H * state);
        P = (Eigen::Matrix4d::Identity() - K * H) * P;

        std::cout << "Step " << i + 1 << " - 状态: " << state.transpose()
                  << "  观测: " << z.transpose() << std::endl;
    }
    std::cout << std::endl;
}

// 4. 手眼标定 (AX = XB 求解)
void demo_hand_eye_calibration() {
    std::cout << "=== 手眼标定 (简化版) ===" << std::endl;

    // 假设已知: 机器人末端两次位姿的变换 T_ee1_ee2
    //           相机两次位姿的变换 T_cam1_cam2
    // 求解: 机器人末端到相机的变换 T_ee_cam

    Eigen::Matrix4d T_ee1_ee2 = Eigen::Matrix4d::Identity();
    T_ee1_ee2.block<3, 3>(0, 0) = Eigen::AngleAxisd(0.1, Eigen::Vector3d::UnitZ()).toRotationMatrix();
    T_ee1_ee2(0, 3) = 0.05;

    Eigen::Matrix4d T_cam1_cam2 = Eigen::Matrix4d::Identity();
    T_cam1_cam2.block<3, 3>(0, 0) = Eigen::AngleAxisd(0.1, Eigen::Vector3d::UnitZ()).toRotationMatrix();
    T_cam1_cam2(1, 3) = -0.05;

    std::cout << "机器人末端变换:\n" << T_ee1_ee2 << std::endl;
    std::cout << "相机变换:\n" << T_cam1_cam2 << std::endl;
    std::cout << std::endl;
}

int main() {
    demo_homogeneous_transform();
    demo_quaternion_rotation();
    demo_kalman_filter();
    demo_hand_eye_calibration();

    return 0;
}
