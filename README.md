# NUemWin

## 1、介绍

N9H 系列免费为其用户提供经 SEGGER 正式授权，专为嵌入式系统打造的 GUI 软件链接库 ( emWin ) 和相关开发工具，让产品开发人员可以快速且容易地，开发出流畅、有质感的用户人机显示接口 ( Human Machine Interface, HMI ) ，为满足客户针对显示应用弹性设计的需求，N9H 系列内建 24 位 TFT RGB 接口、2D 图形加速器以及电阻式触控屏幕。

本源码实现了在 RT-Thread 上的移植。源码地址： <https://github.com/wosayttn/NUemWin>

### 1.1 目录结构

| 名称 | 说明 |
| ---- | ---- |
| Config | |
| Docs  | 文档目录 |
| Examples | 例子目录，并有相应的一些说明 |
| Include  | 头文件目录 |
| Lib  | 庫目录 |
| Tool  | UI 製作工具 |

### 1.2 许可证

NUemWin package 遵循 LGPLv2.1 许可，详见 `LICENSE` 文件。

### 1.3 依赖

- RT-Thread 4.0.3+


## 2、如何打开 NUemWin

使用 NUemWin package 需要在 RT-Thread 的包管理器中选择它，具体路径如下：

```
RT-Thread online packages
    multimedia packages --->
        [*] NUemWin: a NUemWin package for rt-thread
```

然后让 RT-Thread 的包管理器自动更新，或者使用 `pkgs --update` 命令更新包到 BSP 中。

## 3、使用 NUemWin

在打开 NUemWin package 后，当进行 bsp 编译时，它会被加入到 bsp 工程中进行编译。

具体 API 参照官方手册。

## 4、注意事项

無

## 5、联系方式

無
