CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
# CMAKE_MINIMUM_REQUIRED(VERSION 3.2) 必须放在第一行 是cmake所需要的最低版本

# 项目名称
project(sylar)

#[[
    cmake学习：https://blog.csdn.net/DY_1024/article/details/82940653
    CMake是一个跨平台的安装(编译)工具
    CMake的所有的语句都写在一个叫:CMakeLists.txt的文件中。
    当CMakeLists.txt文件确定后,可以用ccmake命令对相关的变量值进行配置。
    这个命令必须指向CMakeLists.txt所在的目录。
    配置完成之后,应用cmake命令生成相应的makefile（在Unix like系统下）或者 project文件（指定用window下的相应编程工具编译时）
]]



# 要显示执行构建过程中详细的信息(比如为了得到更详细的出错信息)
set(CMAKE_VERBOSE_MAKEFILE ON)

#[[
    参考：https://www.cnblogs.com/fengbeihong/p/3641384.html
    设置编译参数
    -rdynamic:通知链接器将所有的符号添加到动态符号表
    -O3：GCC编译优化级别
    -ggdb：加入调试信息
    -std=c++11：c++编译版本
    -Wall：严格编译
    -Wno-deprecated:使用过时的特性时候不要给出警告
    -Werror: 要求gcc把所有的警告当成错误处理
    -Wno-unused-function：姜戈声明但是没定义的static函数

]]
#set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -ggdb -std=c++11 -Wno-deprecated -Werror -Wno-unused-function")

# set来设置变量 LIB_SRC相当于下面的源码文件
set(LIB_SRC
    sylar/log.cc
    )

# 生成库 SHARED：静态库 MODULE动态库 将${LIB_SRC}生成静态库sylar
add_library(sylar SHARED ${LIB_SRC})

# 指定生成目标 即可执行文件
add_executable(testLog "tests/test.cc")
# 添加依赖于该目标或者被该目标依赖的目标 test依赖于sylar静态库
add_dependencies(testLog sylar)

# 添加链接库
# target_link_libraries(demo, child) 指明生成的demo可执行文件需要连接一个叫做：Child的链接库
# 指明生成的test可执行文件需要连接一个叫做：sylar的链接库
target_link_libraries(testLog sylar)

# 指定生成文件的输出路径
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
