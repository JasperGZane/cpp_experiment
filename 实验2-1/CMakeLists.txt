# ============================================================
#  实验2-1（C++ 对 C 的改进和拓展）—— CMake 构建脚本
#  四个小题各自是独立的控制台程序，因此生成 4 个可执行文件。
#  用法：
#     mkdir build && cd build
#     cmake ..
#     cmake --build . --config Release
# ============================================================
cmake_minimum_required(VERSION 3.10)
project(Lab2_1_CPP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

foreach(task task1_student_scores task2_count_vowels task3_calculator task4_swap)
    add_executable(${task} ${task}.cpp)
    if (MSVC)
        # /utf-8：源文件按 UTF-8 解析，避免中文表头在 GBK 代码页下编译报错
        target_compile_options(${task} PRIVATE /utf-8 /W4)
    else()
        target_compile_options(${task} PRIVATE -Wall -Wextra)
    endif()
endforeach()
