#include <iostream>
#include <Eigen/Dense>

int main() {
    std::cout << "=== 线性方程组求解 ===" << std::endl;
    
    // 求解 Ax = b
    // A = [1 2; 3 4]
    // b = [5; 11]
    // 手动计算: x = [1; 2]
    
    Eigen::Matrix2d A;
    A << 1, 2,
         3, 4;
    
    Eigen::Vector2d b;
    b << 5, 11;
    
    std::cout << "Matrix A:\n" << A << std::endl;
    std::cout << "Vector b:\n" << b << std::endl;
    
    // 1. 使用 QR 分解求解
    Eigen::Vector2d x = A.colPivHouseholderQr().solve(b);
    std::cout << "Solution x:\n" << x << std::endl;
    
    // 2. 验证解: 计算 A * x
    std::cout << "A * x:\n" << A * x << std::endl;
    
    // 3. 计算误差: (A * x - b).norm()
    std::cout << "Error: " << (A * x - b).norm() << std::endl;
    
    return 0;
}
