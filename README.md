# 这个是异步日志系统
```bash
#生成静态库---lib
add_library(sync_logsystem STATIC
        pch.h
        Logger.h
        Logger.cpp
)
#生成动态库---dll
add_library(sync_logsystem SHARED
        pch.h
        Logger.h
        Logger.cpp
)

#导出 __declspec(dllexport)
#导入 __declspec(dllimport)
#如果不是C/C++调用,则需要extern "C"
```