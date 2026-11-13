你是否遇到过，拍摄了一张中国动车组的车厢的照片，却难以从车厢号推测车型号的情况？

ChinaMUDecoder（China Multiple Unit Decoder，中国动车组车号查询）是一款基于C++/Raylib开发的**动车组车号查询软件**。  
软件可以通过**中国铁路动车组四位车号或六位车厢号**反推回**中国铁路动车组型号**。  

Tip:请尽量不要将软件放在中文路径下 ~~（别怪我身上，这是Raylib导致的）~~。

使用与功能
---
此为**主菜单**  
![主菜单样式](Picture/readme//main_menu.jpg)  
此为**查询界面**  
![查询界面样式](Picture/readme//decode_menu.jpg)  
图片不保证最新，仅供参考！  

您可以在任意界面编辑您输入的车号。  
Tips:  
1.对于文字过长导致无法在窗口内显示的情况，您可以横向拉长窗口以显示；  
2.当输入不满4位时按“回车”，软件会自动在前面补0至四位；  
3.按F11可以全屏。

作者与信息
---
作者：一个飞友_cpp（aviafan-cpp）
[Bilibili](https://space.bilibili.com/3546620213857006)
[Github](https://github.com/aviafan-cpp) 
[Moegirlpedia](https://zh.moegirl.org.cn/User:AviaFanCpp)  
字体：LXGW
[Github](https://github.com/lxgw/LxgwWenKai)  
图形库：Raylib
[官网](https://raylib.com)
[Github](https://github.com/raysan5/raylib)

平台：Windows x64  
C++版本：C++20  
Raylib版本：5.5  
MinGW版本：15.1

网页版
---
[项目网址](https://github.com/aviafan-cpp/ChinaMUDecoderWeb)  
[Page直通](https://aviafan-cpp.github.io/ChinaMUDecoderWeb/)

数据库与编写
---
数据来源：[ChinaEMU](https://china-emu.cn)  

数据库格式：
```text
型号名 速度(km/h)(无符号整数) 车厢数(无符号整数) 命名 起始车号(无符号整数) 终止车号(无符号整数) 科普网址链接  
```
"-"命令（在某一区间删除子区间）：
```text
-型号名 起始车号(无符号整数) 终止车号(无符号整数)
```
请注意：本文中“无符号整数”的范围在 0~2147483647 之间，而非 0~4294967295 。  

新版数据库详见[文档](docs/dev/train_data_ex/train_data_ex_format.md)

开发自己的版本？
---
请容许我向像您这样的开发者致敬，但恕我直言，这个软件的代码有亿些难懂。  
文件作用：  
ChinaMUDecoder.cpp 主文件  
raylib_obj_ex.cpp/.hpp 为Raylib提供的扩展 ~~（你为什么不用RayGUI……）~~  
sys_api.cpp/.hpp 系统API（目前只写了Windows）  
train_data.cpp/.hpp 数据库
train_data_ex.cpp/.hpp 数据库扩展

如果您想将则软件打包进您的软件中，请遵守协议。  
本软件支持传入一个4位数的车号参数或6位数的车厢号。

再次重申，请尽量不要将软件放在中文路径下 ~~（别怪我身上，这可能是Raylib导致的）~~。

移植？
---
事实上我们已经拥有了[网页版](#网页版)，所以对移植的需求不大。

如果您想要移植到Linux/MacOS上，请为sys_api.cpp编写对应的实现；  
如果您想要移植到安卓/iOS上，请在完成上一条提到内容的基础上给raylib_obj_ex.cpp的按钮逻辑进行修改以适配多点触控 ~~（或者不加）~~，并且最好增加一个屏幕键盘。