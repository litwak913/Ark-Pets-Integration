# ArkPets KDE 集成插件

此特效插件为 ArkPets 提供了一些查询，控制窗口的 D-Bus 方法。

目前仅在 KDE Plasma 6.1.4 上通过测试。

# 安装
## 自动安装（推荐）
ArkPets 启动器会在第一次启动桌宠前自动进行安装过程，根据提示安装即可。
## 从 Zip 安装
从 Releases 下载 zip 压缩包后，运行以下命令：
```bash
sudo mkdir -p /usr/lib/qt6/plugins/kwin/effects/plugins
sudo cp ArkPetsIntegration.so /usr/lib/qt6/plugins/kwin/effects/plugins
```
然后注销并重新登录。

## 从源码安装
请先确保您安装好以下工具：

* GCC
* extra-cmake-modules
* kwin 及其头文件
* cmake

克隆本仓库，然后运行以下命令：

```bash
cmake -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build
sudo cmake --install build
```

然后注销并重新登录。

# 启用
ArkPets 启动器会在启动桌宠前自动启用插件。

您也可以运行以下命令来启用插件：

```bash
qdbus --literal org.kde.KWin /Plugins org.kde.KWin.Plugins.LoadPlugin ArkPetsIntegration
```
