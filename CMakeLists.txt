cmake_minimum_required(VERSION 3.16.0)
project(emu_engine VERSION 0.0.1 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_subdirectory(external/spdlog)

find_package(spdlog)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

set(ON 1)

include_directories(
    src
    ${OPENGL_INCLUDE_DIRS}    
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
)

add_executable(main src/main.cpp)