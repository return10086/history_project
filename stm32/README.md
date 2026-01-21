# stm32

STM32/Keil（MDK-ARM）与 CubeMX 相关工程，含 HAL 库源码与示例。

## 工具链
- Keil MDK-ARM 5.x（ARMCC/ARMClang）；
- 对应芯片的 Device Family Pack（DFP）；
- 如从 CubeMX 生成，请确保安装相应 HAL 库与中间件版本。

## 打开与构建
- 使用 Keil 打开 `.uvprojx` 工程文件；
- 连接调试器（如 ST-LINK），选择目标器件后编译下载；
- 若路径为中文导致编译/调试异常，建议将工程迁移到英文路径重试。

## 目录提示
- `Drivers/`、`Middlewares/`：HAL/LL 与第三方中间件；
- `MDK-ARM/`：Keil 工程与配置；
- `Src/`、`Inc/` 或 `UserCode/`：用户代码区。

## 版本控制建议
- 不提交大型中间产物与编译输出，必要文件已由根目录 `.gitignore` 过滤；
- 若需分享生成的固件（`.hex`/`.bin`），建议单独打包发布或使用 Release。