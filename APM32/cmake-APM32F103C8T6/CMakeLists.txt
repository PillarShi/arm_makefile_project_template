cmake_minimum_required(VERSION 3.20)

set(CMAKE_BUILD_TYPE MinSizeRel)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CXX_STANDARD  17)
set(CMAKE_C_STANDARD    99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(temp
        VERSION 0.0.1
        LANGUAGES CXX C ASM)

set(ELF_FILE ${PROJECT_NAME}.elf)
set(HEX_FILE ${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_NAME}.bin)
set(MAP_FILE ${PROJECT_NAME}.map)
set(DIS_FILE ${PROJECT_NAME}.dis)

set(DEFINES 
    APM32F10X_MD
    )

set(INC_DIR 
    app
    app/led
    bsp
    lib/CMSIS
    lib/driver/inc
    )

file(GLOB_RECURSE C_SRC
    lib/driver/src/apm32f10x_rcm.c
	lib/driver/src/apm32f10x_gpio.c
	lib/driver/src/apm32f10x_usart.c
	lib/driver/src/apm32f10x_dma.c
	lib/driver/src/apm32f10x_misc.c
	bsp/*.c
	app/led/led.c
)

#################################### GCC #####################################
set(ASM_SRC lib/startup/my_startup_apm32f10x_md.S)
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/lib/startup/my_gcc_APM32F10xx6.ld)

set(CC_PREFIX arm-none-eabi-)
set(CMAKE_C_COMPILER    ${CC_PREFIX}gcc)
set(CMAKE_ASM_COMPILER  ${CC_PREFIX}gcc)
set(CMAKE_CXX_COMPILER  ${CC_PREFIX}g++)
set(CMAKE_AR            ${CC_PREFIX}ar)
set(CMAKE_OBJCOPY       ${CC_PREFIX}objcopy)
set(CMAKE_OBJDUMP       ${CC_PREFIX}objdump)
set(CMAKE_SIZE          ${CC_PREFIX}size)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    set(BUILD_FLAGS -Of)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    set(BUILD_FLAGS -Of -g -gdwarf-2)
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
    -mthumb -mcpu=cortex-m3
    -mfloat-abi=soft
    -ffunction-sections -fdata-sections
    -fno-common
    )

set(C_FLAGS
    ${COMMON_FLAGS} ${BUILD_FLAGS}
    -fomit-frame-pointer -Wall
    )

set(ASM_FLAGS
    ${COMMON_FLAGS} ${BUILD_FLAGS}
    -x assembler-with-cpp
    )

set(CXX_FLAGS
    ${COMMON_FLAGS} ${BUILD_FLAGS}
    -fomit-frame-pointer -Wall
    -fno-exceptions -fno-rtti -fno-threadsafe-statics -fvisibility=hidden
    )

set(LD_FLAGS 
    ${COMMON_FLAGS}
    -specs=nano.specs -specs=nosys.specs
    -Wl,--gc-sections,-cref,-Map=${MAP_FILE}
    -T${LINKER_SCRIPT}
    )

##############################################################################

add_executable(${ELF_FILE} ${C_SRC} ${ASM_SRC})
target_compile_definitions(${ELF_FILE} PRIVATE ${DEFINES})
target_include_directories(${ELF_FILE} PRIVATE ${INC_DIR})
target_compile_options(${ELF_FILE} PRIVATE $<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>)
target_compile_options(${ELF_FILE} PRIVATE $<$<COMPILE_LANGUAGE:ASM>:${ASM_FLAGS}>)
target_compile_options(${ELF_FILE} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_FLAGS}>)
target_link_options(${ELF_FILE} PRIVATE ${LD_FLAGS})

add_custom_command(TARGET ${ELF_FILE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex ${ELF_FILE} ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${ELF_FILE} ${BIN_FILE}
    COMMAND ${CMAKE_OBJDUMP} -p -D -h -m arm ${ELF_FILE} > ${DIS_FILE}
    COMMAND ${CMAKE_SIZE} ${ELF_FILE}
    )