##############################################################################
# Makefile for the Unity test harness v2.5.2, for Windows, Linux, MacOS
#
# Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
#
#                   Q u a n t u m  L e a P s
#                   ------------------------
#                   Modern Embedded Software
#
# SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
#
# The QP/C software is dual-licensed under the terms of the open-source GNU
# General Public License (GPL) or under the terms of one of the closed-
# source Quantum Leaps commercial licenses.
#
# Redistributions in source code must retain this top-level comment block.
# Plagiarizing this software to sidestep the license obligations is illegal.
#
# NOTE:
# The GPL (see <www.gnu.org/licenses/gpl-3.0>) does NOT permit the
# incorporation of the QP/C software into proprietary programs. Please
# contact Quantum Leaps for commercial licensing options, which expressly
# supersede the GPL and are designed explicitly for licensees interested
# in using QP/C in closed-source proprietary applications.
#
# Quantum Leaps contact information:
# <www.state-machine.com/licensing>
# <info@state-machine.com>
##############################################################################
#
# examples of invoking this Makefile:
# make         # make and run the tests in the current directory
# make norun   # only make but not run the tests
# make clean   # cleanup the build
#
# NOTE:
# To use this Makefile on Windows, you will need the GNU make utility, which
# is included in the QTools collection for Windows, see:
#    https://github.com/QuantumLeaps/qtools

#-----------------------------------------------------------------------------
# project name:
#
PROJECT := ProductionCode

#-----------------------------------------------------------------------------
# project directories:
#

# list of all source directories used by this project
VPATH = . \
	../src \
	mocks

# list of all include directories needed by this project
INCLUDES := -I. \
	-I../src \
	-Imocks

#-----------------------------------------------------------------------------
# project files:
#

# C source files...
C_SRCS := \
	ProductionCode.c \
	TestProductionCode.c \
	TestProductionCode_Runner.c \

# C++ source files...
CPP_SRCS :=

LIB_DIRS :=
LIBS     :=

# defines...
DEFINES  := $(DEF)

#-----------------------------------------------------------------------------
# target
#
TARGET   := host
BIN_DIR := build_$(TARGET)

OUTPUT   := $(PROJECT)

#-----------------------------------------------------------------------------
# external dependencies

# make sure that QTOOLS env. variable is defined...
ifeq ("$(wildcard $(QTOOLS))","")
$(error QTOOLS not found. Please install QTools and define QTOOLS env. variable)
endif

# location of the QTOOLS directory (if not provided in an environment var.)
ifeq ($(QTOOLS),)
QTOOLS := c:/qp/qtools
endif

# location of the Unity unit testing harness
ifeq ($(UNITY),)
UNITY := $(QTOOLS)/unity
endif

# add QP/C, Unity/CMock to the build...
VPATH    += $(UNITY)/src $(UNITY)/extras/cmock/src
INCLUDES += -I$(UNITY)/src -I$(UNITY)/extras/cmock/src
C_SRCS   += unity.c cmock.c

#============================================================================
# Typically you should not need to change anything below this line

#-----------------------------------------------------------------------------
# GNU toolset:
# NOTE:  GNU toolset (MinGW) is included in the QTools collection for Windows, see:
#     https://www.state-machine.com/qtools
# It is assumed that %QTOOLS%\bin directory is added to the PATH
#
CC    := gcc
CPP   := g++
LINK  := gcc    # for C programs
#LINK  := g++   # for C++ programs
COV   := gcov -abcg

#-----------------------------------------------------------------------------
# basic utilities (included in QTools for Windows), see:
#     https://www.state-machine.com/qtools
#
ifeq ($(OS),Windows_NT)
	MKDIR      := mkdir
	RM         := rm
	CAT        := type
	TARGET_EXT := .exe
else ifeq ($(OSTYPE),cygwin)
	MKDIR      := mkdir -p
	RM         := rm -f
	CAT        := cat
	TARGET_EXT := .exe
else
	MKDIR      := mkdir -p
	RM         := rm -f
	TARGET_EXT :=
endif

#-----------------------------------------------------------------------------
# build options
CFLAGS := -c -g -fno-pie -std=c11 -pedantic -Wall -Wextra -W \
	-O0 --coverage -fcondition-coverage \
	$(INCLUDES) $(DEFINES) $(DEF)

CPPFLAGS := -c -g -fno-pie -std=c++17 -pedantic -Wall -Wextra \
	-O0 --coverage -fcondition-coverage \
	-fno-rtti -fno-exceptions \
	$(INCLUDES) $(DEFINES) $(DEF)

LINKFLAGS := --coverage -fcondition-coverage

ifndef GCC_OLD
	LINKFLAGS += -no-pie
endif

#-----------------------------------------------------------------------------
C_OBJS       := $(patsubst %.c,%.o,   $(C_SRCS))
CPP_OBJS     := $(patsubst %.cpp,%.o, $(CPP_SRCS))

TARGET_EXE   := $(BIN_DIR)/$(PROJECT)$(TARGET_EXT)
C_OBJS_EXT   := $(addprefix $(BIN_DIR)/, $(C_OBJS))
C_DEPS_EXT   := $(patsubst %.o, %.d, $(C_OBJS_EXT))
CPP_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(CPP_OBJS))
CPP_DEPS_EXT := $(patsubst %.o, %.d, $(CPP_OBJS_EXT))

# create $(BIN_DIR) if it does not exist
ifeq ("$(wildcard $(BIN_DIR))","")
$(shell $(MKDIR) $(BIN_DIR))
endif

#-----------------------------------------------------------------------------
# rules
#

.PHONY : norun debug clean show

ifeq ($(MAKECMDGOALS),norun)
all : $(TARGET_EXE)
norun : all
else
all : $(TARGET_EXE) run
endif

$(TARGET_EXE) : $(C_OBJS_EXT) $(CPP_OBJS_EXT)
	$(LINK) $(LINKFLAGS) $(LIB_DIRS) -o $@ $^ $(LIBS)

run : $(TARGET_EXE)
	-$(RM) $(BIN_DIR)/*.gcda
	$(TARGET_EXE)
	$(COV) -o $(BIN_DIR) $(PROJECT) > $(PROJECT).cov
	$(CAT) $(PROJECT).cov

$(BIN_DIR)/%.d : %.c
	$(CC) -MM -MT $(@:.d=.o) $(CFLAGS) $< > $@

$(BIN_DIR)/%.d : %.cpp
	$(CPP) -MM -MT $(@:.d=.o) $(CPPFLAGS) $< > $@

$(BIN_DIR)/%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/%.o : %.cpp
	$(CPP) $(CPPFLAGS) $< -o $@

# create BIN_DIR and include dependencies only if needed
ifneq ($(MAKECMDGOALS),clean)
  ifneq ($(MAKECMDGOALS),show)
    ifneq ($(MAKECMDGOALS),debug)
ifeq ("$(wildcard $(BIN_DIR))","")
$(shell $(MKDIR) $(BIN_DIR))
endif
-include $(C_DEPS_EXT) $(CPP_DEPS_EXT)
    endif
  endif
endif

clean :
	-$(RM) $(BIN_DIR)/*.*

show :
	@echo PROJECT      = $(PROJECT)
	@echo BIN_DIR      = $(BIN_DIR)
	@echo TARGET_EXE   = $(TARGET_EXE)
	@echo MAKECMDGOALS = $(MAKECMDGOALS)
	@echo TESTS        = $(TESTS)
	@echo VPATH        = $(VPATH)
	@echo INCLUDES     = $(INCLUDES)
	@echo C_SRCS       = $(C_SRCS)
	@echo CPP_SRCS     = $(CPP_SRCS)
	@echo C_DEPS_EXT   = $(C_DEPS_EXT)
	@echo C_OBJS_EXT   = $(C_OBJS_EXT)
	@echo CPP_DEPS_EXT = $(CPP_DEPS_EXT)
	@echo CPP_OBJS_EXT = $(CPP_OBJS_EXT)
	@echo LIB_DIRS     = $(LIB_DIRS)
	@echo LIBS         = $(LIBS)
	@echo DEFINES      = $(DEFINES)
	@echo QTOOLS       = $(QTOOLS)
	@echo HOST         = $(HOST)
	@echo QUTEST       = $(QUTEST)
	@echo TESTS        = $(TESTS)
