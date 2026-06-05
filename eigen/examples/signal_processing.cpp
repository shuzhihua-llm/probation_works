#include <iostream>
#include <cmath>
#include <Eigen/Dense>

// ============================================================
// 应用场景 6: 信号处理 - 滤波器设计与谱分析
// ============================================================
// 核心概念:
//   - 卷积: 滤波器的核心运算
//   - 低通/高通滤波器: 去除噪声或提取高频特征
//   - 自相关: 信号周期性分析
// ============================================================

// 1. 1D 卷积 (滤波器实现)
void demo_convolution_1d() {
    std::cout << "=== 1D 卷积 (滤波器) ===" << std::endl;

    // 输入信号 (含噪声)
    Eigen::VectorXd signal(20);
    for (int i = 0; i < 20; ++i) {
        signal(i) = std::sin(2 * M_PI * i / 10.0) + (rand() % 10 - 5) * 0.1;
    }

    // 移动平均滤波器 (低通)
    Eigen::VectorXd kernel = Eigen::VectorXd::Ones(5) / 5.0;

    // 卷积 (same mode)
    int n = signal.size();
    int k = kernel.size();
    Eigen::VectorXd filtered(n);

    for (int i = 0; i < n; ++i) {
        double sum = 0;
        for (int j = 0; j < k; ++j) {
            int idx = i - k / 2 + j;
            if (idx >= 0 && idx < n) {
                sum += signal(idx) * kernel(j);
            }
        }
        filtered(i) = sum;
    }

    std::cout << "原始信号 (前10): " << signal.head(10).transpose() << std::endl;
    std::cout << "滤波信号 (前10): " << filtered.head(10).transpose() << std::endl;
    std::cout << std::endl;
}

// 2. 离散傅里叶变换 (DFT)
void demo_dft() {
    std::cout << "=== 离散傅里叶变换 ===" << std::endl;

    // 生成信号: 50Hz + 120Hz 正弦波
    int N = 64;
    double fs = 1000.0;  // 采样率
    Eigen::VectorXd x(N);

    for (int i = 0; i < N; ++i) {
        double t = i / fs;
        x(i) = 0.7 * std::sin(2 * M_PI * 50 * t) + 0.3 * std::sin(2 * M_PI * 120 * t);
    }

    // DFT 矩阵
    Eigen::MatrixXcd F(N, N);
    for (int k = 0; k < N; ++k) {
        for (int n = 0; n < N; ++n) {
            double angle = -2 * M_PI * k * n / N;
            F(k, n) = std::complex<double>(std::cos(angle), std::sin(angle));
        }
    }

    // 频域表示
    Eigen::VectorXcd X = F * x;
    Eigen::VectorXd magnitude = X.array().abs() * (2.0 / N);

    // 显示前 N/2 个频率分量
    std::cout << "频谱幅度 (前16):\n";
    for (int k = 0; k < 16; ++k) {
        double freq = k * fs / N;
        std::cout << freq << "Hz: " << magnitude(k) << std::endl;
    }
    std::cout << std::endl;
}

// 3. 自相关分析
void demo_autocorrelation() {
    std::cout << "=== 自相关分析 ===" << std::endl;

    // 周期性信号 (周期为10)
    int N = 100;
    Eigen::VectorXd signal(N);
    for (int i = 0; i < N; ++i) {
        signal(i) = std::sin(2 * M_PI * i / 10.0) + (rand() % 20 - 10) * 0.05;
    }

    // 自相关 (使用矩阵乘法)
    int max_lag = 30;
    Eigen::VectorXd autocorr(max_lag);

    double mean = signal.mean();
    Eigen::VectorXd centered = signal.array() - mean;
    double variance = centered.squaredNorm();

    for (int lag = 0; lag < max_lag; ++lag) {
        double sum = 0;
        for (int i = 0; i < N - lag; ++i) {
            sum += centered(i) * centered(i + lag);
        }
        autocorr(lag) = sum / variance;
    }

    // 找第一个过零点 (估计周期)
    int period_est = 0;
    for (int i = 1; i < max_lag; ++i) {
        if (autocorr(i) < 0) {
            period_est = i * 2;  // 过零点对应半周期
            break;
        }
    }

    std::cout << "自相关峰值: " << autocorr.head(20).transpose() << std::endl;
    std::cout << "估计周期: " << period_est << " (真实: 10)" << std::endl;
    std::cout << std::endl;
}

// 4. 维纳滤波器 (最小均方误差)
void demo_wiener_filter() {
    std::cout << "=== 维纳滤波器 ===" << std::endl;

    // 观测信号: s + n (信号 + 噪声)
    int N = 50;
    Eigen::VectorXd s(N), n(N), x(N);

    for (int i = 0; i < N; ++i) {
        s(i) = std::sin(2 * M_PI * i / 20.0);
        n(i) = (rand() % 100 - 50) * 0.02;
        x(i) = s(i) + n(i);
    }

    // 自相关矩阵 (信号)
    int filter_order = 5;
    Eigen::MatrixXd R(filter_order, filter_order);
    Eigen::VectorXd p(filter_order);

    // 估计自相关
    for (int i = 0; i < filter_order; ++i) {
        for (int j = 0; j < filter_order; ++j) {
            double sum = 0;
            for (int k = 0; k < N - std::abs(i - j); ++k) {
                sum += x(k) * x(k + std::abs(i - j));
            }
            R(i, j) = sum / N;
        }
    }

    // 互相关向量
    for (int i = 0; i < filter_order; ++i) {
        double sum = 0;
        for (int k = 0; k < N - i; ++k) {
            sum += s(k + i) * x(k);
        }
        p(i) = sum / N;
    }

    // 维纳滤波器系数: h = R^-1 * p
    Eigen::VectorXd h = R.ldlt().solve(p);

    std::cout << "维纳滤波器系数: " << h.transpose() << std::endl;

    // 应用滤波器
    Eigen::VectorXd y(N);
    for (int i = filter_order - 1; i < N; ++i) {
        double sum = 0;
        for (int j = 0; j < filter_order; ++j) {
            sum += h(j) * x(i - j);
        }
        y(i) = sum;
    }

    // 计算信噪比改善
    double snr_before = s.head(N - filter_order + 1).squaredNorm() /
                        (x.head(N - filter_order + 1) - s.head(N - filter_order + 1)).squaredNorm();
    double snr_after = s.head(N - filter_order + 1).squaredNorm() /
                       (y.tail(N - filter_order + 1) - s.head(N - filter_order + 1)).squaredNorm();

    std::cout << "滤波前 SNR: " << 10 * std::log10(snr_before) << " dB" << std::endl;
    std::cout << "滤波后 SNR: " << 10 * std::log10(snr_after) << " dB" << std::endl;
    std::cout << std::endl;
}

int main() {
    demo_convolution_1d();
    demo_dft();
    demo_autocorrelation();
    demo_wiener_filter();

    return 0;
}
