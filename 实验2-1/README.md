# 实验 2-1：C++ 对 C 的改进和拓展（四个小题）

本目录对应《第2-1章 实验任务-题目.md》中的四个实验任务，每个任务一个独立源文件、各自 `main()`。

## 一、文件清单

| 文件 | 对应任务 | 考察点 |
|---|---|---|
| `task1_student_scores.cpp` | 实验任务一 | 输入输出控制、`<iomanip>` 格式化（`setw`/`setprecision`/`fixed`）、动态数组 |
| `task2_count_vowels.cpp` | 实验任务二 | 指向常量的指针 `const char *` 遍历字符串 |
| `task3_calculator.cpp` | 实验任务三 | 内联函数、带默认参数的函数、函数重载、异常处理 |
| `task4_swap.cpp` | 实验任务四 | 引用作形参、函数重载（引用版 / 指针版 / double 版 / string 版） |
| `build.bat` | — | Windows + Visual Studio 一键编译（输出到 `build\`） |
| `CMakeLists.txt` | — | CMake 构建（生成 4 个可执行文件） |

## 二、编译与运行

### 方式 1：Visual Studio（图形界面）

1. “创建新项目 → 控制台应用（C++）”，分别把 4 个 `.cpp` 中的每一个建成一个工程（一个工程只能有一个 `main`）；
2. 项目属性 → C/C++ → 命令行 → 附加选项，填 `/utf-8`（源文件是 UTF-8 编码，加该选项可避免中文表头报错/乱码）；
3. `Ctrl + F5` 运行，或用下方“运行”命令在控制台手动输入数据。

### 方式 2：命令行（推荐，已实测通过）

```bat
rem 在本目录下执行
build.bat

rem 然后逐个运行（输入样例用重定向更方便）
build\task1_student_scores.exe
```

### 方式 3：CMake / GCC

```bash
mkdir build && cd build
cmake .. && cmake --build . --config Release

# 或者直接用 g++
g++ -std=c++11 -Wall -Wextra -o task1 task1_student_scores.cpp
```

## 三、各题运行结果

### 任务一：学生成绩信息表

输入：

```text
3
LiMing 90 85.5 78
WangHua 88 90 92
ZhangWei 70 68 75
```

输出：

```text
姓名             语文     数学     英语   平均分
------------------------------------------------
LiMing          90.00    85.50    78.00    84.50
WangHua         88.00    90.00    92.00    90.00  Excellent
ZhangWei        70.00    68.00    75.00    71.00
------------------------------------------------
```

说明：

- 平均分 = 三科之和 / 3.0，`fixed + setprecision(2)` 保留两位小数；
- 平均分 `>= 90` 的行末标注 `Excellent`（如 WangHua 恰好 90.00）；
- 题目要求“输出格式需整齐对齐”，故用 `setw` 思路做列对齐，并额外给出表头。
  由于 `setw` 按**字节数**计宽、而汉字在 UTF-8 中占 3 字节却只占 2 列，
  程序里用 `displayWidth()` 统计“显示宽度”后再补空格，保证中英文姓名混排时列不歪：

```text
姓名             语文     数学     英语   平均分
------------------------------------------------
李明            90.00    85.50    78.00    84.50
```

### 任务二：统计元音字母

输入：

```text
Hello World
```

输出：

```text
Number of vowels: 3
```

说明：`const char *p` 中 `const` 修饰的是 `*p`，函数内不能写 `*p = ...`，但指针本身可以 `++p` 向后移动，这正是“指向常量的指针”的特点。统计不区分大小写（`tolower`）。

### 任务三：简易多功能计算器

样例 1 输入 `3 5 +`，输出：

```text
square(3) = 9.00
power(3,3) = 27.00
3 + 5 = 8.00
```

样例 2 输入 `10 0 /`，输出：

```text
square(10) = 100.00
power(10,3) = 1000.00
Error: divide by zero
```

说明：

- `square()` 声明为 `inline`，用于求平方；
- `power(base, exponent = 2)` 的默认参数只在**声明**中给出一次，省略第二实参时按平方计算；
  样例输出为 `power(3,3)`、`power(10,3)`，演示时指数取 3，故主函数显式调用 `power(base, 3)`；
- 两个 `calc` 重载：两个操作数都是整数时调用 `int` 版（内部用 `static_cast<double>` 再做除法，避免 `7/2 == 3` 的整除截断），否则调用 `double` 版；
- 除数为 0 时 `calc` 抛出 `std::domain_error`，`main` 捕获后输出 `Error: divide by zero`。

### 任务四：引用与重载交换

输入：

```text
5 8
```

输出：

```text
Before swap: x=5, y=8
After  swap: x=8, y=5

[重载验证] void swap(int* x, int* y)  —— 指针版
  Before: m=8, n=5
  After : m=5, n=8
[重载验证] void swap(double& x, double& y) —— double 版
  Before: d1=1.25, d2=3.75
  After : d1=3.75, d2=1.25
[重载验证] void swap(std::string& x, std::string& y) —— string 版
  Before: s1=C, s2=C++
  After : s1=C++, s2=C
```

说明：

- 题目要求的 `void swap(int&, int&)` 用引用实现，`x`、`y` 是实参的别名，函数内赋值即修改实参，全程没有全局变量；
- 题目中“重载该函数 `void swap(….)`”给出了重载的要求，这里提供了**指针版** `swap(int*, int*)`（解引用后交换，附空指针保护）以及 `double` 版、`string` 版，用 `[重载验证]` 段落证明编译器能按实参类型正确选择版本；
- 前两行输出与题目样例完全一致，`After  swap` 中间是两个空格。

## 四、注意事项

1. 源文件为 **UTF-8（带 BOM）** 编码，并含 `#pragma execution_character_set("utf-8")`，
   因此无论是否加 `/utf-8` 编译选项，MSVC 下中文都不会乱码；GCC/Clang 会忽略该指示。
2. `build.bat` 会自动定位 Visual Studio 的 C++ 工具链（通过 `vswhere.exe`），
   并把中间文件与可执行文件都放进 `build\` 子目录，不污染源码目录。
3. 编译标准为 C++11（MSVC 下用 `/std:c++14`，因为 MSVC 没有 `/std:c++11` 选项，代码本身未使用 C++14 特性）。
