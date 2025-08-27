huyu.asm:任务一对应的两次弹窗的asm源文件
编译命令为ml /coff /Cp huyu.asm /link /subsystem:windows /section:.text,rwe
huyu.exe:任务一对应的两次弹窗的可执行文件

huyu2.exe:任务二对应的仅弹第二个窗口的可执行文件

setpath.bat:编译asm之前配置路径的脚本

win7virus_hy.asm：病毒对应的asm源文件
编译命令为ml /coff /Cp win7virus_hy.asm /link /subsystem:windows /section:.text,rwe
win7virus_hy.exe：病毒

win11virus_hy.cpp:自己扩展的C++实现的病毒
