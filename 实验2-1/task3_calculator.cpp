// ============================================================
//  实验任务三：C++ 特色函数考察（简易多功能计算器）
//  题目：综合运用“内联函数、带默认参数值的函数、函数重载”实现计算器：
//    1. inline double square(double x);                     —— 求平方
//    2. double power(double base, int exponent = 2);        —— 求乘幂（缺省指数为 2）
//    3. double calc(double a, double b, char op);           —— 四则运算（double 重载）
//       double calc(int    a, int    b, char op);           —— 四则运算（int    重载）
//
//  说明：样例输出为 "power(3,3) = 27.00"、"power(10,3) = 1000.00"，
//        即演示时指数取 3，所以主函数调用 power(base, 3)；
//        若省略第二个实参写成 power(base)，则按默认值 2 计算，等价于 square(base)。
//
//  编译（MSVC）：cl /EHsc /utf-8 /W4 task3_calculator.cpp
//  编译（GCC） ：g++ -std=c++11 -Wall -Wextra -o task3_calculator task3_calculator.cpp
// ============================================================
#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <cctype>
#include <cstddef>

// ------------------------------------------------------------
// 函数声明（原型）：默认参数只在这里给出一次，函数定义中不再重复
// ------------------------------------------------------------
inline double square(double x);                     // 内联函数：平方
double power(double base, int exponent = 2);        // 带默认参数：乘幂
double calc(double a, double b, char op);           // 重载版本 1
double calc(int a, int b, char op);                 // 重载版本 2

namespace {

// 判断一个字符串是否为合法的整数字面量（用于决定调用哪一个 calc 重载版本）
bool isIntegerToken(const std::string &text) {
    if (text.empty()) return false;
    std::size_t i = 0;
    if (text[0] == '+' || text[0] == '-') i = 1;
    if (i >= text.size()) return false;
    for (; i < text.size(); ++i)
        if (!std::isdigit(static_cast<unsigned char>(text[i]))) return false;
    return text.size() <= 10;      // 简单限制长度，避免 std::stoi 溢出
}

} // namespace

int main() {
    std::ios::sync_with_stdio(false);

    // ---------- 读入两个操作数和一个运算符 ----------
    std::string textA, textB;
    char op = '\0';
    if (!(std::cin >> textA >> textB >> op)) {
        std::cerr << "输入错误：请按“操作数1 操作数2 运算符”的格式输入。\n";
        return 1;
    }

    double base = 0.0;
    const bool intMode = isIntegerToken(textA) && isIntegerToken(textB);   // 两个操作数都是整数？
    try {
        base = std::stod(textA);        // 第一操作数的数值，供 square/power 使用
    } catch (const std::exception &) {
        std::cerr << "输入错误：操作数不合法。\n";
        return 1;
    }

    std::cout << std::fixed << std::setprecision(2);    // 结果统一保留两位小数

    // ---------- 1. 调用内联函数 square ----------
    std::cout << "square(" << textA << ") = " << square(base) << '\n';

    // ---------- 2. 调用带默认参数的函数 power ----------
    // 样例要求指数为 3；省略第二个实参时 power(base) 会使用默认值 2。
    const int exponent = 3;
    std::cout << "power(" << textA << ',' << exponent << ") = "
              << power(base, exponent) << '\n';

    // ---------- 3. 调用重载函数 calc ----------
    try {
        // 两个操作数都是整数 → 调用 int 版本；否则调用 double 版本。
        // 条件表达式两个分支的类型都是 double，故整体结果为 double。
        const double result = intMode
            ? calc(std::stoi(textA), std::stoi(textB), op)
            : calc(std::stod(textA), std::stod(textB), op);

        std::cout << textA << ' ' << op << ' ' << textB << " = " << result << '\n';
    } catch (const std::exception &e) {
        // 除数为 0 时输出 "Error: divide by zero"
        std::cout << e.what() << '\n';
    }

    return 0;
}

// ------------------------------------------------------------
// 函数定义
// ------------------------------------------------------------

// 内联函数：在调用处展开，避免函数调用的开销
inline double square(double x) {
    return x * x;
}

// 带默认参数的函数：exponent 缺省为 2，即默认求平方
double power(double base, int exponent) {
    if (exponent == 0) return 1.0;

    const bool negative = (exponent < 0);
    const int  times    = negative ? -exponent : exponent;   // 避免下标为负
    double result = 1.0;
    for (int i = 0; i < times; ++i)
        result *= base;

    return negative ? 1.0 / result : result;
}

// 重载版本 1：double 版四则运算
double calc(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0.0)                                  // 除数为 0：抛出异常
                throw std::domain_error("Error: divide by zero");
            return a / b;
        default:
            throw std::invalid_argument("Error: unsupported operator");
    }
}

// 重载版本 2：int 版四则运算。
// 注意除法：若直接写 a / b 会发生整数除法（7/2 == 3），
// 因此先用 static_cast<double> 把操作数转换为浮点数再做除法。
double calc(int a, int b, char op) {
    switch (op) {
        case '+': return static_cast<double>(a) + b;
        case '-': return static_cast<double>(a) - b;
        case '*': return static_cast<double>(a) * b;
        case '/':
            if (b == 0)                                    // 除数为 0：抛出异常
                throw std::domain_error("Error: divide by zero");
            return static_cast<double>(a) / static_cast<double>(b);
        default:
            throw std::invalid_argument("Error: unsupported operator");
    }
}
