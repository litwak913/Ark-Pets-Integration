# ArkPets GNOME 集成扩展

此扩展为 ArkPets 提供了一些查询，控制窗口的 D-Bus 方法。

修改自 [window-calls](https://github.com/ickyicky/window-calls)。

# 安装
## 自动安装（推荐）
ArkPets 启动器会在第一次启动桌宠前自动进行安装过程，根据提示安装即可。
## 从 Zip 安装
从 Releases 下载 zip 压缩包后，运行以下命令：
```bash
gnome-extensions install --force arkpets-integration@harryh.cn.zip
```
然后注销并重新登录。

## 从源码安装
请先确保您安装好以下工具：

* [Node.js](https://nodejs.org/)
* [pnpm](https://pnpm.io/)

克隆本仓库，然后运行 `make install`。

```bash
make install # 将会安装需要的依赖，编译 TypeScript 代码，并复制文件。
```

然后注销并重新登录。

# 启用
ArkPets 启动器会在启动桌宠前自动启用集成扩展。

您也可以通过`扩展`应用程序或运行以下命令来启用集成扩展：

```bash
gnome-extensions enable arkpets-integration@harryh.cn
```