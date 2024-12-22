cmake_minimum_required(VERSION 3.20)

set(CMAKE_BUILD_TYPE MinSizeRel)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CXX_STANDARD  17)
set(CMAKE_C_STANDARD    99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

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
    bsp
    )

set(SRC
    bsp/main.c
    bsp/system.c
    bsp/apm32f10x_int.c
    bsp/system_apm32f10x.c
)

set(SUB_DIRS lib app)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/lib/startup/my_gcc_APM32F10xx6.ld)

################################ DO NOT CHANGE ###############################

set(TARGET ${ELF_FILE})
add_executable(${TARGET} ${SRC})

#----------------------------------- GCC -------------------------------------
set(CC_PREFIX arm-none-eabi-)
set(CMAKE_C_COMPILER    ${CC_PREFIX}gcc)
set(CMAKE_ASM_COMPILER  ${CC_PREFIX}gcc)
set(CMAKE_CXX_COMPILER  ${CC_PREFIX}g++)
set(CMAKE_AR            ${CC_PREFIX}ar)
set(CMAKE_OBJCOPY       ${CC_PREFIX}objcopy)
set(CMAKE_OBJDUMP       ${CC_PREFIX}objdump)
set(CMAKE_SIZE          ${CC_PREFIX}size)

add_custom_command(TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex ${TARGET} ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${TARGET} ${BIN_FILE}
    COMMAND ${CMAKE_OBJDUMP} -p -D -h -m arm ${TARGET} > ${DIS_FILE}
    COMMAND ${CMAKE_SIZE} ${TARGET}
    )

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
    -mthumb -mthumb-interwork -mcpu=cortex-m3
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
    -x assembler-with-cpp -Wa,-mimplicit-it=thumb
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
#-----------------------------------------------------------------------------

foreach(SUB_DIR ${SUB_DIRS})
    add_subdirectory(${SUB_DIR})
endforeach()
target_link_libraries(${TARGET} PRIVATE ${SUB_DIRS})

target_compile_definitions(${TARGET} PRIVATE ${DEFINES})
target_include_directories(${TARGET} PRIVATE ${INC_DIR})
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>)
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:ASM>:${ASM_FLAGS}>)
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_FLAGS}>)
target_link_options(${TARGET} PRIVATE ${LD_FLAGS})
##############################################################################