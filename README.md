# RM

RM 是机器人控制项目的集合仓库，环境说明只覆盖三个原有 Git 工程：

| 目录 | 内容 | MCU 或运行环境 |
| --- | --- | --- |
| `STM32_RM/` | 主控制程序 | STM32F407IGHx，Cortex-M4 |
| `Lower_Machine_bak/` | 下位机控制程序 | STM32F103C8Tx，Cortex-M3 |
| `Python/` | 蓝牙串口键盘控制与 Python 练习 | Python 3.8 或更高 |

这三个工程原有的 Git 历史已经合并到当前仓库。克隆后不需要初始化子模块。

## 快速开始

```powershell
git clone https://github.com/Prologue23/RM.git
cd RM
```

推荐使用 Windows 10/11、VS Code、CMake Presets、Ninja 和 GNU Arm GCC。

## 需要的软件

| 软件 | 用途 | 建议版本 | 下载 |
| --- | --- | --- | --- |
| Git | 克隆和版本管理 | 2.40 或更高 | https://git-scm.com/download/win |
| VS Code | 编写、构建和调试代码 | 最新稳定版 | https://code.visualstudio.com/ |
| CMake | 生成 STM32 构建文件 | 3.22 或更高 | https://cmake.org/download/ |
| Ninja | CMake 的 STM32 构建后端 | 1.11 或更高 | https://github.com/ninja-build/ninja/releases |
| GNU Arm Embedded Toolchain | 编译 STM32F4/F1 | 14.3.1 或兼容版本 | https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads |
| STM32CubeCLT | Cube 工具链、CMake、Ninja、GDB 集成 | 当前版本 | https://www.st.com/en/development-tools/stm32cubeclt.html |
| STM32CubeMX | 查看和重新生成 `.ioc` 配置 | 当前版本 | https://www.st.com/en/development-tools/stm32cubemx.html |
| SEGGER J-Link Software | J-Link 调试、烧录、GDB Server、RTT | 9.74 或兼容版本 | https://www.segger.com/downloads/jlink/ |
| SEGGER J-Scope | 实时观察变量和波形 | 随 J-Link 提供 | https://www.segger.com/downloads/jlink/#JScope |
| STM32CubeProgrammer | 可选的 STM32 烧录工具 | 当前版本 | https://www.st.com/en/development-tools/stm32cubeprog.html |
| ST-LINK 驱动 | 可选的 ST-LINK 调试器支持 | 当前版本 | https://www.st.com/en/development-tools/stsw-link009.html |
| Python | 运行蓝牙串口控制端 | 3.8 或更高 | https://www.python.org/downloads/ |

如果已经安装 STM32CubeCLT，它内部通常也包含可用的 Ninja、GNU Arm GCC 和 J-Link GDB Server。也可以单独安装 SEGGER J-Link Software。

本机独立 J-Link 安装示例：

```text
D:\Program\JLink_V974
```

其中的常用程序：

```text
JLink.exe
JLinkGDBServerCL.exe
JFlash.exe
JScope.exe
JLinkRTTViewer.exe
```

## VS Code 插件

安装以下扩展：

| 扩展 | 扩展 ID | 用途 |
| --- | --- | --- |
| C/C++ | `ms-vscode.cpptools` | C/C++ 语法、跳转和 IntelliSense |
| CMake Tools | `ms-vscode.cmake-tools` | 配置和构建 CMake 工程 |
| Cortex-Debug | `marus25.cortex-debug` | 通过 J-Link 或 ST-LINK 调试 MCU |
| STM32 VS Code Extension | `STMicroelectronics.stm32-vscode-extension` | STM32Cube 工程识别、构建和调试 |
| Python | `ms-python.python` | Python 运行、调试和虚拟环境 |
| Pylance | `ms-python.vscode-pylance` | Python 类型提示和补全 |
| Serial Monitor | `ms-vscode.vscode-serial-monitor` | 查看串口输出 |

仓库根目录的 `.vscode/extensions.json` 已列出这些推荐扩展。打开仓库后，VS Code 会提示安装。

## 环境变量

把下面这些目录加入系统或当前用户的 `Path`。实际安装位置可能不同。

```text
D:\Program\JLink_V974
C:\Users\<用户名>\AppData\Local\stm32cube\bundles\ninja\1.13.2+st.1\bin
C:\Users\<用户名>\AppData\Local\stm32cube\bundles\gnu-tools-for-stm32\14.3.1+st.2\bin
C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin
D:\Program\cmake-4.1.1-windows-x86_64\cmake-4.1.1-windows-x86_64\bin
```

如果 VS Code 使用 STM32CubeCLT 工具，再设置：

```powershell
$env:CUBE_BUNDLE_PATH = "$env:LOCALAPPDATA\stm32cube\bundles"
```

永久配置可在“系统属性 -> 高级 -> 环境变量”中完成。配置完成后重新打开终端并检查：

```powershell
git --version
cmake --version
ninja --version
arm-none-eabi-gcc --version
JLink.exe -?
python --version
```

## STM32 编译

两个 STM32 工程都使用 `CMakePresets.json`、Ninja 和 `cmake/gcc-arm-none-eabi.cmake`。构建时只需要 CMake、Ninja 和 GNU Arm GCC。只有修改外设配置时才需要使用 STM32CubeMX 打开 `.ioc` 文件。

### 编译 STM32_RM

```powershell
cd STM32_RM
cmake --preset Debug
cmake --build --preset Debug
```

输出：

```text
STM32_RM/build/Debug/STM32_RM.elf
STM32_RM/build/Debug/STM32_RM.map
```

### 编译 Lower_Machine_bak

```powershell
cd Lower_Machine_bak
cmake --preset Debug
cmake --build --preset Debug
```

输出：

```text
Lower_Machine_bak/build/Debug/Lower_Machine.elf
Lower_Machine_bak/build/Debug/Lower_Machine.map
```

## J-Link 烧录

### 使用 J-Flash

1. 使用 J-Link 和 SWD 连接目标板。
2. 打开 `JFlash.exe`。
3. STM32_RM 选择设备 `STM32F407IG`。
4. Lower_Machine 选择设备 `STM32F103C8`。
5. 接口选择 `SWD`，速度可先设为 `4000 kHz`。
6. 加载对应的 `.elf` 文件并执行擦除、编程和复位。

### 使用 J-Link Commander

烧录 STM32_RM：

```powershell
JLink.exe -device STM32F407IG -if SWD -speed 4000 -autoconnect 1
```

进入 J-Link 命令界面后执行：

```text
loadfile STM32_RM\build\Debug\STM32_RM.elf
r
g
qc
```

烧录 Lower_Machine：

```powershell
JLink.exe -device STM32F103C8 -if SWD -speed 4000 -autoconnect 1
```

```text
loadfile Lower_Machine_bak\build\Debug\Lower_Machine.elf
r
g
qc
```

如果连接失败，先检查目标板供电、SWDIO、SWCLK、GND、复位脚以及 J-Link 驱动。

## VS Code J-Link 调试

仓库根目录的 `.vscode/launch.json` 提供了两个调试配置：

```text
STM32_RM (J-Link SWD)
Lower_Machine (J-Link SWD)
```

对应的构建任务位于 `.vscode/tasks.json`。按 F5 时会自动先执行 `cmake --preset Debug`，然后构建对应的 Debug ELF。

`STM32_RM` 的调试配置使用：

```json
{
  "servertype": "jlink",
  "device": "STM32F407IG",
  "interface": "swd",
  "speed": 4000,
  "executable": "${workspaceFolder}/STM32_RM/build/Debug/STM32_RM.elf"
}
```

`Lower_Machine` 的调试配置使用：

```json
{
  "servertype": "jlink",
  "device": "STM32F103C8",
  "interface": "swd",
  "speed": 4000,
  "executable": "${workspaceFolder}/Lower_Machine_bak/build/Debug/Lower_Machine.elf"
}
```

如果 Cortex-Debug 找不到 GDB Server，在配置中增加：

```json
"serverpath": "D:/Program/JLink_V974/JLinkGDBServerCL.exe"
```

同一时间只能有一个程序占用 J-Link。启动 VS Code 调试前，应关闭 J-Flash、J-Link Commander、J-Scope 和其他 GDB Server。

## J-Scope 实时变量观察

1. 使用 Debug 配置编译工程，确保生成的 `.elf` 包含调试符号。
2. 使用 J-Link 和 SWD 连接目标板。
3. 打开 `D:\Program\JLink_V974\JScope.exe`。
4. 选择目标设备和 SWD 接口，速度可先设为 `4000 kHz`。
5. 加载对应的 `.elf`，例如 `STM32_RM/build/Debug/STM32_RM.elf`。
6. 从符号列表中选择需要观察的变量并开始采样。

如果变量无法显示或数值不对，优先检查：

- 使用的是 Debug 版本，而不是 Release 版本
- 变量没有被编译器优化掉
- 需要持续观察的变量使用了 `volatile` 或保持为全局变量
- J-Link、目标板和 MCU 设备型号选择正确
- Cortex-Debug、J-Flash 等程序没有同时占用 J-Link

## Python 环境

Python 控制端需要 `keyboard` 和 `pyserial`。仓库根目录的 `requirements.txt` 固定了版本：

```text
keyboard==0.13.5
pyserial==3.5
```

创建虚拟环境：

```powershell
cd <仓库根目录>
py -3 -m venv .venv
.\.venv\Scripts\Activate.ps1
python -m pip install --upgrade pip
python -m pip install -r requirements.txt
```

如果 PowerShell 禁止激活脚本：

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
```

运行蓝牙串口控制端：

```powershell
python .\Python\BlueTooth.py
```

运行前需要把 `Python/BlueTooth.py` 中的 `COM_PORT = "COM6"` 改为设备管理器显示的实际串口。默认波特率为 `9600`。

`keyboard` 会监听全局按键。如果程序没有收到按键，可以尝试用管理员身份打开 PowerShell 或 VS Code 后重新运行。

## 新电脑检查清单

1. 安装 Git、VS Code、CMake、Ninja、GNU Arm GCC 和 Python。
2. 安装 SEGGER J-Link Software，确认 `JLink.exe`、`JLinkGDBServerCL.exe` 和 `JScope.exe` 可用。
3. 安装 VS Code 推荐扩展。
4. 把 J-Link、CMake、Ninja 和 GNU Arm GCC 目录加入 `Path`。
5. 克隆仓库并编译 `STM32_RM`、`Lower_Machine_bak`。
6. 连接 J-Link，先用 J-Flash 或 J-Link Commander 验证烧录。
7. 在 VS Code 中启动对应的 J-Link 调试配置。
8. 使用 J-Scope 加载 Debug ELF 并观察变量。
9. 创建 Python 虚拟环境，安装 `requirements.txt`，修改实际串口号后运行 `BlueTooth.py`。

## 不纳入仓库的内容

以下内容由 `.gitignore` 排除，需要在新电脑上重新生成或单独配置：

- `build/`、CMake、Ninja 和 Make 生成文件
- `.elf`、`.hex`、`.bin`、`.map`、`.o`、`.obj`、`.pdb` 等编译产物
- `.venv/`、`venv/`、`__pycache__/` 和 Python 缓存
- `.idea/`、`.vs/`、编辑器临时文件和操作系统缓存
- `.env`、证书、密钥、凭据文件和日志
- 本地 J-Link、工具链、STM32Cube 和 VS Code 的机器相关路径

如果后续需要配置模板，应提交带占位值的示例文件，不要提交真实密钥或本机私有配置。

## 调试排错

### J-Link 找不到设备

- 检查 J-Link 驱动和设备管理器。
- 检查 SWDIO、SWCLK、GND、VTref 和复位线。
- 降低 SWD 速度，例如从 `4000 kHz` 改为 `1000 kHz`。
- 确认 MCU 型号为 `STM32F407IG` 或 `STM32F103C8`。

### VS Code 无法启动调试

- 确认 Cortex-Debug 和 STM32 扩展已安装。
- 确认 `.vscode/tasks.json` 中的 CMake 任务执行成功。
- 确认 ELF 路径存在。
- 关闭其他 J-Link 程序。
- 必要时在 Cortex-Debug 配置中设置 `serverpath`。

### Python 无法运行

- 确认当前虚拟环境已激活。
- 执行 `python -m pip show keyboard pyserial` 检查依赖。
- 确认 `COM_PORT` 与设备管理器一致。
- 如果串口被其他程序占用，先关闭串口助手或其他监控工具。
