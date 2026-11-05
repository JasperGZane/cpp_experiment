# 实验 2-2：校园一卡通消费管理系统（C++ 综合项目实训）

本目录对应《第2-2章 实验任务-题目.md》，按模块七的要求拆分为**头文件 + 功能实现 + 主程序**三个源文件。

## 一、工程文件结构

```text
实验2-2/
├── card_operations.h       // 头文件：常量、Card 结构体、异常类型、函数声明（只声明，不定义）
├── card_operations.cpp     // 功能实现：模块二~模块六、模块九的具体函数体
├── CampusCardSystem.cpp    // 主程序：模块八的集成流程、模块九的异常捕获
├── CMakeLists.txt          // CMake 构建脚本（C++11）
├── build.bat               // Windows + Visual Studio 一键编译
└── input_example.txt       // 示例输入数据（配合输入重定向使用）
```

头文件用 `#ifndef / #define / #endif` 防止重复包含；三个文件的关系是：

```text
CampusCardSystem.cpp  ──#include──▶  card_operations.h  ◀──#include──  card_operations.cpp
        (调用接口)                    (声明，链接时匹配)                  (提供定义)
```

## 二、编译与运行

### 方式 1：命令行（已实测通过）

```bat
rem 在本目录执行，自动定位 Visual Studio 的 C++ 工具链
build.bat
build\CampusCardSystem.exe
```

### 方式 2：CMake

```bash
mkdir build && cd build
cmake ..                  # Visual Studio 下可加 -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### 方式 3：g++ / clang++

```bash
g++ -std=c++11 -Wall -Wextra -o CampusCardSystem CampusCardSystem.cpp card_operations.cpp
./CampusCardSystem
```

### 方式 4：Visual Studio 图形界面（对应模块一、模块十）

1. “创建新项目 → 控制台应用（C++）”，工程命名为“校园一卡通系统”；
2. 删除自动生成的 `*.cpp`，把本目录的 3 个文件添加进工程（“添加 → 现有项”）；
3. **必须**在“项目属性 → C/C++ → 命令行 → 附加选项”中填入 `/utf-8`
   （源文件是 UTF-8 编码，否则中文提示会乱码或编译报错；本工程已同时写入 UTF-8 BOM，
   并含 `#pragma execution_character_set("utf-8")`，加上 `/utf-8` 最稳妥）；
4. `F5` 调试运行；把输入重定向更方便：调试 → 命令参数不支持重定向时，可先“开始执行(不调试)”再手动输入。

## 三、模块实现对照表

| 模块 | 要求 | 实现位置 |
|---|---|---|
| 模块一 | 常量 `FEE_RATE = 0.02`、`WARNING_BALANCE = 10.0`，`Card` 结构体 | `card_operations.h` 顶部 |
| 模块二 | `namespace card` 中 `printLine(char c='-', int len=60)`、`title(const string&)` | `card_operations.cpp` → `namespace card` |
| 模块三 | `Card *createCards(int n)`、`void destroyCards(Card*)`、`void inputCards(Card*, int)` | `card_operations.cpp`（`new[]` / `delete[]`） |
| 模块四 | `deposit(..., double fee = FEE_RATE)`、`bool spend(Card&, double)`、重载 `spend(..., const string& item)`、`showBalance(const Card&)`、`viewStudentID(const Card*)` | `card_operations.cpp` → `namespace card` |
| 模块五 | `int toCent(double amount)`，用 `static_cast<int>` | `card_operations.cpp` |
| 模块六 | `void printReport(const Card*, int n)`，表头 ID / Name / Balance / Transactions | `card_operations.cpp` |
| 模块七 | 三文件结构、`#ifndef` 守卫、声明与定义分离 | `card_operations.h` / `.cpp` / `CampusCardSystem.cpp` |
| 模块八 | 主程序集成流程 | `CampusCardSystem.cpp` 的 `main()` |
| 模块九 | 输入校验、余额异常、内存申请异常 | 异常类在 `.h`，`try/throw/catch` 在 `.cpp` 与 `main()` |
| 模块十 | 调试快捷键实训 | 见本文第六节（需自行截图） |

## 四、运行结果（示例输入 → 实际输出）

输入数据（`input_example.txt`）：

```text
3
2025001 李明 100
2025002 王华 50
2025003 张伟 5
```

运行输出：

```text
============================================================
                   校园一卡通消费管理系统
============================================================
请输入学生数量：3
请输入第 1 个账户（学号 姓名 初始余额）：2025001 李明 100
请输入第 2 个账户（学号 姓名 初始余额）：2025002 王华 50
请输入第 3 个账户（学号 姓名 初始余额）：2025003 张伟 5
内存分配成功，可管理 3 个账户。
------------------------------------------------------------
模块四：账户充值（手续费率 2.00%）
[充值] 李明(学号 2025001) 充值 100.00 元，手续费 2.00 元，实际到账 98.00 元，余额 198.00 元
模块四：账户消费
[消费] 项目：食堂午餐，金额 8.50 元，成功，余额 189.50 元
模块四：账户信息查询
[查询] 学号 2025001 的当前余额：189.50 元
[查询] 账户 ID：2025001
模块五：金额单位换算
余额 189.50 元 = 18950 分
------------------------------------------------------------
模块九：异常处理演示
[异常已捕获] 充值金额必须大于 0
[消费] 项目：笔记本电脑，金额 1000000.00 元，失败：余额不足（当前余额 189.50 元）
[异常已捕获] 账户余额不足，无法完成该笔消费
------------------------------------------------------------
模块六：账户报表
============================================================
账户报表（余额单位：元）
------------------------------------------------------------
ID          Name                     Balance  Transactions
------------------------------------------------------------
2025001     李明                      189.50             2
2025002     王华                       50.00             0
2025003     张伟                        5.00             0
------------------------------------------------------------
账户总数：3    余额合计：244.50 元    平均余额：81.50 元
低余额提醒（低于警戒值 10.00 元）：张伟(5.00)
============================================================
动态账户数组已释放（delete[]）。
============================================================
程序正常结束。
```

计算过程：初始 100 → 充值 100 元扣 2% 手续费（2.00 元）→ 198.00 → 消费 8.50 → 189.50；
`toCent(189.50) = 18950` 分；该账户共 2 次交易（1 次充值 + 1 次消费）。

## 五、关键设计说明（实验报告可直接引用）

### 1. 常引用 `const Card&` 与指向常量的指针 `const Card*` 的区别

| | `showBalance(const Card &c)` | `viewStudentID(const Card *p)` |
|---|---|---|
| 语法本质 | 实参的**别名** + 只读 | 存放地址的**指针变量** + 指向的对象只读 |
| 是否拷贝对象 | 不拷贝，效率高 | 不拷贝，只传地址 |
| 能否修改账户 | 不能（`c.balance = ...` 编译报错） | 不能（`p->balance = ...` 编译报错） |
| 能否改变指向 | 引用绑定后不能改指 | 指针本身可以改指（`p = &other`） |
| 是否可为空 | 不可以，必须绑定到对象 | 可以为 `nullptr`，使用前需判空（`viewStudentID` 已判空） |

两者都实现了“只读访问、防止误改”，但引用更安全（不可能为空），指针更灵活（可以指向不同对象、可以为空）。

### 2. 动态内存管理

`createCards(n)` 用 `new Card[n]` 申请数组，`destroyCards(p)` 用 `delete[] p` 释放；
`Card` 提供了默认构造函数，保证 `new Card[n]` 得到的每个元素都有确定的初值。

主程序在**正常路径**和**异常路径**上都会调用 `destroyCards`，因此即使中途抛出异常也不会内存泄漏
（`delete[]` 可以安全地接收空指针，所以异常路径上重复调用不会出错）。

### 3. 异常处理机制

- 自定义异常层次：`CardException : std::runtime_error`，
  其下 `InvalidInputException`（输入非法）、`InsufficientBalanceException`（余额不足），
  主程序用 `catch` 按类型分别处理，最后再用 `catch (const std::exception&)` 兜底。
- **输入校验**：`inputCards` 读入后逐项校验，非法时抛出异常并在 `catch` 中提示重输，
  同时 `cin.clear()` + `cin.ignore(...)` 清理流状态；若输入流已到末尾（EOF）则抛出异常退出，
  避免“读失败 → 无限重试”的死循环。
- **消费校验**：`spend(Card&, double)` 遇到负数金额直接 `throw InvalidInputException`（防止非法扣款）；
  余额不足时按模块四的要求返回 `false`，由主程序根据返回值 `throw InsufficientBalanceException`，
  这样既保留了“函数返回 bool”的接口，又满足模块九“超支要抛异常”的要求。
- **内存申请异常**：`createCards` 捕获 `std::bad_alloc` 并转换为 `CardException`，
  主程序捕获后提示“内存分配失败”，随后释放内存并 `return 1` 安全退出。

### 4. 强制类型转换

```cpp
int toCent(double amount) {
    const double rounded = (amount >= 0.0) ? amount * 100.0 + 0.5
                                           : amount * 100.0 - 0.5;
    return static_cast<int>(rounded);   // 显式转换：去掉小数部分
}
```

先四舍五入再 `static_cast<int>`，避免 0.1 + 0.2 这类浮点误差导致 189.49 元被截成 18948 分。
`static_cast` 是编译期检查的显式转换，比 C 风格 `(int)` 更安全、更易检索。

### 5. 报表对齐

题目要求“余额保留两位小数且各列对齐”。`std::setw` 按**字节数**计算宽度，
而一个汉字在 UTF-8 中占 3 字节、屏幕上只占 2 列，直接用 `setw` 会让中文姓名后的列整体右移。
因此程序用 `displayWidth()` 统计显示宽度（码点 ≥ U+2E80 按 2 列计），再用 `padRight/padLeft` 手工补空格，
所以“李明”和“LiMing”混排、学号长短不一时表格依然对齐。

## 六、测试用例（已实测）

| 用例 | 输入 | 结果 |
|---|---|---|
| 正常流程 | 3 个账户（见上） | 报表、换算、异常演示均正确，退出码 0 |
| 人数非法 | `abc` 或 `0` | 提示 `学生数量必须是正整数`/`必须大于 0` 并重输，输入 `3` 后继续 |
| 学号非正 | `0 张三 100` | 提示 `学号必须为正整数` 并重输该账户 |
| 余额为负 | `2025001 李明 -5` | 提示 `初始余额不能为负数` 并重输该账户 |
| 数据不足 | 声明 5 个账户，只给 1 条 | 提示 `输入流已结束，账户录入失败`，释放内存后退出码 1（无死循环） |

## 七、模块十：Visual Studio 调试快捷键

| 操作功能 | 快捷键 | 建议练习方式 |
|---|---|---|
| 设置/取消断点 | **F9** | 在 `deposit()` 的 `c.balance += actual;` 行下断点，观察余额变化 |
| 单步执行（跳过函数） | **F10** | 在 `main()` 中逐行查看调用顺序 |
| 单步进入函数内部 | **F11** | 从 `card::deposit(...)` 调用处进入函数体 |
| 运行到光标行 | **Ctrl + F10** | 跳过录入循环，直接停在报表输出 |
| 一键编译并调试 | **F5** | 启动调试 |
| 注释 / 取消注释 | **Ctrl + K + C** / **Ctrl + K + U** | 批量注释 `spend` 的重载版本 |
| 代码格式化 | **Ctrl + K + F** | 整理缩进与花括号 |
| 查看定义 / 返回 | **F12** / **Ctrl + 减号** | 从 `#include "card_operations.h"` 中的声明跳到 `.cpp` 定义 |

调试时建议在“监视”窗口加入：`accounts[0].balance`、`accounts[0].transactions`、`n`，
并截图断点命中时的“局部变量/监视”窗口作为实验报告素材。

## 八、编码与可移植性说明

1. 源文件为 **UTF-8（带 BOM）**，并含 `#pragma execution_character_set("utf-8")`（MSVC 专用，其他编译器忽略），
   因此 MSVC 无论是否加 `/utf-8` 都能正确输出中文（已分别实测）。
2. Windows 控制台默认代码页是 GBK，`main()` 开头的 `enableUtf8Console()` 调用
   `SetConsoleOutputCP(CP_UTF8)` / `SetConsoleCP(CP_UTF8)` 把输入输出切到 UTF-8；
   Linux / macOS 终端本身就是 UTF-8，无需处理（该函数在非 Windows 平台为空实现）。
3. 包含 `windows.h` 前定义了 `NOMINMAX`，避免其 `max` 宏与 `std::numeric_limits<>::max()` 冲突。
4. 代码使用 C++11 标准（范围 `using` 未用、无 C++14/17 特性），MSVC 下以 `/std:c++14` 编译，
   MSVC 没有 `/std:c++11` 选项，默认语言级别即 C++14，向下兼容 C++11 代码。
