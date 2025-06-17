## 介绍

这是我的livevent的学习和练习项目，同时我也用它学习怎么写一些异步io的网络通讯程序

我主要看这个网页学习的

[https://libevent.org/libevent-book/](https://libevent.org/libevent-book/)

我用的环境是windows,vs2022, 每个项目都需要手动配置libevent的头文件, 还有链接库文件,

我用cmake-gui编译的libevent, 但需要用比较旧的版本, 要不然编译会出错,

编译libevent之后,可以把头文件单独拷贝到一个目录下, 方便每次引用,(如果跨设备这个配置目录大概会不一样, 需要重新配置)

然后libevent有静态和动态库, 两者都是需要的, **动态库需要直接拷贝到程序执行目录**

## 具体配置如下

头文件 :  
项目属性--配置属性--C/C++--常规--附加包含目录--E:\)\usr\include\event2(我存放头文件的目录)    

静态库 :    
   项目属性--配置属性--链接器--常规--附加库目录--E:\)\lib\libevent_2_1_12(我存放静态库的目录)

项目属性--配置属性--链接器--输入--附加依赖项 --
event.lib
event_core.lib
event_extra.lib
event_openssl.lib