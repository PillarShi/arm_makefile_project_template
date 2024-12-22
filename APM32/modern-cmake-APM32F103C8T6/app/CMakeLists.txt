set(TARGET app)

SET(SRC 
    led/led.c
    )

set(INC_DIR
    .
    led
    )

set(LIB
    lib
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