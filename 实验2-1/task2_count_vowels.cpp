// ============================================================
//  实验任务二：指向常量的指针特性考察
//  题目：编写 C++ 函数 int countVowels(const char* p)，
//        统计字符串中元音字母（a, e, i, o, u）的个数。
//
//  要点：
//    1. 形参类型是 const char*，即“指向常量的指针”：
//          - 通过 p 只能读取字符（*p 不可赋值），从而保证函数不会破坏调用者的字符串；
//          - 指针变量 p 本身不是常量，可以执行 ++p 向后移动；
//    2. 在主函数中输入一行字符串（可能含空格），调用函数并输出结果。
//
//  编译（MSVC）：cl /EHsc /utf-8 /W4 task2_count_vowels.cpp
//  编译（GCC） ：g++ -std=c++11 -Wall -Wextra -o task2_count_vowels task2_count_vowels.cpp
// ============================================================
#include <iostream>
#include <string>
#include <cctype>

// 统计字符串中元音字母的个数。
// 形参：p —— 指向常量字符的指针，函数内部不能通过 p 修改所指向的内容。
int countVowels(const char *p) {
    int count = 0;

    // 用指针遍历：p 指向当前字符，遇到结束符 '\0' 停止
    while (p != NULL && *p != '\0') {
        // 统一转成小写后比较，使 'A'/'E' 等大写元音也能被统计到；
        // tolower 的实参必须是 unsigned char 的取值范围，故先做强制类型转换。
        const char ch = static_cast<char>(
            std::tolower(static_cast<unsigned char>(*p)));

        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u')
            ++count;

        ++p;        // 指针本身可以移动（const 修饰的是 *p，而不是 p）
    }

    return count;
}

int main() {
    std::string line;

    // 读入一整行（保留空格），例如 "Hello World"
    if (!std::getline(std::cin, line)) {
        std::cerr << "输入错误：未读到字符串。\n";
        return 1;
    }

    // c_str() 返回 const char*，正好与函数形参的类型匹配
    std::cout << "Number of vowels: " << countVowels(line.c_str()) << '\n';

    return 0;
}
