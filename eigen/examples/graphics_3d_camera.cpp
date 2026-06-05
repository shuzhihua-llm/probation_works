#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>

// ============================================================
// 应用场景 2: 3D 图形/游戏开发 - 相机变换与投影
// ============================================================
// 核心概念:
//   - 模型变换: 物体在世界空间中的位置
//   - 视图变换: 从相机视角看世界
//   - 投影变换: 3D 映射到 2D 屏幕
// ============================================================

// 1. 模型变换 (平移 + 旋转 + 缩放)
void demo_model_transform() {
    std::cout << "=== 模型变换 ===" << std::endl;

    // 使用 Transform 类型来组合变换
    Eigen::Affine3f model = Eigen::Affine3f::Identity();

    // 平移到 (2, 0, -5)
    model.translate(Eigen::Vector3f(2.0f, 0.0f, -5.0f));

    // 绕 Y 轴旋转 45 度
    model.rotate(Eigen::AngleAxisf(M_PI / 4.0f, Eigen::Vector3f::UnitY()));

    // 缩放 (2倍 X, 1倍 Y, 2倍 Z)
    model.scale(Eigen::Vector3f(2.0f, 1.0f, 2.0f));

    // 获取 4x4 矩阵
    Eigen::Matrix4f model_matrix = model.matrix();
    std::cout << "模型变换矩阵:\n" << model_matrix << std::endl;
    std::cout << std::endl;
}

// 2. 视图变换 (LookAt 相机)
void demo_view_transform() {
    std::cout << "=== 视图变换 (LookAt) ===" << std::endl;

    // 相机参数
    Eigen::Vector3f eye(0.0f, 2.0f, 5.0f);     // 相机位置
    Eigen::Vector3f target(0.0f, 0.0f, 0.0f);   // 观察目标
    Eigen::Vector3f up(0.0f, 1.0f, 0.0f);       // 上方向

    // 使用 Eigen 内置的 LookAt
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();
    
    // 计算相机坐标系
    Eigen::Vector3f z = (eye - target).normalized();
    Eigen::Vector3f x = up.cross(z).normalized();
    Eigen::Vector3f y = z.cross(x);

    view(0, 0) = x.x(); view(1, 0) = x.y(); view(2, 0) = x.z(); view(3, 0) = -x.dot(eye);
    view(0, 1) = y.x(); view(1, 1) = y.y(); view(2, 1) = y.z(); view(3, 1) = -y.dot(eye);
    view(0, 2) = z.x(); view(1, 2) = z.y(); view(2, 2) = z.z(); view(3, 2) = -z.dot(eye);

    std::cout << "视图矩阵:\n" << view << std::endl;
    std::cout << std::endl;
}

// 3. 透视投影
void demo_perspective_projection() {
    std::cout << "=== 透视投影 ===" << std::endl;

    float fov = 45.0f * M_PI / 180.0f;  // 视场角
    float aspect = 16.0f / 9.0f;         // 宽高比
    float near = 0.1f;                   // 近裁剪面
    float far = 100.0f;                  // 远裁剪面

    // 计算投影矩阵
    float f = 1.0f / tanf(fov / 2.0f);
    Eigen::Matrix4f proj = Eigen::Matrix4f::Zero();

    proj(0, 0) = f / aspect;
    proj(1, 1) = f;
    proj(2, 2) = (far + near) / (near - far);
    proj(2, 3) = (2 * far * near) / (near - far);
    proj(3, 2) = -1.0f;

    std::cout << "透视投影矩阵:\n" << proj << std::endl;

    // MVP 矩阵组合
    Eigen::Affine3f model = Eigen::Affine3f::Identity();
    model.translate(Eigen::Vector3f(0, 0, -3));
    model.rotate(Eigen::AngleAxisf(M_PI / 4, Eigen::Vector3f::UnitY()));
    Eigen::Matrix4f model_matrix = model.matrix();

    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();
    view(3, 2) = -5.0f;

    Eigen::Matrix4f mvp = proj * view * model_matrix;
    std::cout << "\nMVP 矩阵 (前3行):\n" << mvp.block<3, 4>(0, 0) << std::endl;
    std::cout << std::endl;
}

// 4. 骨骼动画 (父子层级变换)
void demo_skeletal_animation() {
    std::cout << "=== 骨骼动画 (父子层级) ===" << std::endl;

    // 简化骨骼: 根 -> 躯干 -> 上臂 -> 前臂
    Eigen::Affine3f root_transform = Eigen::Affine3f::Identity();
    root_transform.translate(Eigen::Vector3f(0, 1, 0));

    // 躯干相对于根
    Eigen::Affine3f torso_local = Eigen::Affine3f::Identity();
    torso_local.translate(Eigen::Vector3f(0, 0.5f, 0));
    torso_local.rotate(Eigen::AngleAxisf(0.1f, Eigen::Vector3f::UnitZ()));

    // 上臂相对于躯干
    Eigen::Affine3f upper_arm_local = Eigen::Affine3f::Identity();
    upper_arm_local.translate(Eigen::Vector3f(0.3f, 0.5f, 0));
    upper_arm_local.rotate(Eigen::AngleAxisf(-0.5f, Eigen::Vector3f::UnitX()));

    // 前臂相对于上臂
    Eigen::Affine3f forearm_local = Eigen::Affine3f::Identity();
    forearm_local.translate(Eigen::Vector3f(0.4f, 0, 0));
    forearm_local.rotate(Eigen::AngleAxisf(-0.8f, Eigen::Vector3f::UnitX()));

    // 计算全局变换 (世界坐标)
    Eigen::Affine3f torso_global = root_transform * torso_local;
    Eigen::Affine3f upper_arm_global = torso_global * upper_arm_local;
    Eigen::Affine3f forearm_global = upper_arm_global * forearm_local;

    // 获取前臂的世界坐标位置
    Eigen::Vector3f forearm_pos = forearm_global.translation();
    std::cout << "前臂世界坐标位置: " << forearm_pos.transpose() << std::endl;
    std::cout << std::endl;
}

// 5. 法线变换 (用于光照计算)
void demo_normal_transform() {
    std::cout << "=== 法线变换 ===" << std::endl;

    // 模型变换矩阵 (3x3 部分)
    Eigen::Affine3f model_3d;
    model_3d = Eigen::AngleAxisf(M_PI / 6, Eigen::Vector3f::UnitY());
    model_3d.scale(Eigen::Vector3f(2.0f, 1.0f, 0.5f));
    Eigen::Matrix3f model = model_3d.linear();

    // 法线变换矩阵 = (M^-1)^T
    Eigen::Matrix3f normal_matrix = model.inverse().transpose();

    // 原始法线
    Eigen::Vector3f normal(1, 0, 0);
    Eigen::Vector3f transformed_normal = (normal_matrix * normal).normalized();

    std::cout << "原始法线: " << normal.transpose() << std::endl;
    std::cout << "变换后法线: " << transformed_normal.transpose() << std::endl;
    std::cout << std::endl;
}

int main() {
    demo_model_transform();
    demo_view_transform();
    demo_perspective_projection();
    demo_skeletal_animation();
    demo_normal_transform();

    return 0;
}
