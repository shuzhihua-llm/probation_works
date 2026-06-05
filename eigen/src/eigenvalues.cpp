#include <iostream>
#include <Eigen/Dense>

int main() {
    std::cout << "=== 特征值分解 ===" << std::endl;
    
    // 创建对称矩阵（对称矩阵的特征值是实数）
    Eigen::Matrix3d A;
    A << 2, 1, 1,
         1, 2, 1,
         1, 1, 2;
    
    std::cout << "Matrix A:\n" << A << std::endl;
    
    // 1. 计算特征值和特征向量
    Eigen::EigenSolver<Eigen::Matrix3d> solver(A);
    
    // 2. 获取特征值
    Eigen::Vector3cd eigenvalues = solver.eigenvalues();
    std::cout << "\nEigenvalues:\n" << eigenvalues << std::endl;
    
    // 3. 获取特征向量
    Eigen::Matrix3cd eigenvectors = solver.eigenvectors();
    std::cout << "\nEigenvectors:\n" << eigenvectors << std::endl;
    
    // 4. 验证: A * v = lambda * v
    std::cout << "\n=== 验证 A * v = lambda * v ===" << std::endl;
    for (int i = 0; i < 3; i++) {
        auto lambda = eigenvalues(i);
        auto v = eigenvectors.col(i);
        auto Av = A * v;
        auto lambdaV = lambda * v;
        std::cout << "Eigenvalue " << i << ": " << lambda << std::endl;
        std::cout << "||A*v - lambda*v|| = " << (Av - lambdaV).norm() << std::endl;
    }
    
    return 0;
}
