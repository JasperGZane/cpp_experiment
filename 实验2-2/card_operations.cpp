// ============================================================
//  card_operations.cpp
//  校园一卡通消费管理系统 —— 功能函数实现文件
//  （模块二、三、四、五、六、九的具体实现）
//  编译标准：C++11
// ============================================================
#include "card_operations.h"

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <limits>
#include <new>
#include <sstream>

// MSVC 默认按系统代码页（中文 Windows 为 GBK）解释窄字符串字面量，而本文件
// 采用 UTF-8 编码，这里强制字面量也按 UTF-8 编码，避免中文提示显示为乱码；
// 其他编译器（GCC / Clang）会忽略该指示。
#if defined(_MSC_VER)
#  pragma execution_character_set("utf-8")
#endif

// ------------------------------------------------------------
//  匿名命名空间：只在本文件中使用的辅助函数（内部链接，外部不可见）
// ------------------------------------------------------------
namespace {

// 计算 UTF-8 字符串在控制台上的显示宽度。
// std::setw 按字节数计算宽度，而一个汉字在 UTF-8 中占 3 字节、显示为 2 列，
// 直接使用 setw 会使“李明 / LiMing”混排时列错位，因此这里自行统计显示宽度。
int displayWidth(const std::string &text) {
    int width = 0;
    for (std::size_t i = 0; i < text.size(); ) {
        const unsigned char c = static_cast<unsigned char>(text[i]);
        int bytes = 1;                 // 该字符在 UTF-8 中的字节数
        unsigned int code = c;         // 该字符的 Unicode 码点
        if (c >= 0xF0)      { bytes = 4; code = c & 0x07u; }
        else if (c >= 0xE0) { bytes = 3; code = c & 0x0Fu; }
        else if (c >= 0xC0) { bytes = 2; code = c & 0x1Fu; }
        for (int k = 1; k < bytes && i + k < text.size(); ++k)
            code = (code << 6) | (static_cast<unsigned char>(text[i + k]) & 0x3Fu);
        width += (code >= 0x2E80u) ? 2 : 1;    // 中日韩文字按 2 列计算
        i += bytes;
    }
    return width;
}

// 左对齐：不足 width 列时在右侧补空格
std::string padRight(const std::string &text, int width) {
    std::string result(text);
    for (int pad = width - displayWidth(text); pad > 0; --pad)
        result += ' ';
    return result;
}

// 右对齐：不足 width 列时在左侧补空格（数值列使用）
std::string padLeft(const std::string &text, int width) {
    std::string result;
    for (int pad = width - displayWidth(text); pad > 0; --pad)
        result += ' ';
    return result + text;
}

// 把金额格式化为“保留两位小数”的字符串
std::string money(double value) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << value;
    return out.str();
}

} // namespace

// ============================================================
//  模块二：命名空间与格式化输出模块
// ============================================================
namespace card {

void printLine(char c, int len) {
    if (len <= 0) len = 1;                       // 防止非法长度
    std::cout << std::string(static_cast<std::size_t>(len), c) << '\n';
}

void title(const std::string &s) {
    const int lineLen = 60;
    printLine('=', lineLen);

    // 标题居中：两侧补空格，使标题在 60 列宽度内居中显示
    int pad = (lineLen - displayWidth(s)) / 2;
    if (pad < 0) pad = 0;
    std::cout << std::string(static_cast<std::size_t>(pad), ' ') << s << '\n';

    printLine('=', lineLen);
}

// ============================================================
//  模块四：账户操作模块（含常引用与指向常量的指针）
// ============================================================

// 充值：按手续费率扣除手续费后增加余额，并累加交易次数。
// 形参 c 是引用，函数内修改的就是调用者的账户对象；
// 形参 fee 带默认参数 FEE_RATE（2%），调用时可省略。
void deposit(Card &c, double amount, double fee) {
    if (amount <= 0.0)
        throw InvalidInputException("充值金额必须大于 0");
    if (fee < 0.0 || fee >= 1.0)
        throw InvalidInputException("手续费率必须在 [0,1) 区间内");

    const double feeMoney = amount * fee;        // 手续费
    const double actual   = amount - feeMoney;   // 实际到账金额

    c.balance += actual;                         // 更新余额
    ++c.transactions;                            // 累加交易次数

    std::cout << "[充值] " << c.name << "(学号 " << c.id << ") 充值 "
              << money(amount) << " 元，手续费 " << money(feeMoney)
              << " 元，实际到账 " << money(actual) << " 元，余额 "
              << money(c.balance) << " 元\n";
}

// 消费：余额足够则扣款、累加交易次数并返回 true，否则返回 false。
// 金额为负属于非法参数，直接抛出异常，防止非法扣款。
bool spend(Card &c, double cost) {
    if (cost <= 0.0)
        throw InvalidInputException("消费金额必须大于 0");

    if (cost > c.balance)
        return false;                            // 余额不足：由调用者决定如何处理

    c.balance -= cost;
    ++c.transactions;
    return true;
}

// 消费（重载版本）：先输出消费项目，再调用上一个版本完成扣款。
bool spend(Card &c, double cost, const std::string &item) {
    std::cout << "[消费] 项目：" << item << "，金额 " << money(cost) << " 元，";

    const bool ok = spend(c, cost);              // 复用两参数版本，避免重复代码

    if (ok)
        std::cout << "成功，余额 " << money(c.balance) << " 元\n";
    else
        std::cout << "失败：余额不足（当前余额 " << money(c.balance) << " 元）\n";

    return ok;
}

// 常引用查询：const Card& 表示函数只能读取、不能修改该账户，
// 既避免了拷贝整个结构体的开销，又保证了数据安全。
void showBalance(const Card &c) {
    std::cout << "[查询] 学号 " << c.id << " 的当前余额：" << money(c.balance) << " 元";
    if (c.balance < WARNING_BALANCE)
        std::cout << "  << 警告：余额低于警戒值 " << money(WARNING_BALANCE) << " 元";
    std::cout << '\n';
}

// 指向常量的指针查询：const Card* 表示不能通过该指针修改所指向的账户。
void viewStudentID(const Card *p) {
    if (p == NULL) {                             // 空指针保护
        std::cout << "[查询] 账户指针为空，无法显示 ID\n";
        return;
    }
    std::cout << "[查询] 账户 ID：" << p->id << '\n';
}

} // namespace card

// ============================================================
//  模块三：动态内存管理与账户录入模块
// ============================================================

// 用 new[] 动态创建 n 个账户；内存申请失败时把 bad_alloc 转换为系统自定义异常
Card *createCards(int n) {
    if (n <= 0)
        throw InvalidInputException("账户数量必须为正整数");

    try {
        return new Card[static_cast<std::size_t>(n)];   // 每个元素由默认构造函数初始化
    } catch (const std::bad_alloc &) {
        throw CardException("内存分配失败：无法为 " + std::to_string(n) + " 个账户申请内存");
    }
}

// 用 delete[] 释放 new[] 申请的数组（delete[] 可以安全地接收空指针）
void destroyCards(Card *p) {
    delete[] p;
}

// 循环读入 n 个账户：读入后先做合法性校验，非法则抛出异常并提示重新输入。
void inputCards(Card *arr, int n) {
    if (arr == NULL || n <= 0)
        throw InvalidInputException("账户数组为空或数量非法，无法录入");

    for (int i = 0; i < n; ++i) {
        std::cout << "请输入第 " << (i + 1) << " 个账户（学号 姓名 初始余额）：";

        for (;;) {                               // 校验失败时重试，直到本条数据合法
            try {
                Card temp;                       // 先读入临时对象，校验通过后再写入数组
                if (!(std::cin >> temp.id >> temp.name >> temp.balance))
                    throw InvalidInputException("输入格式错误（格式：学号 姓名 初始余额）");
                if (temp.id <= 0)
                    throw InvalidInputException("学号必须为正整数");
                if (temp.name.empty())
                    throw InvalidInputException("姓名不能为空");
                if (temp.balance < 0.0)
                    throw InvalidInputException("初始余额不能为负数");

                temp.transactions = 0;           // 新账户的交易次数初始化为 0
                arr[i] = temp;
                break;
            } catch (const CardException &e) {
                // 输入流已结束：无法继续重试，抛出异常交由主程序统一处理，
                // 同时保证已申请的动态内存会被释放（不会死循环）。
                if (std::cin.eof())
                    throw CardException("输入流已结束，账户录入失败");

                std::cout << "[错误] " << e.what() << "，请重新输入：";
                std::cin.clear();                // 清除输入流的错误状态
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
}

// ============================================================
//  模块五：强制类型转换与余额校验模块
// ============================================================

// 元 -> 分：先乘以 100 并四舍五入，再用 static_cast<int> 去掉小数部分。
// static_cast<int> 是“显式”的类型转换，相比 C 风格的 (int) 更安全、更易读。
int toCent(double amount) {
    const double rounded = (amount >= 0.0) ? amount * 100.0 + 0.5
                                           : amount * 100.0 - 0.5;
    return static_cast<int>(rounded);
}

// ============================================================
//  模块六：报表输出模块
// ============================================================

// 以表格形式输出所有账户，余额保留两位小数，各列对齐
void printReport(const Card *arr, int n) {
    if (arr == NULL || n <= 0) {
        std::cout << "没有可输出的账户数据。\n";
        return;
    }

    const int wId    = 12;      // 学号列宽
    const int wName  = 20;      // 姓名列宽
    const int wBal   = 12;      // 余额列宽
    const int wTran  = 14;      // 交易次数列宽

    card::printLine('=');
    std::cout << "账户报表（余额单位：元）\n";
    card::printLine('-');

    // 表头：ID、Name、Balance、Transactions
    std::cout << padRight("ID", wId)
              << padRight("Name", wName)
              << padLeft("Balance", wBal)
              << padLeft("Transactions", wTran) << '\n';
    card::printLine('-');

    double total    = 0.0;      // 余额合计
    int    lowCount = 0;        // 低余额账户数

    for (int i = 0; i < n; ++i) {
        std::cout << padRight(std::to_string(arr[i].id), wId)
                  << padRight(arr[i].name, wName)
                  << padLeft(money(arr[i].balance), wBal)
                  << padLeft(std::to_string(arr[i].transactions), wTran) << '\n';

        total += arr[i].balance;
        if (arr[i].balance < WARNING_BALANCE)
            ++lowCount;
    }

    card::printLine('-');
    std::cout << "账户总数：" << n
              << "    余额合计：" << money(total) << " 元"
              << "    平均余额：" << money(total / static_cast<double>(n)) << " 元\n";

    // 使用 WARNING_BALANCE 做低余额提醒
    if (lowCount > 0) {
        std::cout << "低余额提醒（低于警戒值 " << money(WARNING_BALANCE) << " 元）：";
        bool first = true;
        for (int i = 0; i < n; ++i) {
            if (arr[i].balance < WARNING_BALANCE) {
                if (!first) std::cout << "，";
                std::cout << arr[i].name << "(" << money(arr[i].balance) << ")";
                first = false;
            }
        }
        std::cout << '\n';
    }

    card::printLine('=');
}
