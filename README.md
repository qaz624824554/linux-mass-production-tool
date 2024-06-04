# Introduce

本项目是一个电子产品量产工具，用于产品出厂时进行功能检测，如果功能正常，对应的按钮会显示绿灯，如果出问题则显示红灯。

# Get Start

1.打开`Makefile`，修改`CROSS_COMPILE`为对应的编译器前缀，默认为`arm-linux-gnueabihf-`

2.需要提前安装好`tslib、freetype`库

3.执行`make`命令，将编译的`test`文件放到板子上运行即可。



# Result

![VID_20240604_162742.mp4](https://images-1256612942.cos.ap-guangzhou.myqcloud.com/undefinedVID_20240604_162742.mp4.gif)
