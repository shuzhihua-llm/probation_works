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

# 基础学习文件
add_executable(hello src/hello.cpp)
target_link_libraries(hello Eigen3::Eigen)

add_executable(hello_changd src/hello_changd.cpp)
target_link_libraries(hello_changd Eigen3::Eigen)

add_executable(matrix_ops src/matrix_ops.cpp)
target_link_libraries(matrix_ops Eigen3::Eigen)

add_executable(matrix_access src/matrix_access.cpp)
target_link_libraries(matrix_access Eigen3::Eigen)

add_executable(linear_solve src/linear_solve.cpp)
target_link_libraries(linear_solve Eigen3::Eigen)

add_executable(matrix_inverse src/matrix_inverse.cpp)
target_link_libraries(matrix_inverse Eigen3::Eigen)
