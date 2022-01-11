cmake_minimum_required(VERSION 3.16.0)
project(emu_engine VERSION 0.0.1 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_subdirectory(external/spdlog)
add_subdirectory(external/eigen)

find_package(spdlog)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(
    src
    external/spdlog/include
    external/eigen
    ${OPENGL_INCLUDE_DIRS}    
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIRS}
)

add_library(emu
    src/emu/src/shader.cpp
)
target_link_libraries(emu
    spdlog
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    glfw
    ${GLFW_LIBRARIES}
)

add_executable(main src/main.cpp)
target_link_libraries(main
    spdlog
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    glfw
    ${GLFW_LIBRARIES}
    emu
)