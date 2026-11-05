// ============================================================
//  card_operations.h
//  校园一卡通消费管理系统 —— 头文件（模块七：多文件编程）
//
//  说明：头文件只写“声明”——系统常量、数据结构、异常类型、函数原型；
//        所有函数体都写在 card_operations.cpp 中，
//        主程序 CampusCardSystem.cpp 通过 #include "card_operations.h" 使用这些接口。
//  编译标准：C++11
// ============================================================
#ifndef CARD_OPERATIONS_H          // 防止头文件被重复包含
#define CARD_OPERATIONS_H

#include <string>
#include <stdexcept>

// ------------------------------------------------------------
//  模块一：系统常量
// ------------------------------------------------------------
// 充值手续费率：每次充值按 2% 收取手续费
const double FEE_RATE        = 0.02;
// 低余额警戒值：余额低于 10 元时给出提示
const double WARNING_BALANCE = 10.0;

// ------------------------------------------------------------
//  模块一：学生一卡通账户结构体
// ------------------------------------------------------------
struct Card {
    int         id;            // 学号
    std::string name;          // 姓名
    double      balance;       // 当前余额（元）
    int         transactions;  // 交易次数（每次充值或消费各计 1 次）

    // 默认构造函数：new Card[n] 时保证每个元素的成员都有确定的初值
    Card() : id(0), balance(0.0), transactions(0) {}
};

// ------------------------------------------------------------
//  模块九：自定义异常类型
//  （统一派生自 std::runtime_error，便于在主程序中分类捕获）
// ------------------------------------------------------------
class CardException : public std::runtime_error {
public:
    explicit CardException(const std::string &message)
        : std::runtime_error(message) {}
};

// 输入数据非法（学号非正、余额为负、格式错误等）
class InvalidInputException : public CardException {
public:
    explicit InvalidInputException(const std::string &message)
        : CardException(message) {}
};

// 账户余额不足
class InsufficientBalanceException : public CardException {
public:
    explicit InsufficientBalanceException(const std::string &message)
        : CardException(message) {}
};

// ------------------------------------------------------------
//  模块二、模块四：命名空间 card 中的系统功能
// ------------------------------------------------------------
namespace card {

// ----- 模块二：格式化输出 -----
void printLine(char c = '-', int len = 60);   // 打印由 len 个字符 c 组成的分隔线（带默认参数）
void title(const std::string &s);             // 打印居中标题

// ----- 模块四：账户操作 -----
// 充值：扣除手续费后增加余额并累加交易次数（fee 带默认参数）
void deposit(Card &c, double amount, double fee = FEE_RATE);

// 消费：余额足够则扣款、累加交易次数并返回 true，否则返回 false
bool spend(Card &c, double cost);

// 消费（重载）：先输出消费项目，再调用上面的版本完成扣款
bool spend(Card &c, double cost, const std::string &item);

// 常引用查询：只读显示账户余额，函数内部无法修改账户数据
void showBalance(const Card &c);

// 指向常量的指针查询：只读显示账户 ID
void viewStudentID(const Card *p);

} // namespace card

// ------------------------------------------------------------
//  模块三：动态内存管理与账户录入
// ------------------------------------------------------------
Card *createCards(int n);            // 用 new[] 动态创建 n 个账户
void  destroyCards(Card *p);         // 用 delete[] 释放账户数组
void  inputCards(Card *arr, int n);  // 从键盘录入 n 个账户的信息

// ------------------------------------------------------------
//  模块五：强制类型转换
// ------------------------------------------------------------
// 金额由“元”换算为“分”（内部使用 static_cast<int>）
int toCent(double amount);

// ------------------------------------------------------------
//  模块六：报表输出
// ------------------------------------------------------------
// 以表格形式输出所有账户的 ID、Name、Balance、Transactions
void printReport(const Card *arr, int n);

#endif // CARD_OPERATIONS_H
