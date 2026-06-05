#include <iostream>
#include <Eigen/Dense>

// 应用1：数据去噪
void denoise_example() {
    std::cout << "=== 应用1：数据去噪 ===" << std::endl;
    
    // 原始信号（低秩）
    Eigen::MatrixXd signal(5, 5);
    signal << 1, 2, 3, 4, 5,
              2, 4, 6, 8, 10,
              3, 6, 9, 12, 15,
              4, 8, 12, 16, 20,
              5, 10, 15, 20, 25;
    
    // 添加噪声
    Eigen::MatrixXd noise = Eigen::MatrixXd::Random(5, 5) * 0.5;
    Eigen::MatrixXd noisy = signal + noise;
    
    std::cout << "带噪声矩阵:\n" << noisy << std::endl;
    
    // SVD 去噪：保留主要奇异值
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(noisy, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::VectorXd S = svd.singularValues();
    
    // 只保留前2个奇异值（原始信号秩为1）
    Eigen::MatrixXd S_k = Eigen::MatrixXd::Zero(2, 2);
    S_k(0,0) = S(0);
    S_k(1,1) = S(1);
    
    Eigen::MatrixXd denoised = svd.matrixU().leftCols(2) * S_k * svd.matrixV().leftCols(2).transpose();
    
    std::cout << "\n去噪后矩阵:\n" << denoised << std::endl;
    std::cout << "\n与原始信号误差: " << (signal - denoised).norm() / signal.norm() * 100 << "%" << std::endl;
}

// 应用2：求解超定方程组（最小二乘）
void least_squares_example() {
    std::cout << "\n=== 应用2：最小二乘拟合 ===" << std::endl;
    
    // 数据点：(1,2), (2,3), (3,5), (4,4), (5,6)
    // 拟合直线 y = a*x + b
    Eigen::MatrixXd A(5, 2);
    A << 1, 1,
         2, 1,
         3, 1,
         4, 1,
         5, 1;
    
    Eigen::VectorXd b(5);
    b << 2, 3, 5, 4, 6;
    
    // SVD 求解最小二乘
    Eigen::VectorXd x = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);
    
    std::cout << "拟合直线: y = " << x(0) << " * x + " << x(1) << std::endl;
    std::cout << "残差: " << (A * x - b).norm() << std::endl;
}

// 应用3：计算矩阵伪逆
void pseudoinverse_example() {
    std::cout << "\n=== 应用3：矩阵伪逆 ===" << std::endl;
    
    Eigen::MatrixXd A(3, 2);
    A << 1, 2,
         3, 4,
         5, 6;
    
    // SVD 计算伪逆 A^+ = V * Σ^+ * U^T
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::VectorXd S = svd.singularValues();
    
    // Σ^+：奇异值取倒数
    Eigen::MatrixXd S_pinv = Eigen::MatrixXd::Zero(2, 2);
    S_pinv(0,0) = 1.0 / S(0);
    S_pinv(1,1) = 1.0 / S(1);
    
    Eigen::MatrixXd A_pinv = svd.matrixV() * S_pinv * svd.matrixU().leftCols(2).transpose();
    
    std::cout << "矩阵 A:\n" << A << std::endl;
    std::cout << "\n伪逆 A^+:\n" << A_pinv << std::endl;
    std::cout << "\n验证 A * A^+ * A ≈ A:\n" << A * A_pinv * A << std::endl;
}

int main() {
    denoise_example();
    least_squares_example();
    pseudoinverse_example();
    return 0;
}
