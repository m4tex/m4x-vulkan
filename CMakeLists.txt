cmake_minimum_required(VERSION 3.2)
project(m4xdev)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(m4xdev src/main.cpp)

target_link_libraries(m4xdev PRIVATE glm::glm  glfw Vulkan::Vulkan)
