#include <iostream>
#include <Eigen/Dense>

// ============================================================
// 应用场景 5: 计算机视觉 - 相机标定与图像变换
// ============================================================
// 核心概念:
//   - 相机内参矩阵: 3D 点投影到 2D 图像
//   - 单应性矩阵: 平面间的透视变换
//   - 三角测量: 从多视图恢复 3D 点
// ============================================================

// 1. 相机内参与投影
void demo_camera_intrinsics() {
    std::cout << "=== 相机内参与投影 ===" << std::endl;

    // 相机内参矩阵 K
    double fx = 500, fy = 500;   // 焦距 (像素)
    double cx = 320, cy = 240;   // 光心 (像素)

    Eigen::Matrix3d K;
    K << fx, 0, cx,
         0, fy, cy,
         0, 0, 1;

    std::cout << "相机内参矩阵:\n" << K << std::endl;

    // 3D 世界点 (相机坐标系)
    Eigen::Vector3d point_3d(0.5, 0.3, 2.0);

    // 投影到归一化平面
    Eigen::Vector3d point_normalized = point_3d / point_3d.z();

    // 投影到像素坐标
    Eigen::Vector3d point_pixel_h = K * point_normalized;
    Eigen::Vector2d point_pixel = point_pixel_h.head(2);

    std::cout << "3D 点: " << point_3d.transpose() << std::endl;
    std::cout << "像素坐标: " << point_pixel.transpose() << std::endl;
    std::cout << std::endl;
}

// 2. 图像单应性变换 (透视变换)
void demo_homography() {
    std::cout << "=== 单应性变换 ===" << std::endl;

    // 4对对应点 (用于求解单应性矩阵)
    // 源图像中的点 (四边形)
    Eigen::Matrix<double, 8, 8> A = Eigen::Matrix<double, 8, 8>::Zero();
    Eigen::VectorXd b(8);

    // 源点 (文档四角)
    Eigen::Vector2d src[4] = {{0, 0}, {2100, 0}, {2100, 2970}, {0, 2970}};
    // 目标点 (图像四角)
    Eigen::Vector2d dst[4] = {{100, 100}, {500, 150}, {550, 600}, {80, 550}};

    // 构建线性方程组 Ah = 0
    for (int i = 0; i < 4; ++i) {
        double x = src[i].x(), y = src[i].y();
        double u = dst[i].x(), v = dst[i].y();

        A(2 * i, 0) = x; A(2 * i, 1) = y; A(2 * i, 2) = 1;
        A(2 * i, 6) = -u * x; A(2 * i, 7) = -u * y;

        A(2 * i + 1, 3) = x; A(2 * i + 1, 4) = y; A(2 * i + 1, 5) = 1;
        A(2 * i + 1, 6) = -v * x; A(2 * i + 1, 7) = -v * y;

        b(2 * i) = u;
        b(2 * i + 1) = v;
    }

    // 求解单应性矩阵参数 (简化版, 实际使用 DLT 算法)
    Eigen::VectorXd h = A.colPivHouseholderQr().solve(b);

    Eigen::Matrix3d H = Eigen::Matrix3d::Identity();
    H(0, 0) = h(0); H(0, 1) = h(1); H(0, 2) = h(2);
    H(1, 0) = h(3); H(1, 1) = h(4); H(1, 2) = h(5);
    H(2, 0) = h(6); H(2, 1) = h(7);

    std::cout << "单应性矩阵:\n" << H << std::endl;

    // 应用变换
    Eigen::Vector3d src_pt(1000, 1500, 1);
    Eigen::Vector3d dst_pt_h = H * src_pt;
    Eigen::Vector2d dst_pt = dst_pt_h.head(2) / dst_pt_h.z();

    std::cout << "源点 (1000, 1500) -> 目标点 " << dst_pt.transpose() << std::endl;
    std::cout << std::endl;
}

// 3. 三角测量 (双目视觉)
void demo_triangulation() {
    std::cout << "=== 三角测量 ===" << std::endl;

    // 左相机: P1 = K * [I | 0]
    Eigen::Matrix3d K;
    K << 500, 0, 320,
         0, 500, 240,
         0, 0, 1;

    Eigen::Matrix<double, 3, 4> P1;
    P1.setZero();
    P1.block<3, 3>(0, 0) = K;

    // 右相机: P2 = K * [R | t], 基线 0.1m
    Eigen::Matrix<double, 3, 4> P2;
    P2.setZero();
    P2.block<3, 3>(0, 0) = K;
    P2(0, 3) = -K(0, 0) * 0.1;  // 视差

    // 图像匹配点
    Eigen::Vector2d u1(400, 300);
    Eigen::Vector2d u2(350, 300);

    // 线性三角测量 (DLT)
    Eigen::Matrix4d A_tri;
    A_tri.row(0) = u1(0) * P1.row(2) - P1.row(0);
    A_tri.row(1) = u1(1) * P1.row(2) - P1.row(1);
    A_tri.row(2) = u2(0) * P2.row(2) - P2.row(0);
    A_tri.row(3) = u2(1) * P2.row(2) - P2.row(1);

    // SVD 求解
    Eigen::JacobiSVD<Eigen::Matrix4d> svd(A_tri, Eigen::ComputeFullV);
    Eigen::Vector4d X_hom = svd.matrixV().col(3);
    Eigen::Vector3d X_3d = X_hom.head(3) / X_hom.w();

    std::cout << "重建的 3D 点: " << X_3d.transpose() << std::endl;
    std::cout << std::endl;
}

// 4. 图像仿射变换
void demo_affine_transform() {
    std::cout << "=== 仿射变换 ===" << std::endl;

    // 3对点求解 2x3 仿射矩阵
    Eigen::Matrix3d A_affine;
    Eigen::Vector3d bx, by;

    // 源点
    Eigen::Vector2d p1(0, 0), p2(1, 0), p3(0, 1);
    // 目标点
    Eigen::Vector2d q1(50, 50), q2(150, 100), q3(50, 150);

    A_affine << p1.x(), p1.y(), 1,
                p2.x(), p2.y(), 1,
                p3.x(), p3.y(), 1;

    bx << q1.x(), q2.x(), q3.x();
    by << q1.y(), q2.y(), q3.y();

    Eigen::Vector3d ax = A_affine.colPivHouseholderQr().solve(bx);
    Eigen::Vector3d ay = A_affine.colPivHouseholderQr().solve(by);

    Eigen::Matrix<double, 2, 3> affine;
    affine.row(0) = ax.transpose();
    affine.row(1) = ay.transpose();

    std::cout << "仿射变换矩阵:\n" << affine << std::endl;

    // 应用变换
    Eigen::Vector3d pt(0.5, 0.5, 1);
    Eigen::Vector2d pt_transformed = affine * pt;
    std::cout << "点 (0.5, 0.5) -> " << pt_transformed.transpose() << std::endl;
    std::cout << std::endl;
}

// 5. PnP 求解 (3D-2D 对应)
void demo_pnp() {
    std::cout << "=== PnP 简化求解 ===" << std::endl;

    // 相机内参
    Eigen::Matrix3d K;
    K << 500, 0, 320,
         0, 500, 240,
         0, 0, 1;

    // 3D 点 (物体坐标系)
    std::vector<Eigen::Vector3d> points_3d = {
        {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}
    };

    // 对应的 2D 图像点
    std::vector<Eigen::Vector2d> points_2d = {
        {320, 240}, {400, 245}, {315, 320}, {325, 200}
    };

    // EPnP 或 DLT 简化求解 (实际使用更复杂的算法)
    // 这里演示反向投影误差计算

    Eigen::Matrix3d R = Eigen::Matrix3d::Identity();  // 初始旋转
    Eigen::Vector3d t(0, 0, 2);  // 初始平移

    // 计算重投影误差
    double total_error = 0;
    for (size_t i = 0; i < points_3d.size(); ++i) {
        Eigen::Vector3d X_cam = R * points_3d[i] + t;
        Eigen::Vector2d x_proj = (K * (X_cam / X_cam.z())).head(2);
        double error = (x_proj - points_2d[i]).norm();
        total_error += error;
        std::cout << "点 " << i << ": 预测 " << x_proj.transpose()
                  << " 实际 " << points_2d[i].transpose()
                  << " 误差 " << error << std::endl;
    }
    std::cout << "平均重投影误差: " << total_error / points_3d.size() << std::endl;
    std::cout << std::endl;
}

int main() {
    demo_camera_intrinsics();
    demo_homography();
    demo_triangulation();
    demo_affine_transform();
    demo_pnp();

    return 0;
}
