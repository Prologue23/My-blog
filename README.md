# RM

RM 是机器人控制与学习资料的集合仓库，包含上下位机 STM32 工程、Python 蓝牙串口控制端，以及 C/C++ 和 OpenCV 练习代码。

## 仓库结构

| 目录 | 内容 | 主要技术 |
| --- | --- | --- |
| `STM32_RM/` | 主控制程序，目标芯片 `STM32F407IGHx` | STM32CubeMX、HAL、CMake、Ninja、GNU Arm GCC |
| `Lower_Machine_bak/` | 下位机控制程序备份，目标芯片 `STM32F103C8Tx` | STM32CubeMX、HAL、CMake、Ninja、GNU Arm GCC |
| `Python/` | 蓝牙串口键盘控制脚本与 Python 练习 | Python、keyboard、pyserial |
| `C/` | 基础 C 练习 | GCC/MinGW-w64 |
| `C++/` | C++ 练习与 OpenCV 学习工程 | C++11、CMake、OpenCV 4.x |
| `README.md` | 环境、构建、烧录和运行说明 | Markdown |

三个原有子工程的历史已经合并到当前仓库的对应目录中。克隆后不需要初始化子模块。

## 快速开始

### 1. 获取源码

```powershell
git clone https://github.com/Prologue23/RM.git
cd RM
```

### 2. 安装通用工具

推荐在 Windows 10/11 上使用 PowerShell 7 或 Windows PowerShell 5.1。

| 工具 | 建议版本 | 用途 | 下载 |
| --- | --- | --- | --- |
| Git | 2.40 或更高 | 克隆和版本管理 | https://git-scm.com/download/win |
| CMake | 3.22 或更高 | STM32 与 OpenCV 构建 | https://cmake.org/download/ |
| Ninja | 1.11 或更高 | STM32 CMake 生成器 | https://github.com/ninja-build/ninja/releases |
| GNU Arm Embedded Toolchain | 14.3.1 或兼容版本 | 编译 STM32F4/F1 | https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads |
| STM32CubeCLT | 当前版本 | Cube 工具链、Ninja、CMake、GDB 集成 | https://www.st.com/en/development-tools/stm32cubeclt.html |
| STM32CubeMX | 当前版本 | 查看和重新生成 `.ioc` 配置 | https://www.st.com/en/development-tools/stm32cubemx.html |
| STM32CubeProgrammer | 当前版本 | 通过 SWD/UART 烧录 | https://www.st.com/en/development-tools/stm32cubeprog.html |
| ST-LINK 驱动 | 当前版本 | ST-LINK 调试器驱动 | https://www.st.com/en/development-tools/stsw-link009.html |
| SEGGER J-Link | 当前版本 | J-Link 调试和烧录 | https://www.segger.com/downloads/jlink/ |
| J-Scope | 随 J-Link 提供 | 实时变量观察 | https://www.segger.com/downloads/jlink/#JScope |
| Visual Studio Code | 最新稳定版 | 推荐编辑器 | https://code.visualstudio.com/ |
| Python | 3.8 或更高 | 运行 Python 控制脚本 | https://www.python.org/downloads/ |
| MSYS2 + MinGW-w64 GCC | 当前版本 | 编译 C/C++ 练习 | https://www.msys2.org/ |
| OpenCV | 4.7.0 或兼容的 4.x | C++ 图像处理练习 | https://opencv.org/releases/ |

建议安装以下 VS Code 扩展：

- `ms-vscode.cmake-tools`
- `ms-vscode.cpptools`
- `marus25.cortex-debug`
- `STMicroelectronics.stm32-vscode-extension`

### 3. 配置 PATH

把以下目录加入系统或当前用户的 `Path`。实际安装位置可能不同，请以本机为准。

```text
D:\Program\cmake-4.1.1-windows-x86_64\cmake-4.1.1-windows-x86_64\bin
C:\Users\<用户名>\AppData\Local\stm32cube\bundles\ninja\1.13.2+st.1\bin
C:\Users\<用户名>\AppData\Local\stm32cube\bundles\gnu-tools-for-stm32\14.3.1+st.2\bin
C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin
C:\Program Files\SEGGER\JLink
C:\msys64\mingw64\bin
```

重新打开终端后检查：

```powershell
git --version
cmake --version
ninja --version
arm-none-eabi-gcc --version
python --version
gcc --version
g++ --version
```

使用 STM32CubeCLT 的 VS Code 集成时，再设置：

```powershell
$env:CUBE_BUNDLE_PATH = "$env:LOCALAPPDATA\stm32cube\bundles"
```

永久配置可通过“系统属性 -> 高级 -> 环境变量”完成。

## STM32 工程

### 硬件

| 工程 | MCU | 调试接口 | 说明 |
| --- | --- | --- | --- |
| `STM32_RM` | STM32F407IGHx | SWD | Cortex-M4，主控制器 |
| `Lower_Machine_bak` | STM32F103C8Tx | SWD | Cortex-M3，下位机控制器 |

两套工程都使用 CMake 和 Ninja，并通过 `.ioc` 文件保存 STM32CubeMX 配置。正常编译只需要安装 GNU Arm Embedded Toolchain、CMake 和 Ninja；只有修改外设配置时才需要 STM32CubeMX。

### 编译 STM32_RM

```powershell
cd STM32_RM
cmake --preset Debug
cmake --build --preset Debug
```

输出文件：

```text
STM32_RM/build/Debug/STM32_RM.elf
STM32_RM/build/Debug/STM32_RM.map
```

等价的手动配置命令：

```powershell
cmake -S . -B build/Debug -G Ninja `
  -DCMAKE_BUILD_TYPE=Debug `
  -DCMAKE_TOOLCHAIN_FILE=cmake/gcc-arm-none-eabi.cmake
cmake --build build/Debug -j 8
```

### 编译 Lower_Machine_bak

```powershell
cd Lower_Machine_bak
cmake --preset Debug
cmake --build --preset Debug
```

输出文件：

```text
Lower_Machine_bak/build/Debug/Lower_Machine.elf
Lower_Machine_bak/build/Debug/Lower_Machine.map
```

### 烧录

使用 STM32CubeProgrammer CLI 烧录 STM32_RM：

```powershell
STM32_Programmer_CLI.exe -c port=SWD -w STM32_RM\build\Debug\STM32_RM.elf -rst
```

使用 ST-LINK CLI 烧录 Lower_Machine：

```powershell
ST-LINK_CLI.exe -c SWD -P Lower_Machine_bak\build\Debug\Lower_Machine.elf -Rst
```

如果使用 J-Link，可在 VS Code 的 Cortex-Debug 配置中选择 `jlink`，或在 J-Flash 中加载相同的 ELF 文件。

### 调试和实时观察

- VS Code Cortex-Debug 可使用 `stlink` 或 `jlink` servertype。
- STM32_RM 的 `.vscode/settings.json` 示例目标为 `STM32F407VE`；实际板卡 MCU 为 `STM32F407IGHx`，调试前应按板卡和探针修改。
- 使用 J-Scope 时，先用 J-Link 连接目标板，再加载 ELF 符号并选择需要观察的变量。
- 串口日志的端口和波特率以代码及硬件接线为准；Python 控制端默认使用 `COM6`、`9600`。

## Python 环境

Python 控制端需要 `keyboard` 和 `pyserial`。仓库根目录的 `requirements.txt` 已固定通过测试的版本：

```text
keyboard==0.13.5
pyserial==3.5
```

创建隔离环境：

```powershell
cd <仓库根目录>
py -3 -m venv .venv
.\.venv\Scripts\Activate.ps1
python -m pip install --upgrade pip
python -m pip install -r requirements.txt
```

如果 PowerShell 禁止激活脚本，可在当前窗口临时执行：

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
```

运行蓝牙串口控制端：

```powershell
python .\Python\BlueTooth.py
```

运行前需要把 `Python/BlueTooth.py` 中的 `COM_PORT = "COM6"` 改成设备管理器显示的实际串口。`keyboard` 会监听全局按键；如果目标窗口没有收到按键，请以管理员身份打开终端后重试。

运行 MIT 测试：

```powershell
cd Python\mit6_100l_f22_ps2_code\1_ps2
python -m unittest test_ps2_student.py
```

## C 和 C++ 示例

### C 文件

安装 MSYS2 后，在 MSYS2 MINGW64 终端安装 GCC：

```bash
pacman -S --needed mingw-w64-x86_64-gcc
```

编译单个文件：

```powershell
cd C
gcc "Hello world.c" -o "Hello world.exe"
.\Hello world.exe
```

### OpenCV 工程

`C++/c++/c++/OpenCV_learning` 是 OpenCV 4.x 工程。`OpenCV_DIR` 必须指向包含 `OpenCVConfig.cmake` 的目录。OpenCV 与 C++ 编译器必须使用兼容的 ABI，不要在 MinGW GCC 工程中直接链接 MSVC 的 `.lib`。

如果使用 MSYS2 MinGW-w64，推荐安装对应包：

```bash
pacman -S --needed mingw-w64-x86_64-opencv
```

使用 MinGW 的 OpenCV 配置：

```powershell
$env:OpenCV_DIR = "C:\msys64\mingw64\lib\cmake\opencv4"
```

配置并构建：

```powershell
cmake -S "C++\c++\c++\OpenCV_learning" `
  -B "C++\c++\c++\OpenCV_learning\build" `
  -G Ninja `
  -DCMAKE_C_COMPILER=gcc `
  -DCMAKE_CXX_COMPILER=g++ `
  -DOpenCV_DIR="$env:OpenCV_DIR"
cmake --build "C++\c++\c++\OpenCV_learning\build" --config Debug
```

运行程序前，确保 MinGW 的 `bin` 目录在 `Path` 中：

```text
C:\msys64\mingw64\bin
```

如果使用 OpenCV 官方 Windows 预编译包和 Visual Studio，OpenCV 的 `vc16` 目录必须配合 MSVC 工具链，不能与 MinGW GCC 混用。

其他 C++ 单文件练习可在 VS Code 中使用仓库的 `C++/.vscode/tasks.json` 编译。默认编译器为 `g++`，调试器为 `gdb`。

## 新电脑检查清单

1. 安装 Git、CMake、Ninja 和 GNU Arm Embedded Toolchain。
2. 将各工具目录加入 `Path`，重新打开终端并执行上一节的版本检查。
3. 克隆仓库并确认 `STM32_RM/`、`Lower_Machine_bak/`、`Python/` 都完整存在。
4. 进入 `STM32_RM`，执行 `cmake --preset Debug` 和 `cmake --build --preset Debug`。
5. 对 `Lower_Machine_bak` 重复相同构建流程。
6. 创建 Python 虚拟环境并安装 `requirements.txt`。
7. 如需 OpenCV，安装 OpenCV 4.x，并正确设置 `OpenCV_DIR` 与运行库 `Path`。
8. 连接调试器和目标板后，再进行烧录、调试或 J-Scope 观察。

## 不纳入仓库的内容

以下内容由 `.gitignore` 排除，克隆后应在目标电脑重新生成或单独配置：

- `build/`、`CMakeFiles/`、`CMakeCache.txt`、Ninja 和 Make 生成文件
- `.elf`、`.hex`、`.bin`、`.map`、`.o`、`.obj`、`.exe`、`.dll`、`.lib`、`.pdb` 等编译产物
- `.venv/`、`venv/`、`__pycache__/` 和 Python 缓存
- `.idea/`、`.vs/`、编辑器临时文件、`.DS_Store`、`__MACOSX/`
- `.env`、证书、密钥、凭据文件和日志
- OpenCV 构建目录及本地机器路径

如果工程需要新的非敏感配置模板，应提交 `.env.example` 或带占位值的配置文件，而不是提交真实密钥。
