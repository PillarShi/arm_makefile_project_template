set(TARGET lib)

SET(SRC
    system.c
    system_apm32f10x.c
    driver/src/apm32f10x_rcm.c
    driver/src/apm32f10x_gpio.c
    driver/src/apm32f10x_usart.c
    driver/src/apm32f10x_dma.c
    driver/src/apm32f10x_misc.c
    startup/my_startup_apm32f10x_md.S
    )

set(INC_DIR
    .
    CMSIS
    driver/inc
    )

set(LIB
    )

set(DEFINES
    ${DEFINES}
    )

set(SUB_DIRS
    )
################################ DO NOT CHANGE ###############################
add_library(${TARGET} OBJECT ${SRC}) # OBJECT 不能互相循环包含 STATIC 可以

target_include_directories(${TARGET} PUBLIC ${INC_DIR})

if (SUB_DIRS) # 存在子目录作为组成
    foreach(SUB_DIR ${SUB_DIRS})
        add_subdirectory(../${SUB_DIR} ../${SUB_DIR})
    endforeach()
    add_dependencies(${TARGET} ${SUB_DIRS})
    target_link_libraries(${TARGET} PUBLIC ${SUB_DIRS}) # 作为组成所以PUBLIC
endif()

target_link_libraries(${TARGET} PRIVATE ${LIB})  # 其他库和组

target_compile_definitions(${TARGET} PRIVATE ${DEFINES})
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>)
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:ASM>:${ASM_FLAGS}>)
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_FLAGS}>)
##############################################################################