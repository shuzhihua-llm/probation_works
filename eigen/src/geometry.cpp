#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>

int main() {
    std::cout << "=== 几何变换 ===" << std::endl;
    
    // 1. 2D 旋转矩阵
    std::cout << "\n=== 2D 旋转 ===" << std::endl;
    double angle = M_PI / 4;  // 45度
    
    Eigen::Rotation2D<double> rot2d(angle);
    Eigen::Matrix2d R = rot2d.toRotationMatrix();
    
    std::cout << "旋转矩阵 (45度):\n" << R << std::endl;
    
    Eigen::Vector2d point(1, 0);
    Eigen::Vector2d rotated = R * point;
    std::cout << "点 (1,0) 旋转后: " << rotated.transpose() << std::endl;
    
    // 2. 3D 旋转矩阵
    std::cout << "\n=== 3D 旋转 ===" << std::endl;
    
    // 绕 Z 轴旋转 90 度
    Eigen::AngleAxisd rotZ(M_PI / 2, Eigen::Vector3d::UnitZ());
    Eigen::Matrix3d Rz = rotZ.toRotationMatrix();
    
    std::cout << "绕 Z 轴旋转 90 度:\n" << Rz << std::endl;
    
    Eigen::Vector3d p3d(1, 0, 0);
    std::cout << "点 (1,0,0) 旋转后: " << (Rz * p3d).transpose() << std::endl;
    
    // 3. 四元数
    std::cout << "\n=== 四元数 ===" << std::endl;
    
    Eigen::Quaterniond q(rotZ);
    std::cout << "四元数: w=" << q.w() << ", x=" << q.x() << ", y=" << q.y() << ", z=" << q.z() << std::endl;
    
    // 四元数旋转
    Eigen::Vector3d rotated_q = q * p3d;
    std::cout << "四元数旋转结果: " << rotated_q.transpose() << std::endl;
    
    // 4. 欧拉角
    std::cout << "\n=== 欧拉角 ===" << std::endl;
    
    Eigen::Vector3d euler_angles(0, 0, M_PI / 2);  // roll, pitch, yaw
    Eigen::Matrix3d R_euler;
    R_euler = Eigen::AngleAxisd(euler_angles[0], Eigen::Vector3d::UnitX()) *
              Eigen::AngleAxisd(euler_angles[1], Eigen::Vector3d::UnitY()) *
              Eigen::AngleAxisd(euler_angles[2], Eigen::Vector3d::UnitZ());
    
    std::cout << "欧拉角旋转矩阵:\n" << R_euler << std::endl;
    
    // 5. 仿射变换
    std::cout << "\n=== 仿射变换 ===" << std::endl;
    
    Eigen::Affine3d transform = Eigen::Affine3d::Identity();
    transform.rotate(Eigen::AngleAxisd(M_PI / 4, Eigen::Vector3d::UnitZ()));
    transform.translate(Eigen::Vector3d(1, 2, 0));
    
    Eigen::Vector3d p(0, 0, 0);
    Eigen::Vector3d transformed = transform * p;
    std::cout << "原点 (0,0,0) 旋转+平移后: " << transformed.transpose() << std::endl;
    
    return 0;
}
