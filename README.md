# history_project

个人学习与项目归档（数据结构、Web 前端练习、MATLAB、PyQt、Qt、STM32 等）。本仓库按目录分类保存历史代码与示例，便于复用与参考。

## 目录总览
- 数据结构课程作业：C/C/C++ 练习与作业代码
- 学做网页：HTML/CSS 学习与小练习
- matlab：MATLAB 脚本与小项目
- pyqtcreater：Python + PyQt 学习与示例
- qtcreator：Qt 项目与示例（Qt 5.15.2 MinGW）
- stm32：STM32/Keil（MDK-ARM）与 CubeMX 相关工程

## 快速开始
- 克隆：
  ```bash
  git clone https://github.com/return10086/history_project.git
  cd history_project
  ```
- 查看各目录 README/说明（如有），或直接打开对应 IDE：
  - 数据结构课程作业：VS Code + GCC/Clang，或任意兼容编译器
  - 学做网页：浏览器直接打开 HTML 文件预览
  - matlab：MATLAB R2020+ 打开 `.m` 文件运行
  - pyqtcreater：Python 3.9+，`pip install pyqt5` 后运行脚本
  - qtcreator：用 Qt Creator 打开对应 `.pro`/CMake 工程（环境：Qt 5.15.2 MinGW）
  - stm32：Keil MDK-ARM 打开 `.uvprojx`；或用 CubeMX 重新生成代码

## 子目录说明
- 数据结构课程作业
  - 内容：若干作业/练习（C/C++）。常见文件：`main.c`/`main.cpp`、输入数据 `*.txt`。
  - 构建：GCC/Clang/MSVC 均可；示例（GCC）：`g++ main.cpp -O2 -std=c++17 -o app`。

- 学做网页
  - 内容：HTML/CSS 基础练习与小案例。
  - 运行：双击 HTML 即可在浏览器预览；或使用 Live Server。

- matlab
  - 内容：`first_project.m`、`second_project.m`、`third_project.m` 等脚本。
  - 运行：在 MATLAB 中打开并执行脚本。

- pyqtcreater
  - 内容：基础 Python 语法与 PyQt 学习示例、串口 demo 等。
  - 依赖：Python 3.9+；`pip install pyqt5 pyserial`（如需）。

- qtcreator
  - 内容：Qt 示例工程（含 `build-*` 目录）。
  - 建议：优先在 IDE 中新建干净的 `build` 目录进行 out-of-source 构建。

- stm32
  - 内容：Keil MDK-ARM 工程、HAL 库源码、CubeMX 生成代码、示例驱动等。
  - 打开：Keil 打开 `.uvprojx`；需要相应芯片包（Device Family Pack）。
  - 提示：部分工程包含生成的 `build/Debug` 等文件，后续维护可考虑清理并忽略。

## 仓库规范
- 忽略与行尾
  - 采用根目录的 [.gitignore](.gitignore) 与 [.gitattributes](.gitattributes)：
    - 忽略常见构建产物、编辑器文件与临时文件
    - 统一文本类文件的行尾（LF/CRLF），并标记二进制资产
- 提交信息风格（建议）：
  - `feat(scope): ...` 新增功能/目录导入
  - `fix(scope): ...` 问题修复
  - `docs(scope): ...` 文档/说明
  - `chore(scope): ...` 其他杂项

## 备注
- 历史代码主要用于学习与归档，可能包含过时写法或临时代码；后续按需整理优化。
- 若某些工具对中文路径/文件名支持不佳，建议在本地迁移至英文路径后再构建。
