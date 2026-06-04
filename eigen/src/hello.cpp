#include <iostream>
#include <Eigen/Dense>

int main() {
    // 创建一个 2x2 矩阵
    Eigen::Matrix2d m;
    m(0,0) = 1;
    m(0,1) = 2;
    m(1,0) = 3;
    m(1,1) = 4;
    
    std::cout << "My first matrix:\n" << m << std::endl;
    
    return 0;
}
