set(TARGET ${TARGET})

set(SRC
    main.c
    )

set(INC_DIR
    .
    )

set(LIB
    )

set(DEFINES
    APM32F10X_MD
    )

set(SUB_DIRS_NOT_REAL 
    lib
    board
    app
    ) # 不是子目录，而是其他组
################################ DO NOT CHANGE ###############################
add_executable(${TARGET} ${SRC})

target_include_directories(${TARGET} PRIVATE ${INC_DIR}) #头文件

if (SUB_DIRS_NOT_REAL) # 有其他组
    foreach(SUB_DIR ${SUB_DIRS_NOT_REAL})
        add_subdirectory(../${SUB_DIR} ../${SUB_DIR})
    endforeach()
    add_dependencies(${TARGET} ${SUB_DIRS_NOT_REAL})
    target_link_libraries(${TARGET} PRIVATE ${SUB_DIRS_NOT_REAL})
endif()

target_link_libraries(${TARGET} PRIVATE ${LIB}) # 其他库和组

target_compile_definitions(${TARGET} PRIVATE ${DEFINES}) # 宏定义
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>) # C 编译选项
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:ASM>:${ASM_FLAGS}>) # 汇编 编译选项
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_FLAGS}>) # C++ 编译选项
target_link_options(${TARGET} PRIVATE ${LD_FLAGS}) # 链接选项
##############################################################################