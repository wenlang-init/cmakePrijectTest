cmake_minimum_required(VERSION 3.16)

project(test VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#aux_source_directory(./ sources)
file(GLOB decode CONFIGURE_DEPENDS decode/*.c decode/*.h)
file(GLOB fft CONFIGURE_DEPENDS fft/*.c fft/*.h)
file(GLOB method CONFIGURE_DEPENDS method/*.c method/*.h method/*.hpp)
file(GLOB sql CONFIGURE_DEPENDS sql/*.c sql/*.h)
set(sources
    ${decode}
    ${fft}
    ${method}
    ${sql}
    main.cpp
)
#message(STATUS ${sources})
#target_sources(${PROJECT_NAME} PRIVATE ${APP_RESOURCES})
#target_include_directories(${PROJECT_NAME} module/)

add_executable(${PROJECT_NAME}
    ${sources}
)

# 动态库
# add_library(${PROJECT_NAME}_shared
#     SHARED
#     ${sources}
# )

# 静态库
# add_library(${PROJECT_NAME}_static
#     ${sources}
# )

target_link_libraries(${PROJECT_NAME} PRIVATE
    log
)

#find_package(Threads REQUIRED)
#target_link_libraries(test Threads::Threads)

# target_link_libraries(${PROJECT_NAME}
#     PUBLIC
#         log
# )

# set_target_properties(${PROJECT_NAME} PROPERTIES
#     # 检测栈内存溢出 -fsanitize=address
#     # 关闭栈保护 -fno-stack-protector
#     #COMPILE_FLAGS "-fsanitize=address"
#     LINK_FLAGS "-fsanitize=address"
# )

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/static
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# CMake 中安装路径通常基于「前缀路径」（CMAKE_INSTALL_PREFIX），默认值为：
# Linux/macOS：/usr/local
# Windows：C:/Program Files/<项目名>
# 可通过以下方式修改前缀路径：
# 命令行配置时指定：cmake -DCMAKE_INSTALL_PREFIX=/opt/myapp ..
# CMakeLists 中设置：set(CMAKE_INSTALL_PREFIX "/opt/myapp")

# CMAKE_INSTALL_BINDIR	可执行文件路径	bin
# CMAKE_INSTALL_LIBDIR	库文件路径	lib（64 位可能为 lib64）
# CMAKE_INSTALL_INCLUDEDIR	头文件路径	include
# CMAKE_INSTALL_DATADIR	数据文件路径（如文档、配置）	share
# CMAKE_INSTALL_MANDIR	手册页路径	share/man
# message(STATUS "CMAKE_INSTALL_PREFIX:" ${CMAKE_INSTALL_PREFIX})
# message(STATUS "CONFIGURATIONS:" ${CONFIGURATIONS})
# message(STATUS "DESTINATION:" ${DESTINATION})

add_subdirectory(log)

# dl库
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})

# set_target_properties(${PROJECT_NAME} PROPERTIES
#     COMPILE_FLAGS "-ldl"
#     LINK_FLAGS "-ldl"
# )

#add_compile_options(-Wall)
#target_compile_options(${PROJECT_NAME} PRIVATE "-W")
# set_target_properties(${PROJECT_NAME} PROPERTIES
#     COMPILE_FLAGS "-w"
# )

# 设置cmake模块的路径,存放上面函数的脚本名称为 enableWraning.txt，存放在cmake目录下：
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/")
# 引入cmake的模块
include(enableWraning.cmake)
target_set_warnings(${PROJECT_NAME} true false)

#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

# 设置编译器路径
#set(CMAKE_C_COMPILER "/path/to/arm-gcc")
#set(CMAKE_CXX_COMPILER "/path/to/arm-g++")
#set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
#set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/gcc++.exe")
#set(CMAKE_MAKE_PROGRAM "C:/msys64/mingw64/bin/mingw32-make.exe")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

#set(CMAKE_C_COMPILER_FORCED ON)
#set(CMAKE_CXX_COMPILER_FORCED ON)

# 设置目标平台和架构
#set(CMAKE_SYSTEM_NAME Linux)
#set(CMAKE_SYSTEM_PROCESSOR arm)

# -G "Ninja"
# -G "MinGW Makefiles"
# -G "Unix Makefiles"
# -G "Visual Studio 17 2022"
# -DCMAKE_TOOLCHAIN_FILE=./arm-gcc.cmake 指定编译器
# -DCMAKE_MAKE_PROGRAM=make
# -DCMAKE_CXX_COMPILER=g++
# -DCMAKE_C_COMPILER=gcc
# -DCMAKE_BUILD_TYPE=Release 编译类型
# -DCMAKE_MODULE_PATH=<path> 添加自定义 CMake 模块搜索路径
# -DCMAKE_INSTALL_PREFIX=/usr/local 安装路径
# -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=bin 可执行文件输出目录
# -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=lib 动态库输出目录
# -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=static 静态库输出目录
# cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=bin -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=lib -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=static  -S .. -B .
# cmake  -G "Unix Makefiles" -DCMAKE_MAKE_PROGRAM=/usr/bin/make -DCMAKE_CXX_COMPILER=/usr/bin/g++ -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_TOOLCHAIN_FILE=/usr/bin/g++ -DCMAKE_PREFIX_PATH:PATH=/usr/bin -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=bin -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=lib -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=static  -S ../ -B ./
