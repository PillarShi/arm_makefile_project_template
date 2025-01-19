cmake_minimum_required(VERSION 3.20)

set(CMAKE_BUILD_TYPE MinSizeRel)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_SYSTEM_NAME Generic) # 目标系统 Windows Linux Android 等 嵌入式(裸机/RTOS)-Generic
set(CMAKE_SYSTEM_PROCESSOR arm) # 目标系统的处理器架构
set(CMAKE_CXX_STANDARD  17)
set(CMAKE_C_STANDARD    99)

project(morden_cmake_project
        VERSION 0.0.1
        LANGUAGES CXX C ASM
        ) # project message

set(REAL_CMAKE_DIR main) # 进入正真构建cmake的目录

set(ELF_FILE ${PROJECT_NAME}.elf)
set(HEX_FILE ${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_NAME}.bin)
set(MAP_FILE ${PROJECT_NAME}.map)
set(DIS_FILE ${PROJECT_NAME}.dis)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/lib/startup/my_gcc_APM32F10xx6.ld)
################################ BUILD SETTING ###############################
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../out) # 可执行文件生产目录重定义
set(TARGET ${ELF_FILE})
#----------------------------------- GCC -------------------------------------
set(CC_PREFIX arm-none-eabi-)
set(CMAKE_C_COMPILER    ${CC_PREFIX}gcc)
set(CMAKE_ASM_COMPILER  ${CC_PREFIX}gcc)
set(CMAKE_CXX_COMPILER  ${CC_PREFIX}g++)
set(CMAKE_AR            ${CC_PREFIX}ar)
set(CMAKE_OBJCOPY       ${CC_PREFIX}objcopy)
set(CMAKE_OBJDUMP       ${CC_PREFIX}objdump)
set(CMAKE_SIZE          ${CC_PREFIX}size)

add_custom_target(RUN_POST_BUILD ALL
    COMMAND ${CMAKE_OBJCOPY} -Oihex ${TARGET} ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${TARGET} ${BIN_FILE}
    COMMAND ${CMAKE_OBJDUMP} -p -D -h -m arm ${TARGET} > ${DIS_FILE}
    COMMAND ${CMAKE_SIZE} ${TARGET}
    DEPENDS ${TARGET}
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    ) # 总是在 ${TARGET} 后执行

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    set(BUILD_FLAGS -Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    set(BUILD_FLAGS -Ofast -g -gdwarf-2)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    set(BUILD_FLAGS -Os)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message(STATUS "Minimal optimization, debug info included")
    set(BUILD_FLAGS -Og -g -gdwarf-2)
else ()
    message(ERROR "CMAKE_BUILD_TYPE ERROR")
endif ()

set(COMMON_FLAGS
    -mthumb -mthumb-interwork -mcpu=cortex-m3
    -mfloat-abi=soft
    -ffunction-sections -fdata-sections
    -fno-common
    )

set(C_FLAGS
    ${COMMON_FLAGS} ${BUILD_FLAGS}
    -fomit-frame-pointer -Wall
    ) # C 编译选项

set(ASM_FLAGS
    ${COMMON_FLAGS} ${BUILD_FLAGS}
    -x assembler-with-cpp -Wa,-mimplicit-it=thumb
    ) # 汇编 编译选项

set(CXX_FLAGS
    ${COMMON_FLAGS} ${BUILD_FLAGS}
    -fomit-frame-pointer -Wall
    -fno-exceptions -fno-rtti -fno-threadsafe-statics -fvisibility=hidden
    ) # C++ 编译选项

set(LD_FLAGS 
    ${COMMON_FLAGS}
    -specs=nano.specs -specs=nosys.specs
    -Wl,--gc-sections,-cref,-Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${MAP_FILE}
    -T${LINKER_SCRIPT}
    ) # 链接选项
#-----------------------------------------------------------------------------
add_subdirectory(${REAL_CMAKE_DIR}) # 进入正真构建cmake
##############################################################################