#include <iostream>
#include <Eigen/Dense>

int main(){
    Eigen::Matrix2d a,b;
    a << 1,2,3,4;
    b << 5,6,7,8;
    std::cout << " a = \n" << a << std::endl;
    std::cout << " b = \n" << b << std::endl;
    // 矩阵加法
    std::cout << " a + b = \n" << a + b << std::endl;
    // 矩阵乘法
    std::cout << " a * b = \n" << a * b << std::endl;
    // 矩阵转置
    std::cout << " a.transpose() = \n" << a.transpose() << std::endl;
    // 矩阵求逆
    std::cout << " a.inverse() = \n" << a.inverse() << std::endl;
    //标量乘法
    std::cout << " 2 * a = \n" << 2 * a << std::endl;
    // 矩阵的行列式
    std::cout << " a.determinant() = \n" << a.determinant() << std::endl;
    return 0;

}