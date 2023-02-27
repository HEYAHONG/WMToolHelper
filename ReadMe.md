# 说明

W600烧写辅助工具。调用W600官方工具wm_tool完成烧录。

## 源代码下载

由于本源代码包含第三方源代码,故直接下载可能有部分源代码缺失，需要通过以下方法解决:

- 在进行git clone 使用--recurse-submodules参数。

- 若已通过git clone下载,则在源代码目录中执行以下命令下载子模块:

  ```bash
   git submodule update --init --recursive
  ```

# 注意

非root用户在Linux下使用需要注意串口权限问题，如在ubuntu22.04可使用以下命令解决(修改后重新登陆后生效):

```bash
sudo adduser `id -nu` dialout
```

# 编译

本工具主要支持GCC编译（MSVC编译可能会出现问题）。在Windows下可使用MSYS2的Mingw32或Mingw64环境编译。

## 依赖库

- [wxWidgets](http://wxwidgets.org/)
- [pcre](http://www.pcre.org)



# 截图

![screenshot](doc/screenshot.png)
