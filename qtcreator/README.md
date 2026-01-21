# qtcreator

Qt 示例工程（Qt 5.15.2 MinGW 环境）。

## 打开与构建
- 使用 Qt Creator 打开 `.pro` 或 CMake 工程；
- 建议使用独立的构建目录（Out-of-Source Build），避免将中间产物写入源码目录；
- 选择合适的 Kit（如 Qt 5.15.2 MinGW 64-bit），配置完成后构建运行。

## 备注
- 仓库中可能包含历史 `build-*` 目录，仅作参考；建议在本地新建干净的 `build/` 目录重新构建。