// ============================================================
//  CampusCardSystem.cpp
//  校园一卡通消费管理系统 —— 主程序文件（模块八、模块十）
//
//  本项目共三个文件（模块七：多文件编程）：
//      card_operations.h      —— 常量、结构体、异常类型与函数声明
//      card_operations.cpp    —— 各功能函数的具体实现
//      CampusCardSystem.cpp   —— 主程序：用户交互与功能调用
//
//  编译（MSVC）：cl /EHsc /utf-8 /W4 /std:c++14 /Fe:CampusCardSystem.exe CampusCardSystem.cpp card_operations.cpp
//  编译（GCC） ：g++ -std=c++11 -Wall -Wextra -o CampusCardSystem CampusCardSystem.cpp card_operations.cpp
// ============================================================
#include "card_operations.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <new>
#include <string>

#ifdef _WIN32
// windows.h 会把 min/max 定义成宏，从而与 std::numeric_limits<>::max() 冲突，
// 故先定义 NOMINMAX 再包含该头文件。
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#endif

// MSVC 默认按系统代码页（中文 Windows 为 GBK）解释窄字符串字面量，而本文件
// 采用 UTF-8 编码，这里强制字面量也按 UTF-8 编码，与 enableUtf8Console()
// 设置的 UTF-8 控制台代码页保持一致；其他编译器会忽略该指示。
#if defined(_MSC_VER)
#  pragma execution_character_set("utf-8")
#endif

namespace {

// Windows 控制台默认使用 GBK(936) 代码页，而源文件是 UTF-8 编码，
// 这里把输入/输出代码页都切换为 UTF-8，避免中文提示和姓名显示为乱码。
// 非 Windows 平台（Linux / macOS）终端本身就是 UTF-8，无需处理。
void enableUtf8Console() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

// 读取学生数量：输入非法时抛出异常，在本函数内部捕获并提示重新输入。
int readCardCount() {
    int n = 0;
    std::cout << "请输入学生数量：";

    for (;;) {
        try {
            if (!(std::cin >> n))
                throw InvalidInputException("学生数量必须是正整数");
            if (n <= 0)
                throw InvalidInputException("学生数量必须大于 0");
            return n;
        } catch (const InvalidInputException &e) {
            std::cout << "[错误] " << e.what() << "，请重新输入：";
            std::cin.clear();                    // 清除输入流错误状态
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (std::cin.eof())
                throw;                           // 输入流结束，交由 main 统一处理
        }
    }
}

} // namespace

int main() {
    enableUtf8Console();
    std::ios::sync_with_stdio(false);
    std::cout << std::fixed << std::setprecision(2);   // 金额统一保留两位小数

    Card *accounts = NULL;          // 指向动态数组的指针，便于异常路径下统一释放

    try {
        // ---------- 模块二：系统标题 ----------
        card::title("校园一卡通消费管理系统");

        // ---------- 模块八：输入学生数量 ----------
        const int n = readCardCount();

        // ---------- 模块三：动态创建账户数组并录入信息 ----------
        accounts = createCards(n);
        std::cout << "内存分配成功，可管理 " << n << " 个账户。\n";
        inputCards(accounts, n);

        // ---------- 模块四：充值（带默认参数的手续费率） ----------
        card::printLine('-');
        std::cout << "模块四：账户充值（手续费率 " << FEE_RATE * 100.0 << "%）\n";
        card::deposit(accounts[0], 100.0);           // 第一个账户充值 100 元

        // ---------- 模块四：消费（重载版本，带消费项目） ----------
        std::cout << "模块四：账户消费\n";
        if (!card::spend(accounts[0], 8.5, "食堂午餐"))   // 消费 8.5 元
            throw InsufficientBalanceException("账户余额不足，消费 8.50 元失败");

        // ---------- 模块四：查询（常引用 / 指向常量的指针） ----------
        std::cout << "模块四：账户信息查询\n";
        card::showBalance(accounts[0]);              // const Card&
        card::viewStudentID(accounts);               // const Card*

        // ---------- 模块五：金额单位换算 ----------
        std::cout << "模块五：金额单位换算\n";
        std::cout << "余额 " << accounts[0].balance << " 元 = "
                  << toCent(accounts[0].balance) << " 分\n";

        // ---------- 模块九：异常处理演示 ----------
        card::printLine('-');
        std::cout << "模块九：异常处理演示\n";

        // 演示 1：非法充值金额（负数）—— 由 deposit() 主动抛出异常
        try {
            card::deposit(accounts[0], -50.0);
        } catch (const InvalidInputException &e) {
            std::cout << "[异常已捕获] " << e.what() << '\n';
        }

        // 演示 2：消费金额超过余额 —— spend() 返回 false，主程序抛出异常
        try {
            if (!card::spend(accounts[0], 1000000.0, "笔记本电脑"))
                throw InsufficientBalanceException("账户余额不足，无法完成该笔消费");
        } catch (const InsufficientBalanceException &e) {
            std::cout << "[异常已捕获] " << e.what() << '\n';
        }

        // ---------- 模块六：输出全部账户报表 ----------
        card::printLine('-');
        std::cout << "模块六：账户报表\n";
        printReport(accounts, n);

        // ---------- 模块三：释放动态内存 ----------
        destroyCards(accounts);       // delete[]
        accounts = NULL;
        std::cout << "动态账户数组已释放（delete[]）。\n";

        card::printLine('=');
        std::cout << "程序正常结束。\n";
        return 0;
    }
    // ---------- 模块九：统一的异常捕获 ----------
    catch (const std::bad_alloc &) {          // 内存申请失败
        std::cout << "[致命错误] 内存分配失败，程序安全退出。\n";
    } catch (const CardException &e) {        // 本系统自定义的异常
        std::cout << "[错误] " << e.what() << '\n';
    } catch (const std::exception &e) {       // 其他标准异常
        std::cout << "[未知错误] " << e.what() << '\n';
    }

    destroyCards(accounts);       // 异常路径下同样释放内存，避免内存泄漏
    return 1;
}
