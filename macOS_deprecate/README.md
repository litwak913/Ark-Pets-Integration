# ArkPets macOS 集成库

此库为 ArkPets 提供了一些与 Objective-C 交互的方法。

# 安装
## 自动安装（推荐）
ArkPets 启动器会在第一次启动桌宠前自动进行安装过程，根据提示安装即可。

## 从 Zip 安装
从 Releases 下载 zip 压缩包后，运行以下命令：
```bash
cp libgdglow.dylib ~/Library/Application\ Support/ArkPets/
```

## 从源码安装
请先确保您已经安装好 Xcode Command Line Tools。

克隆本仓库，然后运行 `make install` 进行编译安装。

