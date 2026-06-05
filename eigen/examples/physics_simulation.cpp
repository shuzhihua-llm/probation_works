#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Sparse>

// ============================================================
// 应用场景 4: 物理仿真 - 动力学方程求解
// ============================================================
// 核心概念:
//   - 有限元方法: 将连续问题离散化为线性方程组
//   - 弹簧质点系统: F = kx
//   - 稀疏矩阵: 高效存储大规模物理系统
// ============================================================

// 1. 弹簧质点系统 (1D)
void demo_spring_mass_system() {
    std::cout << "=== 弹簧质点系统 ===" << std::endl;

    // 5个质点, 4个弹簧
    int n = 5;
    double k = 100.0;  // 弹簧刚度

    // 构建刚度矩阵 K
    Eigen::MatrixXd K = Eigen::MatrixXd::Zero(n, n);
    for (int i = 0; i < n; ++i) {
        if (i > 0) {
            K(i, i) += k;
            K(i, i - 1) -= k;
            K(i - 1, i) -= k;
            K(i - 1, i - 1) += k;
        }
    }

    // 边界条件: 固定第一个质点
    K(0, 0) = 1e10;

    // 外力 (重力)
    Eigen::VectorXd F(n);
    F.setZero();
    F.tail(n - 1) << 9.8, 9.8, 9.8, 9.8;

    // 求解位移: K * x = F
    Eigen::VectorXd displacement = K.ldlt().solve(F);

    std::cout << "弹簧刚度矩阵:\n" << K << std::endl;
    std::cout << "外力: " << F.transpose() << std::endl;
    std::cout << "位移: " << displacement.transpose() << std::endl;
    std::cout << std::endl;
}

// 2. 热传导方程 (2D 网格)
void demo_heat_equation() {
    std::cout << "=== 2D 热传导方程 ===" << std::endl;

    int nx = 4, ny = 4;
    int n = nx * ny;
    double dx = 1.0, dy = 1.0;
    double alpha = 0.1;  // 热扩散系数

    // 构建拉普拉斯矩阵 (5点 stencil)
    Eigen::SparseMatrix<double> L(n, n);
    std::vector<Eigen::Triplet<double>> triplets;

    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            int idx = i * ny + j;
            double diag = 0.0;

            // 上邻居
            if (j > 0) {
                triplets.push_back({idx, idx - 1, 1.0 / (dy * dy)});
                diag -= 1.0 / (dy * dy);
            }
            // 下邻居
            if (j < ny - 1) {
                triplets.push_back({idx, idx + 1, 1.0 / (dy * dy)});
                diag -= 1.0 / (dy * dy);
            }
            // 左邻居
            if (i > 0) {
                triplets.push_back({idx, idx - ny, 1.0 / (dx * dx)});
                diag -= 1.0 / (dx * dx);
            }
            // 右邻居
            if (i < nx - 1) {
                triplets.push_back({idx, idx + ny, 1.0 / (dx * dx)});
                diag -= 1.0 / (dx * dx);
            }

            triplets.push_back({idx, idx, diag});
        }
    }

    L.setFromTriplets(triplets.begin(), triplets.end());

    std::cout << "拉普拉斯矩阵 (稀疏, nnz=" << L.nonZeros() << "):\n"
              << Eigen::MatrixXd(L) << std::endl;

    // 初始温度分布
    Eigen::VectorXd T0(n);
    T0.setZero();
    T0(5) = 100.0;  // 中心加热

    // 显式欧拉法: T(t+dt) = T(t) + alpha * dt * L * T(t)
    double dt = 0.1;
    Eigen::VectorXd T = T0;

    for (int step = 0; step < 10; ++step) {
        T = T + alpha * dt * (L * T);
    }

    std::cout << "10步后的温度分布:\n";
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            std::cout << T(i * ny + j) << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// 3. 刚体动力学 (简化2D)
void demo_rigid_body_dynamics() {
    std::cout << "=== 2D 刚体动力学 ===" << std::endl;

    // 状态: [x, y, theta, vx, vy, omega]
    Eigen::Matrix<double, 6, 1> state;
    state << 0, 0, 0, 1, 0.5, 0.2;

    double dt = 0.016;  // 60fps
    double mass = 1.0;
    double inertia = 0.5;

    // 外力 [Fx, Fy, tau]
    Eigen::Vector3d force;
    force << 1.0, -0.5, 0.1;

    // 质量矩阵
    Eigen::Matrix<double, 6, 6> M = Eigen::Matrix<double, 6, 6>::Identity();
    M(3, 3) = mass;
    M(4, 4) = mass;
    M(5, 5) = inertia;

    // 加速度
    Eigen::Matrix<double, 6, 1> accel = Eigen::Matrix<double, 6, 1>::Zero();
    accel(3) = force(0) / mass;
    accel(4) = force(1) / mass;
    accel(5) = force(2) / inertia;

    // 欧拉积分
    state.head(3) += dt * state.tail(3);
    state.tail(3) += dt * accel.tail(3);

    std::cout << "初始状态: " << state.transpose() << std::endl;
    std::cout << "1步后状态: " << state.transpose() << std::endl;
    std::cout << std::endl;
}

// 4. 有限元分析 (1D 杆)
void demo_fem_1d() {
    std::cout << "=== 1D 杆有限元分析 ===" << std::endl;

    int n_elements = 4;
    double E = 200e9;   // 弹性模量 (钢)
    double A = 1e-4;    // 截面积
    double L = 1.0;     // 总长度
    double Le = L / n_elements;  // 单元长度

    // 全局刚度矩阵
    Eigen::MatrixXd K_global(n_elements + 1, n_elements + 1);
    K_global.setZero();

    // 单元刚度矩阵 k = EA/Le * [1 -1; -1 1]
    double ke = E * A / Le;
    for (int e = 0; e < n_elements; ++e) {
        K_global(e, e) += ke;
        K_global(e, e + 1) -= ke;
        K_global(e + 1, e) -= ke;
        K_global(e + 1, e + 1) += ke;
    }

    // 边界条件: 固定左端
    K_global(0, 0) = 1e15;

    // 外力: 右端施加 1000N 拉力
    Eigen::VectorXd F(n_elements + 1);
    F.setZero();
    F(n_elements) = 1000.0;

    // 求解位移
    Eigen::VectorXd U = K_global.ldlt().solve(F);

    std::cout << "节点位移 (mm):\n";
    for (int i = 0; i <= n_elements; ++i) {
        std::cout << "节点 " << i << ": " << U(i) * 1e6 << " μm" << std::endl;
    }

    // 计算应力
    std::cout << "\n单元应力 (MPa):\n";
    for (int e = 0; e < n_elements; ++e) {
        double strain = (U(e + 1) - U(e)) / Le;
        double stress = E * strain / 1e6;  // 转换为 MPa
        std::cout << "单元 " << e << ": " << stress << " MPa" << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    demo_spring_mass_system();
    demo_heat_equation();
    demo_rigid_body_dynamics();
    demo_fem_1d();

    return 0;
}
