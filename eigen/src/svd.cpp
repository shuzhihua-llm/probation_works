#include <iostream>
#include <Eigen/Dense>

int main() {
    std::cout << "=== SVD 奇异值分解 ===" << std::endl;
    
    // 任意矩阵（不需要是方阵）
    Eigen::MatrixXd A(3, 2);
    A << 1, 2,
         3, 4,
         5, 6;
    
    std::cout << "Matrix A (3x2):\n" << A << std::endl;
    
    // 1. 计算 SVD: A = U * Σ * V^T
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
    
    // 2. 获取 U, Σ, V
    Eigen::MatrixXd U = svd.matrixU();
    Eigen::MatrixXd S = svd.singularValues().asDiagonal();
    Eigen::MatrixXd V = svd.matrixV();
    
    std::cout << "\nU (左奇异向量):\n" << U << std::endl;
    std::cout << "\nΣ (奇异值):\n" << S << std::endl;
    std::cout << "\nV (右奇异向量):\n" << V << std::endl;
    
    // 3. 验证: A = U * Σ * V^T
    Eigen::MatrixXd reconstructed = U * S * V.transpose();
    std::cout << "\n验证 U * Σ * V^T:\n" << reconstructed << std::endl;
    std::cout << "\n误差: " << (A - reconstructed).norm() << std::endl;
    
    return 0;
}
