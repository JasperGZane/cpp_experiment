// ============================================================
//  实验任务一：C++ 的输入输出控制考察
//  题目：输入若干名学生的信息（姓名 + 语文/数学/英语三科成绩），
//        计算每名学生的平均成绩，并输出成绩信息表。
//
//  要点：
//    1. 第一行输入学生人数 n；
//    2. 之后每行输入 “姓名 语文 数学 英语”；
//    3. 每名学生输出姓名、三科成绩与平均分（保留两位小数），
//       平均分 >= 90 的学生在行末标注 "Excellent"；
//    4. 输出使用 <iomanip> 中的 setw/setprecision 控制格式，各列整齐对齐。
//
//  编译（MSVC）：cl /EHsc /utf-8 /W4 task1_student_scores.cpp
//  编译（GCC） ：g++ -std=c++11 -Wall -Wextra -o task1_student_scores task1_student_scores.cpp
// ============================================================
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstddef>

// MSVC 默认按系统代码页（中文 Windows 为 GBK）解释窄字符串字面量，而本文件
// 采用 UTF-8 编码，这里强制字面量也按 UTF-8 编码，避免中文表头显示为乱码；
// 其他编译器（GCC / Clang）会忽略该指示。
#if defined(_MSC_VER)
#  pragma execution_character_set("utf-8")
#endif

namespace {

// ------------------------------------------------------------
// 计算字符串在控制台上的“显示宽度”。
// std::setw 是按“字节数”计算宽度的，而一个汉字在 UTF-8 中占 3 个字节、
// 在屏幕上只占 2 列。若直接对中文姓名使用 setw，后面的列就会错位，
// 因此这里自己统计显示宽度，保证中英文姓名混排时表格依然对齐。
// ------------------------------------------------------------
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

// 左对齐输出：文本不足 width 列时在右侧补空格
void printLeft(const std::string &text, int width) {
    std::cout << text;
    for (int pad = width - displayWidth(text); pad > 0; --pad) std::cout << ' ';
}

// 右对齐输出：文本不足 width 列时在左侧补空格（数值列常用）
void printRight(const std::string &text, int width) {
    for (int pad = width - displayWidth(text); pad > 0; --pad) std::cout << ' ';
    std::cout << text;
}

// 把 double 转成“保留两位小数”的字符串，便于统一控制列宽
std::string fixed2(double value) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << value;
    return out.str();
}

// 学生信息：姓名 + 三门课成绩 + 平均分
struct Student {
    std::string name;
    double      chinese;
    double      math;
    double      english;
    double      average;
};

} // namespace

int main() {
    std::ios::sync_with_stdio(false);

    // ---------- 1. 读入学生人数 ----------
    int n = 0;
    if (!(std::cin >> n) || n <= 0) {
        std::cerr << "输入错误：学生人数必须是大于 0 的整数。\n";
        return 1;
    }

    // ---------- 2. 动态申请数组存放 n 名学生 ----------
    Student *students = new Student[static_cast<std::size_t>(n)];

    for (int i = 0; i < n; ++i) {
        if (!(std::cin >> students[i].name >> students[i].chinese
                       >> students[i].math    >> students[i].english)) {
            std::cerr << "输入错误：第 " << (i + 1) << " 名学生的数据不完整。\n";
            delete[] students;                 // 出错也要释放动态内存
            return 1;
        }
        // ---------- 3. 计算平均成绩 ----------
        students[i].average =
            (students[i].chinese + students[i].math + students[i].english) / 3.0;
    }

    // ---------- 4. 输出成绩信息表 ----------
    const int wName = 12;      // 姓名列宽（左对齐）
    const int wScore = 9;      // 三科成绩与平均分列宽（右对齐）

    std::cout << std::fixed << std::setprecision(2);   // 之后所有浮点数均保留两位小数

    printLeft("姓名", wName);
    printRight("语文", wScore);
    printRight("数学", wScore);
    printRight("英语", wScore);
    printRight("平均分", wScore);
    std::cout << '\n' << std::string(wName + wScore * 4, '-') << '\n';

    for (int i = 0; i < n; ++i) {
        printLeft(students[i].name, wName);
        printRight(fixed2(students[i].chinese), wScore);
        printRight(fixed2(students[i].math),    wScore);
        printRight(fixed2(students[i].english), wScore);
        printRight(fixed2(students[i].average), wScore);
        if (students[i].average >= 90.0)       // 平均分 >= 90 标注 Excellent
            std::cout << "  Excellent";
        std::cout << '\n';
    }

    std::cout << std::string(wName + wScore * 4, '-') << '\n';

    delete[] students;      // 释放由 new[] 申请的动态数组
    return 0;
}
