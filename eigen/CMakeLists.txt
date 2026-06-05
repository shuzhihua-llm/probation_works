cmake_minimum_required(VERSION 3.10)
project(eigen_manual)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG 3.4.0
)
FetchContent_MakeAvailable(eigen)

add_executable(hello src/hello.cpp)
target_link_libraries(hello Eigen3::Eigen)

add_executable(hello_changd src/hello_changd.cpp)
target_link_libraries(hello_changd Eigen3::Eigen)

add_executable(matrix_ops src/matrix_ops.cpp)
target_link_libraries(matrix_ops Eigen3::Eigen)

# ========== 应用场景模板 ==========
add_executable(robot_slam examples/robot_slam_transform.cpp)
target_link_libraries(robot_slam Eigen3::Eigen)

add_executable(graphics_3d examples/graphics_3d_camera.cpp)
target_link_libraries(graphics_3d Eigen3::Eigen)

add_executable(machine_learning examples/machine_learning_pca.cpp)
target_link_libraries(machine_learning Eigen3::Eigen)

add_executable(physics_sim examples/physics_simulation.cpp)
target_link_libraries(physics_sim Eigen3::Eigen)

add_executable(vision_cv examples/vision_camera_calibration.cpp)
target_link_libraries(vision_cv Eigen3::Eigen)

add_executable(signal_proc examples/signal_processing.cpp)
target_link_libraries(signal_proc Eigen3::Eigen)
