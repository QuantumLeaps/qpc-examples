# dpp

# use a recent CMake version
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
cmake_policy(VERSION 3.13)
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.14")
    cmake_policy(SET CMP0083 NEW)
endif()

# first of all protect against in-source builds
file(REAL_PATH "${CMAKE_SOURCE_DIR}" _srcdir)
file(REAL_PATH "${CMAKE_BINARY_DIR}" _bindir)

if(${_srcdir} STREQUAL ${_bindir})
    message(FATAL_ERROR "  FATAL: In-source builds are not allowed!
        You should create a separate directory for build files.")
endif()
unset(_srcdir)
unset(_bindir)

# set up configurable options
set(PROJECT_NAME dpp CACHE STRING "set to the project name of the qpc library project (default: QPC)")
set(QPC_CFG_KERNEL QV CACHE STRING "set to the kernel to use (QV, QK, QXK) (default: QV)")
set(QPC_SDK_PATH "${CMAKE_SOURCE_DIR}/../../.." CACHE STRING "set to the source path of QPC (default: ../../..)")

option(QPC_CFG_UNIT_TEST  "set to ON, if Q_UTEST shall be enabled (default: OFF)" OFF)
option(QPC_CFG_GUI        "set to ON, if a Windows (TM) GUI shall be compiled in (default: ON)" OFF)
option(QPC_CFG_VERBOSE   "set to ON, to enable more verbosity. (default: OFF)" OFF)
option(QPC_CFG_DEBUG     "set to ON, to enable debug settings. (default: ON)" ON)

# update CMAKE_MODULE_PATH
list(PREPEND CMAKE_MODULE_PATH ${QPC_SDK_PATH}/3rd_party/cmake ${CMAKE_SOURCE_DIR}/Source/cmake ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_SOURCE_DIR} ${HOME_PATH}/cmake)

# set the qpc include path to find our local `qp_config.h`
set(QPC_CFG_QPCONFIG_H_INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR})
# import QPC SDK; download from git, if necessary
include(qpc_sdk_import)

if(NOT SW_VERSION)
    set(SW_VERSION "1.0.0" CACHE STRING "Software Version")
endif()

project(
    ${PROJECT_NAME}
    VERSION ${SW_VERSION}
    DESCRIPTION "Dining Philosophers Problem (dpp) using qpc@win32"
    LANGUAGES C
)

# add support for SPY configuration if not set via CMAKE_TOOLCHAIN_FILE
if(NOT CMAKE_C_FLAGS_SPY)
    foreach(LANG IN ITEMS C CXX ASM)
        set(CMAKE_${LANG}_FLAGS_SPY "${CMAKE_${LANG}_FLAGS_DEBUG} -DQ_SPY")
    endforeach()
endif()

include(CTest)
include(CheckIncludeFile)
include(CheckLibraryExists)

# the project target
add_executable(dpp bsp.c main.c philo.c table.c)

# set position independent code compile/link parameters
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.14")
    include(CheckPIESupported)
    check_pie_supported()
    set_property(TARGET dpp PROPERTY POSITION_INDEPENDENT_CODE FALSE)
endif()

# add the qpc library to the project
qpc_sdk_init()
target_compile_definitions(qpc PUBLIC
    QP_CONFIG
)
target_include_directories(qpc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# set general defines and options
target_compile_definitions(dpp PRIVATE
    $<$<BOOL:${QPC_CFG_GUI}>:QWIN_GUI>
)

target_compile_options(dpp PRIVATE
    $<$<C_COMPILER_ID:GNU>:-mconsole>
    $<$<BOOL:${QPC_CFG_VERBOSE}>:-v>
)

target_link_options(dpp PRIVATE
    $<$<C_COMPILER_ID:GNU>:-mconsole>
    $<$<BOOL:${QPC_CFG_VERBOSE}>:-v>
)

# link the library with the application
target_link_libraries(dpp PRIVATE
    qpc
    $<$<CONFIG:Spy>:ws2_32>
)

# show configuration results
message(STATUS
"Configured project ${PROJECT_NAME} for ${PORT}
 Configuration options set:
    PROJECT_NAME                = ${QPC_PROJECT}
    SW_VERSION                  = ${SW_VERSION}

    CONFIG_GUI                  = ${QPC_CFG_GUI}
    CONFIG_QSPY                 = ${QPC_CFG_QSPY}
    CONFIG_UNIT_TEST            = ${QPC_CFG_UNIT_TEST}
    CONFIG_DEBUG                = ${CONFIG_DEBUG}

    QPC_CFG_KERNEL              = ${QPC_CFG_KERNEL}
    QPC_DIR                     = ${QPC_DIR}"
)

if(QPC_CFG_VERBOSE)
message(STATUS
" System information:
    CMAKE_VERSION               = ${CMAKE_VERSION}
    CMAKE_CROSSCOMPILING        = ${CMAKE_CROSSCOMPILING}
    CMAKE_HOST_SYSTEM           = ${CMAKE_HOST_SYSTEM}
    CMAKE_HOST_SYSTEM_NAME      = ${CMAKE_HOST_SYSTEM_NAME}
    CMAKE_HOST_LINUX            = ${CMAKE_HOST_LINUX}
    CMAKE_HOST_UNIX             = ${CMAKE_HOST_UNIX}
    CMAKE_HOST_WIN32            = ${CMAKE_HOST_WIN32}
    CMAKE_SYSTEM                = ${CMAKE_SYSTEM}
    CMAKE_SYSTEM_NAME           = ${CMAKE_SYSTEM_NAME}
    CMAKE_SYSTEM_PROCESSOR      = ${CMAKE_SYSTEM_PROCESSOR}
    WIN32                       = ${WIN32}
    MSYS                        = ${MSYS}
    MINGW                       = ${MINGW}
    UNIX                        = ${UNIX}
    LINUX                       = ${LINUX}

    CMAKE_BUILD_TYPE            = ${CMAKE_BUILD_TYPE}
    CMAKE_CONFIGURATION_TYPES   = ${CMAKE_CONFIGURATION_TYPES}

    CMAKE_C_COMPILER            = ${CMAKE_C_COMPILER}
    CMAKE_C_COMPILER_ID         = ${CMAKE_C_COMPILER_ID}
    CMAKE_C_COMPILER_VERSION    = ${CMAKE_C_COMPILER_VERSION}
    CMAKE_C_FLAGS               = ${CMAKE_C_FLAGS}

    CMAKE_CXX_COMPILER          = ${CMAKE_CXX_COMPILER}
    CMAKE_CXX_FLAGS             = ${CMAKE_CXX_FLAGS}

    CMAKE_ASM_COMPILER          = ${CMAKE_ASM_COMPILER}
    CMAKE_ASM_FLAGS             = ${CMAKE_ASM_FLAGS}"
)
endif()
