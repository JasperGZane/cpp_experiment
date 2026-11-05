# ============================================================
#  校园一卡通消费管理系统 —— CMake 构建脚本（C++11）
#  用法：
#     mkdir build && cd build
#     cmake ..              （Visual Studio 下可加 -G "Visual Studio 17 2022" -A x64）
#     cmake --build . --config Release
# ============================================================
cmake_minimum_required(VERSION 3.10)
project(CampusCardSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(CampusCardSystem
    CampusCardSystem.cpp
    card_operations.cpp
)

if (MSVC)
    # /utf-8：源文件按 UTF-8 解析，避免中文提示在 GBK 代码页下编译报错
    target_compile_options(CampusCardSystem PRIVATE /utf-8 /W4)
else()
    target_compile_options(CampusCardSystem PRIVATE -Wall -Wextra)
endif()
