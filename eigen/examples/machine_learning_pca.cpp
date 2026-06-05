#include <iostream>
#include <vector>
#include <Eigen/Dense>

// ============================================================
// 应用场景 3: 机器学习 - PCA 降维与最小二乘
// ============================================================
// 核心概念:
//   - PCA: 通过协方差矩阵特征值分解找到数据主要方向
//   - 最小二乘: 线性回归的参数估计
//   - SVD: 矩阵分解的核心工具
// ============================================================

// 1. PCA 降维 (主成分分析)
void demo_pca() {
    std::cout << "=== PCA 降维 ===" << std::endl;

    // 生成模拟数据 (100个样本, 3维特征)
    int n_samples = 100;
    int n_features = 3;
    Eigen::MatrixXd X(n_samples, n_features);

    // 生成相关数据: x3 = 2*x1 + 3*x2 + noise
    for (int i = 0; i < n_samples; ++i) {
        X(i, 0) = rand() % 100 / 100.0;
        X(i, 1) = rand() % 100 / 100.0;
        X(i, 2) = 2.0 * X(i, 0) + 3.0 * X(i, 1) + (rand() % 10 - 5) * 0.01;
    }

    // 步骤1: 数据中心化 (减去均值)
    Eigen::RowVectorXd mean = X.colwise().mean();
    X = X.rowwise() - mean;

    // 步骤2: 计算协方差矩阵
    Eigen::MatrixXd cov = (X.transpose() * X) / (n_samples - 1);

    // 步骤3: 特征值分解
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(cov);
    Eigen::VectorXd eigenvalues = solver.eigenvalues();
    Eigen::MatrixXd eigenvectors = solver.eigenvectors();

    // 输出特征值 (按升序排列)
    std::cout << "特征值: " << eigenvalues.transpose() << std::endl;
    std::cout << "解释方差比例: "
              << (eigenvalues / eigenvalues.sum()).transpose() * 100 << "%" << std::endl;

    // 步骤4: 投影到主成分 (保留前2个)
    Eigen::MatrixXd W = eigenvectors.rightCols(2);  // 取最大的2个特征值对应的向量
    Eigen::MatrixXd X_pca = X * W;

    std::cout << "降维后数据维度: " << X_pca.rows() << " x " << X_pca.cols() << std::endl;
    std::cout << "前5个样本的PCA结果:\n" << X_pca.topRows(5) << std::endl;
    std::cout << std::endl;
}

// 2. 线性回归 (最小二乘法)
void demo_linear_regression() {
    std::cout << "=== 线性回归 (最小二乘法) ===" << std::endl;

    // 生成数据: y = 3*x1 - 2*x2 + 5 + noise
    int n = 50;
    Eigen::MatrixXd X(n, 3);  // 特征矩阵 (含偏置项)
    Eigen::VectorXd y(n);

    for (int i = 0; i < n; ++i) {
        X(i, 0) = rand() % 100 / 100.0;
        X(i, 1) = rand() % 100 / 100.0;
        X(i, 2) = 1.0;  // 偏置项
        y(i) = 3.0 * X(i, 0) - 2.0 * X(i, 1) + 5.0 + (rand() % 10 - 5) * 0.1;
    }

    // 最小二乘解: w = (X^T X)^-1 X^T y
    // 使用 QR 分解 (数值稳定)
    Eigen::VectorXd w = X.householderQr().solve(y);

    std::cout << "真实参数: [3, -2, 5]" << std::endl;
    std::cout << "估计参数: " << w.transpose() << std::endl;

    // 计算 R² 分数
    Eigen::VectorXd y_pred = X * w;
    double ss_res = (y - y_pred).squaredNorm();
    double ss_tot = (y - y.mean() * Eigen::VectorXd::Ones(n)).squaredNorm();
    double r_squared = 1.0 - ss_res / ss_tot;
    std::cout << "R² 分数: " << r_squared << std::endl;
    std::cout << std::endl;
}

// 3. SVD 矩阵分解 (推荐系统基础)
void demo_svd() {
    std::cout << "=== SVD 矩阵分解 ===" << std::endl;

    // 用户-物品评分矩阵 (5用户 x 4物品)
    Eigen::MatrixXd R(5, 4);
    R << 5, 3, 0, 1,
         4, 0, 0, 1,
         1, 1, 0, 5,
         1, 0, 0, 4,
         0, 1, 5, 4;

    std::cout << "原始评分矩阵:\n" << R << std::endl;

    // SVD 分解: R = U * Σ * V^T
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(R, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::MatrixXd U = svd.matrixU();
    Eigen::VectorXd S = svd.singularValues();
    Eigen::MatrixXd V = svd.matrixV();

    std::cout << "奇异值: " << S.transpose() << std::endl;

    // 低秩近似 (保留前2个奇异值)
    int k = 2;
    Eigen::MatrixXd R_approx = U.leftCols(k) * S.head(k).asDiagonal() * V.leftCols(k).transpose();

    std::cout << "低秩近似 (k=2):\n" << R_approx << std::endl;
    std::cout << std::endl;
}

// 4. K-Means 聚类 (简化版)
void demo_kmeans() {
    std::cout << "=== K-Means 聚类 ===" << std::endl;

    // 2D 数据点
    Eigen::MatrixXd X(10, 2);
    X << 1, 2,
         1, 3,
         2, 2,
         2, 3,
         8, 9,
         8, 8,
         9, 9,
         9, 8,
         5, 5,
         5, 6;

    int k = 2;
    int max_iter = 10;

    // 初始化聚类中心 (选择前两个点)
    Eigen::MatrixXd centers(k, 2);
    centers << 1, 2,
               8, 9;

    // 迭代
    for (int iter = 0; iter < max_iter; ++iter) {
        // 分配每个点到最近的中心
        Eigen::VectorXi labels(10);
        for (int i = 0; i < 10; ++i) {
            double min_dist = std::numeric_limits<double>::max();
            for (int c = 0; c < k; ++c) {
                double dist = (X.row(i) - centers.row(c)).squaredNorm();
                if (dist < min_dist) {
                    min_dist = dist;
                    labels(i) = c;
                }
            }
        }

        // 更新聚类中心
        for (int c = 0; c < k; ++c) {
            int count = (labels.array() == c).count();
            if (count > 0) {
                Eigen::MatrixXd mask = (labels.array() == c).cast<double>();
                centers.row(c) = (mask.replicate(1, 2).array() * X.array()).colwise().sum() / count;
            }
        }

        std::cout << "迭代 " << iter + 1 << " - 中心:\n" << centers << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    demo_pca();
    demo_linear_regression();
    demo_svd();
    demo_kmeans();

    return 0;
}
