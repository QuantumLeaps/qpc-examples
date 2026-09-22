# DPP example for QP/C on the host: Windows (win32 port) or POSIX (posix port)
cmake_minimum_required(VERSION 3.25)

# check the setup before project(): one clear error, no follow-ups ........
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/qpc.h)
    message(FATAL_ERROR "QP/C not found. Keep this example inside qpc/examples.")
endif()
if(CMAKE_GENERATOR MATCHES "Ninja")
    find_program(CMAKE_MAKE_PROGRAM ninja)
    if(NOT CMAKE_MAKE_PROGRAM)
        message(FATAL_ERROR "Ninja not found on the PATH (https://ninja-build.org)")
    endif()
endif()
if(NOT CMAKE_C_COMPILER)             # without the preset: also use gcc
    set(CMAKE_C_COMPILER gcc)
endif()
find_program(HOST_CC NAMES ${CMAKE_C_COMPILER})
if(NOT HOST_CC)
    message(FATAL_ERROR "C compiler '${CMAKE_C_COMPILER}' not found on the PATH.\n"
        "Windows (QP-bundle): set PATH=%QTOOLS%\\mingw32\\bin;%PATH%\n"
        "Linux (Debian/Ubuntu): sudo apt install build-essential")
endif()

project(dpp LANGUAGES C)       # the name of the program

# compiler settings (the same as in the Makefile) ...........................
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)          # -std=c11, not -std=gnu11
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wsign-conversion)
    set(CMAKE_C_FLAGS_DEBUG   "-g -O")
    set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
    set(CMAKE_C_FLAGS_SPY     "-g -O")
endif()

# QP/C framework ............................................................
add_library(qpc_config INTERFACE)    # qp_config.h is in this directory
target_include_directories(qpc_config INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
    set(QPC_PORT win32)
else()
    set(QPC_PORT posix)
endif()
add_subdirectory(../../.. qpc)

# the application ...........................................................
add_executable(${PROJECT_NAME}
    main.c
    philo.c
    table.c
    bsp.c
)
target_link_libraries(${PROJECT_NAME} PRIVATE qpc::qpc)
