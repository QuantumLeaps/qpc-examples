##############################################################################
# Makefile for QUTEST-QP/C for Windows and POSIX *HOSTS*
#
# Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
#
#                    Q u a n t u m  L e a P s
#                    ------------------------
#                    Modern Embedded Software
#
# SPDX-License-Identifier: LicenseRef-QL-cert
#
# This software is part of the QP/C Certification Kit, which is licensed
# under the terms of the Quantum Leaps Certification Kit licenses. Please
# contact Quantum Leaps for more information about the available licensing
# options.
#
# RESTRICTIONS
# You may NOT:
# (a) redistribute, encumber, sell, rent, lease, sublicense, or otherwise
#     transfer rights in this software,
# (b) remove or alter any trademark, logo, copyright or other proprietary
#     notices, legends, symbols or labels present in this software,
# (c) plagiarize this software to sidestep the licensing obligations.
#
# Quantum Leaps contact information:
# <www.state-machine.com/licensing>
# <info@state-machine.com>
##############################################################################
#
# examples of invoking this Makefile:
# make         # make and run the Python tests in the current directory
# make TESTS=test*.py  # make and run the selected tests in the curr. dir.
# make GCOV=1  # make and run the Python tests and collect coverage info
# make norun   # only make but not run the tests
# make clean   # cleanup the build
# make debug   # only run tests in DEBUG mode
#
# NOTE:
# To use this Makefile on Windows, you will need the GNU make utility, which
# is included in the QTools collection for Windows, see:
#    https://github.com/QuantumLeaps/qtools

#-----------------------------------------------------------------------------
# project:
#
PROJECT := qep_hsm

VPATH := . \
	../src

INCLUDES := -I. \
	-I../src

C_SRCS := \
	tstsm.c \
	tstsm_sf.c \
	test_fixture.c

DEFINES  :=

CPP_SRCS :=
LIB_DIRS :=
LIBS     :=
OUTPUT   := $(PROJECT)

#-----------------------------------------------------------------------------
# target
#
TARGET   := host

DEFINES += -DQ_SPY -DQ_UTEST \
	-DQ_HOST

BIN_DIR := build_$(TARGET)

#-----------------------------------------------------------------------------
# QP framework
#

# location of the SafeQP/C framework (if not provided in an env. variable)
ifeq ($(QP),)
QP := ../../../..
endif

ifeq ($(OS),Windows_NT)

QP_PORT_DIR := $(QP)/ports/win32-qutest
LIBS += -lws2_32

else

QP_PORT_DIR := $(QP)/ports/posix-qutest
#QP_PORT_DIR := $(QP)/ports/posix
LIBS += -lpthread

endif

VPATH += \
	$(QP)/src/qs \
	$(QP_PORT_DIR)

INCLUDES += \
	-I$(QP)/include \
	-I$(QP_PORT_DIR)

#-----------------------------------------------------------------------------
# SafeQP/C framework sources
#
QP_SRCS := \
	qep_hsm.c \
	qep_msm.c \
	qf_act.c \
	qf_actq.c \
	qf_defer.c \
	qf_dyn.c \
	qf_mem.c \
	qf_ps.c \
	qf_qact.c \
	qf_qeq.c \
	qf_qmact.c \
	qf_time.c \
	qs.c \
	qs_64bit.c \
	qs_rx.c \
	qs_fp.c \
	qutest.c \
	qutest_port.c

VPATH += \
	$(QP)/src/qf \
	$(QP)/src/qs \
	$(QP_PORT_DIR)

INCLUDES += \
	-I$(QP)/include \
	-I$(QP_PORT_DIR)

# add the QP sources to the build
C_SRCS += $(QP_SRCS)

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
COV   := gcov -f

#-----------------------------------------------------------------------------
# basic utilities (included in QTools for Windows), see:
#     https://www.state-machine.com/qtools
#
ifeq ($(OS),Windows_NT)
	MKDIR      := mkdir
	RM         := rm
	TARGET_EXT := .exe
else ifeq ($(OSTYPE),cygwin)
	MKDIR      := mkdir -p
	RM         := rm -f
	TARGET_EXT := .exe
else
	MKDIR      := mkdir -p
	RM         := rm -f
	TARGET_EXT :=
endif


##############################################################################
# Typically you should not need to change anything below this line

# make sure that QTOOLS env. variable is defined...
ifeq ("$(wildcard $(QTOOLS))","")
$(error QTOOLS not found. Please install QTools and define QTOOLS env. variable)
endif

#-----------------------------------------------------------------------------
# QUTest test script utilities (requires QTOOLS):
#
ifeq ("$(wildcard $(QUTEST))","")
QUTEST := python3 $(QTOOLS)/qutest/qutest.py
endif

#-----------------------------------------------------------------------------
# build options
CFLAGS := -c -g -fno-pie -std=c11 -pedantic -Wall -Wextra -W \
	-O0 $(INCLUDES) $(DEFINES) $(DEF)

CPPFLAGS := -c -g -fno-pie -std=c++17 -pedantic -Wall -Wextra \
	-fno-rtti -fno-exceptions \
	-O0 $(INCLUDES) $(DEFINES) $(DEF)

ifndef GCC_OLD
	LINKFLAGS := -no-pie
endif

ifdef GCOV
	CFLAGS    += -fprofile-arcs -ftest-coverage
	CPPFLAGS  += -fprofile-arcs -ftest-coverage
	LINKFLAGS += -lgcov --coverage
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
	$(CC) $(CFLAGS) $(QP)/src/qs/qstamp.c -o $(BIN_DIR)/qstamp.o
	$(LINK) $(LINKFLAGS) $(LIB_DIRS) -o $@ $^ $(BIN_DIR)/qstamp.o $(LIBS)

run : $(TARGET_EXE)
ifdef GCOV
	-$(RM) $(BIN_DIR)/*.gcda
endif
	$(QUTEST) -e$(TARGET_EXE) -q$(QSPY) -l$(LOG) -o$(OPT) -- $(TESTS)
ifdef GCOV
	$(COV) -o $(BIN_DIR) $(PROJECT)
endif

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

debug :
	$(QUTEST) -edebug -q$(QSPY) -l$(LOG) -o$(OPT) -- $(TESTS)

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
