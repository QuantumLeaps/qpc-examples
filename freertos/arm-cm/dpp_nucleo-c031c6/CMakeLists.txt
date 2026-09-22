# DPP example for QP/C on FreeRTOS, NUCLEO-C031C6 board (ARM Cortex-M0+)
cmake_minimum_required(VERSION 3.25)
set(QPC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)

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

# compiler settings (the same as in the Makefile) ...........................
add_compile_options("$<$<COMPILE_LANGUAGE:C>:-Wall>") # not for the assembler
set(CMAKE_C_FLAGS_DEBUG   "-g -O")
set(CMAKE_C_FLAGS_RELEASE "-Os -DNDEBUG")    # no -flto on ARMv6-M
set(CMAKE_C_FLAGS_SPY     "-g -O")

# the board .................................................................
add_subdirectory(${QPC_DIR}/3rd_party/nucleo-c031c6 nucleo-c031c6)

# FreeRTOS (with its own CMake support) .....................................
add_library(freertos_config INTERFACE) # FreeRTOSConfig.h is in this directory
target_include_directories(freertos_config INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(freertos_config INTERFACE nucleo-c031c6::cpu) # CPU flags

set(FREERTOS_PORT GCC_ARM_CM0)       # FreeRTOS port for ARMv6-M (the GCC
                                     # ports are also for ARMClang)
                                     # no FREERTOS_HEAP: static allocation only
add_subdirectory(${QPC_DIR}/3rd_party/FreeRTOS-Kernel FreeRTOS)

# QP/C framework ............................................................
add_library(qpc_config INTERFACE)    # qp_config.h is in this directory
target_include_directories(qpc_config INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(qpc_config INTERFACE nucleo-c031c6::cpu) # CPU flags

set(QPC_PORT freertos)
add_subdirectory(${QPC_DIR} qpc)

# the application ...........................................................
add_executable(${PROJECT_NAME}
    main.c
    philo.c
    table.c
    bsp.c
)
target_link_libraries(${PROJECT_NAME} PRIVATE qpc::qpc nucleo-c031c6)
if(CMAKE_C_COMPILER_ID STREQUAL "ARMClang")
    set(script armclang/dpp.sct)     # scatter file
else()
    set(script gnu/dpp.ld)
endif()
link_image(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/${script})
