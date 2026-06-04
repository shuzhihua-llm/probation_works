#include <iostream>
#include <Eigen/Dense>

int main() {
    // 创建一个 3x3 矩阵
    Eigen::Matrix3d m;
    m(0,0) = 1;
    m(0,1) = 2;
    m(0,2) = 3;
    m(1,0) = 4;
    m(1,1) = 5;
    m(1,2) = 6;
    m(2,0) = 7;
    m(2,1) = 8;
    m(2,2) = 9;
    
    std::cout << "My first matrix:\n" << m << std::endl;
    
    return 0;
}