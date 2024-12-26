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

add_dependencies(RUN_POST_BUILD ${TARGET})

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