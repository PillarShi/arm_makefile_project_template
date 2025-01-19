set(TARGET app)

SET(SRC
    app.c
    )

set(INC_DIR
    .
    )

set(LIB
    lib
    board
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
        add_subdirectory(${SUB_DIR})
    endforeach()
    add_dependencies(${TARGET} ${SUB_DIRS})
    target_link_libraries(${TARGET} PUBLIC ${SUB_DIRS}) # 作为组成所以PUBLIC
endif()

target_link_libraries(${TARGET} PRIVATE ${LIB}) # 其他库和组

target_compile_definitions(${TARGET} PRIVATE ${DEFINES})
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>)
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:ASM>:${ASM_FLAGS}>)
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_FLAGS}>)
##############################################################################