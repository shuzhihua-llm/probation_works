#include <iostream>
#include <Eigen/Dense>

int main() {
    std::cout << "=== 矩阵元素访问与块操作 ===" << std::endl;
    
    // 1. 创建 3x3 矩阵
    Eigen::Matrix3d m;
    m << 1, 2, 3, 4, 5, 6, 7, 8, 9;
    
    std::cout << "Matrix m:\n" << m << std::endl;
    
    // 2. 访问单个元素 (行, 列)，索引从 0 开始
    for(int i = 0; i < m.rows(); ++i) {
        for(int j = 0; j < m.cols(); ++j) {
            std::cout << "Element at (" << i << ", " << j << ") = " << m(i, j) << " ";
        }
        std::cout << std::endl;
    }
    // 3. 获取第 2 行
    for(int j = 0; j < m.cols(); ++j) {
        std::cout << "Element at (1, " << j << ") = " << m(1, j) << " ";
    }
    std::cout << std::endl;
    // 4. 获取第 3 列
    for(int i = 0; i < m.rows(); ++i) {
        std::cout << "Element at (" << i << ", 2) = " << m(i, 2) << " ";
    }
    std::cout << std::endl;
    // 5. 提取 2x2 子块 (从第1行第1列开始)
    std::cout << "Submatrix (1:2, 1:2):\n" << m.block<2,2>(1,1) << std::endl;
    // 6. 修改某个元素的值
    m(1, 1) = 99;
    std::cout << "Matrix m after modification:\n" << m << std::endl;
    return 0;
}
