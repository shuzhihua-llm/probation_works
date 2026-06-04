#include <iostream>
#include <Eigen/Dense>

int main() {
    std::cout << "=== 逆矩阵与行列式 ===" << std::endl;
    
    Eigen::Matrix2d A;
    A << 1, 2,
         3, 4;
    
    std::cout << "Matrix A:\n" << A << std::endl;
    
    // 1. 计算行列式
    double det = A.determinant();
    std::cout << "Determinant of A: " << det << std::endl;
    
    // 2. 计算逆矩阵
    Eigen::Matrix2d A_inv = A.inverse();
    std::cout << "Inverse of A:\n" << A_inv << std::endl;
    
    // 3. 验证: A * A^-1 应该等于单位矩阵
    Eigen::Matrix2d identity = A * A_inv;
    std::cout << "A * A^-1 (should be identity):\n" << identity << std::endl;
    
    // 4. 尝试奇异矩阵
    std::cout << "\n=== 奇异矩阵测试 ===" << std::endl;
    Eigen::Matrix2d B;
    B << 1, 2,
         2, 4;
    
    std::cout << "Matrix B:\n" << B << std::endl;
    double det_B = B.determinant();
    std::cout << "Determinant of B: " << det_B << std::endl;
    
    Eigen::Matrix2d B_inv = B.inverse();
    std::cout << "Inverse of B (will be inf/nan):\n" << B_inv << std::endl;
    
    return 0;
}
