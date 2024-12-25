set(TARGET ${TARGET})

set(SRC
    main.c
    apm32f10x_int.c
)

set(INC_DIR
    .
    )

set(LIB
    )

set(DEFINES
    APM32F10X_MD
    )

set(SUB_DIRS lib app)


################################ DO NOT CHANGE ###############################

add_executable(${TARGET} ${SRC})

#----------------------------------- GCC -------------------------------------
add_custom_command(TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BIN_FILE}
    COMMAND ${CMAKE_OBJDUMP} -p -D -h -m arm ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET} > ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${DIS_FILE}
    COMMAND ${CMAKE_SIZE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET}
    )
#-----------------------------------------------------------------------------

foreach(SUB_DIR ${SUB_DIRS})
    add_subdirectory(../${SUB_DIR} ../${SUB_DIR})
endforeach()
target_link_libraries(${TARGET} PRIVATE ${SUB_DIRS})

target_compile_definitions(${TARGET} PRIVATE ${DEFINES})
target_include_directories(${TARGET} PRIVATE ${INC_DIR})
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>)
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:ASM>:${ASM_FLAGS}>)
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_FLAGS}>)
target_link_options(${TARGET} PRIVATE ${LD_FLAGS})
##############################################################################