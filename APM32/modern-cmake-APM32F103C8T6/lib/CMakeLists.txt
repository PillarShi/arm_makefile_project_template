set(TARGET lib)

SET(SRC 
    driver/src/apm32f10x_rcm.c
    driver/src/apm32f10x_gpio.c
    driver/src/apm32f10x_usart.c
    driver/src/apm32f10x_dma.c
    driver/src/apm32f10x_misc.c
    startup/my_startup_apm32f10x_md.S
    )

set(INC_DIR
    ../${INC_DIR}
    .
    CMSIS
    driver/inc
    )

set(LIB
    )

set(DEFINES
    ${DEFINES}
    )

################################ DO NOT CHANGE ###############################
add_library(${TARGET} ${SRC})

target_include_directories(${TARGET} PUBLIC ${INC_DIR})

target_link_libraries(${TARGET} PRIVATE ${LIB})
target_compile_definitions(${TARGET} PRIVATE ${DEFINES})
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>)
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:ASM>:${ASM_FLAGS}>)
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_FLAGS}>)
##############################################################################