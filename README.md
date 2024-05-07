# 火车票管理系统

SJTU CS1951 课程大作业

## 概况

### 作业安排

本作业分为两个部分。

在第一部分中，需要实现一个基于文件的 B+ 树。

在第二部分中，需要实现一个火车票管理系统。此部分要求使用 Git 开发，维持良好的项目管理习惯。此部分的中期检查等检查方式均会通过查看登记的 Git 仓库链接，因此如果想更换仓库的链接请及时联系助教。

### 作业周期

**注意：管理系统的截止日期 (2024-06-02 第 15 周周日) 为作业的硬性截止日期，除非有极其特殊的原因，否则不接受任何在截止后的提交，所有代码均以截止前的提交为准！**

- B+ 树: 2024-04-14（第 8 周周日）~ 2024-05-12（第 12 周周日）
- 管理系统: 2024-05-12（第 12 周周日）~ 2024-06-02（第 15 周周日）

## 评分标准

本作业占本课程总成绩 15%，其中 B+ 树占 7%，管理系统占 8%。

- B+ 树: 7%
  - OJ 测试: 80%
  - Code Review: 20%
- 管理系统: 8%
  - 正确性测试: 50%
    - 在正确性测试中，每一个测试点都有一个相对宽松的时间和磁盘使用限制，以仅检验程序的正确性和鲁棒性，只要通过测试即可得到满分。因此请不要尝试针对特定情况进行有损正确性和鲁棒性的优化。
  - 压力测试 - 30%
    - 在压力测试中，每一个测试点会有两档时间和磁盘限制，通过所有 Easy 的测试可以得到 (60% * 30% =) 18% 的分数，通过所有 Hard 测试可以得到另外 (40% * 30% =)  12% 的分数。
  - Code Review: 20%

bonus 另外计算，计入平时分总分，且不超过总分的 1%。

## B+ 树 - 7%

### 作业要求

作业要求实现基于 BPT 的外存管理系统。在本作业中，只允许调用以下头文件中的函数和类：

iostream, string, cstdio, cmath, string, fstream, filesystem

不允许使用这些头文件包含的 STL 容器 (如 `std::vector`) 或算法 (如 `std::sort`)。唯一的例外是，你可以使用 `std::string`。如果需要用到其他与算法、数据结构无关的标准库，请向助教提出请求。

你需要在最后通过 [OJ 测试](https://acm.sjtu.edu.cn/OnlineJudge/problem/2186)。

注意：建议使用类模板以方便后续完成管理系统。

## 管理系统 - 8%

敬请期待发布。

## Bonus

见 [Bonus 文档](bonus.md)。

准备自行设计并实现其他 bonus 的同学可以联系助教协商。

## 扣分

请保证自己项目结构的可读性，可以包括优化项目结构、完善 README 的内容、适当的文件树指南等，晦涩难懂的项目可能会加大助教的工作量，也可能会影响你的成绩（B+ 树阶段此条可忽略）。

**如有出现任何抄袭现象按 0 分计，并按照违反学术诚信的操作办法处理。**
