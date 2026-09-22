# DPP example for QP/C on the NUCLEO-H533RE board (ARM Cortex-M33 with FPU)
cmake_minimum_required(VERSION 3.25)
set(QPC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

# check the setup before project(): one clear error, no follow-ups ........
if(NOT EXISTS ${QPC_DIR}/include/qpc.h)
    message(FATAL_ERROR "QP/C not found. Keep this example inside qpc/examples.")
endif()
if(NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "No toolchain file. Use a preset: cmake --list-presets")
elseif(IS_ABSOLUTE "${CMAKE_TOOLCHAIN_FILE}" AND NOT EXISTS "${CMAKE_TOOLCHAIN_FILE}")
    message(FATAL_ERROR "Toolchain file not found (qpc/3rd_party missing?).\n"
        "In the qpc folder run: git submodule update --init")
endif()
if(CMAKE_GENERATOR MATCHES "Ninja")
    find_program(CMAKE_MAKE_PROGRAM ninja)
    if(NOT CMAKE_MAKE_PROGRAM)
        message(FATAL_ERROR "Ninja not found on the PATH (https://ninja-build.org)")
    endif()
endif()

project(dpp LANGUAGES C)       # the name of the program

# compiler settings (the same as in the Makefiles) ...........................
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)          # -std=c11, not -std=gnu11
add_compile_options( # C only, not for the assembler
    "$<$<COMPILE_LANGUAGE:C>:-Wall;-Wextra;-Wpedantic;-Wsign-conversion>")
set(CMAKE_C_FLAGS_DEBUG   "-g -O")
set(CMAKE_C_FLAGS_RELEASE "-Os -DNDEBUG")
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON) # -flto in Release
endif()
set(CMAKE_C_FLAGS_SPY     "-g -O")

# the board .................................................................
add_subdirectory(${QPC_DIR}/3rd_party/nucleo-h533re nucleo-h533re)

# QP/C framework ............................................................
add_library(qpc_config INTERFACE)    # qp_config.h is in this directory
target_include_directories(qpc_config INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(qpc_config INTERFACE nucleo-h533re::cpu) # CPU flags

set(QPC_PORT arm-cm)
if(NOT QPC_KERNEL)                   # normally set by the preset
    set(QPC_KERNEL qv)
endif()
string(TOLOWER "${QPC_KERNEL}" kernel)
add_subdirectory(${QPC_DIR} qpc)

# the application ...........................................................
add_executable(${PROJECT_NAME}
    main.c
    philo.c
    table.c
    ${kernel}/bsp.c
)
if(kernel STREQUAL "qxk")
    target_sources(${PROJECT_NAME} PRIVATE qxk/xthread1.c qxk/xthread2.c)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE qpc::qpc nucleo-h533re)
if(CMAKE_C_COMPILER_ID STREQUAL "ARMClang")
    set(script ${kernel}/armclang/project.sct)   # scatter file
else()
    set(script ${kernel}/gnu/project.ld)
endif()
link_image(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/${script})
