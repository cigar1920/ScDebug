# ScDebug

ScDebug是基于LLVM/Clang 3.6.2实现的，所以工具需要安装LLVM3.6.2及其依赖工具，具体包括Python2.7+，cmake3.+，gcc6.5.0等。ScDebug工具源码组织结构如下图 7-1所示。src文件夹下为规范化和转换过程的源码，bin文件夹下为工具编译完成后对应的格式化和转换工具，log文件夹下存放包括转换和分块的相关log文件，scdebug.h和scdebugMpfr.h对应为EFA和MPFR两种高精度计算实现的不同方式的头文件，install_scdebug.sh为工具安装的脚本，包括生成MakeFile文件以及编译。

![img](http://www.softlang.cn/images/e/e7/Image-20210104192308418.png)



直接执行install_scdebug.sh进行编译安装，或执行cmake CMakeLists.txt 指令生成MakeFile文件，然后执行make命令，编译ScDebug，编译过程如下图 所示。

![img](http://www.softlang.cn/images/7/73/Image-20210104192326070.png)

 

