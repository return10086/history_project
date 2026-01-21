# 数据结构课程作业

C/C/C++ 作业与练习，含若干题目与输入样例（如 `test.txt`）。

## 构建与运行
- C：
  ```bash
  gcc main.c -O2 -std=c11 -o app
  ./app < test.txt
  ```
- C++：
  ```bash
  g++ main.cpp -O2 -std=c++17 -o app
  ./app < test.txt
  ```
- Windows 上可用 MSVC（开发者命令行）：
  ```bat
  cl /O2 /std:c11 main.c
  app.exe < test.txt
  ```

## 备注
- 部分题目路径/数据文件名为中文，个别工具可能兼容性较差；必要时可将路径改为英文后编译运行。
- 若同一目录含多个题目，请进入对应子目录分别编译运行。