// ============================================================
//  实验任务四：引用特性考察
//  题目：编写函数 void swap(int& x, int& y)，用 C++ 的“引用”交换两个整数；
//        然后重载该函数，用“指针”同样完成两个整数的交换。
//
//  要点：
//    1. 引用形参 int& x 是实参的别名，函数内修改 x 就是修改实参本身，
//       因此不需要指针、也不需要返回值；
//    2. 重载版本 void swap(int* x, int* y) 通过解引用 *x 修改实参；
//    3. 不使用任何全局变量；
//    4. 另外重载了 double 版与 std::string 版，用于验证重载的匹配规则。
//
//  编译（MSVC）：cl /EHsc /utf-8 /W4 task4_swap.cpp
//  编译（GCC） ：g++ -std=c++11 -Wall -Wextra -o task4_swap task4_swap.cpp
// ============================================================
#include <iostream>
#include <string>

// MSVC 默认按系统代码页（中文 Windows 为 GBK）解释窄字符串字面量，而本文件
// 采用 UTF-8 编码，这里强制字面量也按 UTF-8 编码，避免中文提示显示为乱码；
// 其他编译器（GCC / Clang）会忽略该指示。
#if defined(_MSC_VER)
#  pragma execution_character_set("utf-8")
#endif

// ------------------------------------------------------------
// 函数声明（重载）
// ------------------------------------------------------------
void swap(int &x, int &y);              // 版本 1：引用
void swap(int *x, int *y);              // 版本 2：指针（重载）
void swap(double &x, double &y);        // 版本 3：double 引用（重载）
void swap(std::string &x, std::string &y);   // 版本 4：string 引用（重载）

// ------------------------------------------------------------
// 函数定义
// ------------------------------------------------------------

// 版本 1：引用交换。x、y 是调用者实参的别名，赋值即作用于实参本身
void swap(int &x, int &y) {
    const int temp = x;
    x = y;
    y = temp;
}

// 版本 2：指针交换。形参是指针，必须解引用后才能读写实参；
//          未使用全局变量，且对空指针做了保护。
void swap(int *x, int *y) {
    if (x == NULL || y == NULL) return;
    const int temp = *x;
    *x = *y;
    *y = temp;
}

// 版本 3：两个 double 的交换（重载：形参类型不同）
void swap(double &x, double &y) {
    const double temp = x;
    x = y;
    y = temp;
}

// 版本 4：两个 string 的交换（重载：形参类型不同）
void swap(std::string &x, std::string &y) {
    std::string temp = x;      // 这里不能用 const，因为 temp 要参与赋值
    x = y;
    y = temp;
}

int main() {
    // ---------- 题目要求的整数交换 ----------
    int x = 0, y = 0;
    if (!(std::cin >> x >> y)) {
        std::cerr << "输入错误：请输入两个整数。\n";
        return 1;
    }

    std::cout << "Before swap: x=" << x << ", y=" << y << '\n';
    swap(x, y);                                  // 编译器按实参类型选择引用版
    std::cout << "After  swap: x=" << x << ", y=" << y << '\n';

    // ---------- 重载版本的验证（说明“重载”确实生效） ----------
    std::cout << "\n[重载验证] void swap(int* x, int* y)  —— 指针版\n";
    int m = x, n = y;
    std::cout << "  Before: m=" << m << ", n=" << n << '\n';
    swap(&m, &n);
    std::cout << "  After : m=" << m << ", n=" << n << '\n';

    std::cout << "[重载验证] void swap(double& x, double& y) —— double 版\n";
    double d1 = 1.25, d2 = 3.75;
    std::cout << "  Before: d1=" << d1 << ", d2=" << d2 << '\n';
    swap(d1, d2);
    std::cout << "  After : d1=" << d1 << ", d2=" << d2 << '\n';

    std::cout << "[重载验证] void swap(std::string& x, std::string& y) —— string 版\n";
    std::string s1 = "C", s2 = "C++";
    std::cout << "  Before: s1=" << s1 << ", s2=" << s2 << '\n';
    swap(s1, s2);
    std::cout << "  After : s1=" << s1 << ", s2=" << s2 << '\n';

    return 0;
}
