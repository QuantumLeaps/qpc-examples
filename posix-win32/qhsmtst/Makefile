##############################################################################
# Makefile for QP/C on the host (Windows/Linux/macOS)
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
# building configurations: Debug (default), Release, and Spy
# make
# make CONF=rel
# make CONF=spy
# make clean   # cleanup the build
# make CONF=spy clean   # cleanup the build
#
# NOTE:
# To use this Makefile on Windows, you will need the GNU make utility, which
# is included in the QTools collection for Windows, see:
#    https://github.com/QuantumLeaps/qtools

#-----------------------------------------------------------------------------
# project:
#
PROJECT := qhsmtst

VPATH := .

INCLUDES := -I.

C_SRCS := \
	main.c \
	qhsmtst.c

DEFINES  :=

CPP_SRCS :=
LIB_DIRS :=
LIBS     :=
OUTPUT   := $(PROJECT)

#-----------------------------------------------------------------------------
# target
#
TARGET   := host

DEFINES :=

BIN_DIR := build_$(TARGET)

#-----------------------------------------------------------------------------
# QP framework
#

# location of the QP/C framework (if not provided in an env. variable)
ifeq ($(QPC),)
QPC := ../../..
endif

QP_PORT_DIR := $(QPC)/ports/qep-only

VPATH += \
	$(QP_PORT_DIR)

INCLUDES += \
	-I$(QPC)/include \
	-I$(QP_PORT_DIR)

#-----------------------------------------------------------------------------
# QP/C framework sources
#
QP_SRCS := \
	qep_hsm.c \

QS_SRCS := \
	qs.c \
	qs_64bit.c \
	qs_rx.c \
	qs_fp.c \
	qs_port.c

VPATH += \
	$(QPC)/src/qf \
	$(QPC)/src/qs $(QP_PORT_DIR)

INCLUDES += \
	-I$(QPC)/include \
	-I$(QP_PORT_DIR)

# add the QP sources to the build
C_SRCS += $(QP_SRCS)

ifeq (spy, $(CONF))  # Spy configuration
VPATH  += $(QPC)/src/qs
C_SRCS += $(QS_SRCS)
endif

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

#-----------------------------------------------------------------------------
# build configurations...

ifeq (rel, $(CONF)) # Release configuration ..................................

BIN_DIR := build_rel
# gcc options:
CFLAGS := -c -fno-pie -std=c11 -pedantic -Wall -Wextra -W \
	-O3 $(INCLUDES) $(DEFINES) $(DEF) -DNDEBUG

CPPFLAGS := -c -fno-pie -std=c++11 -pedantic -Wall -Wextra \
	-fno-rtti -fno-exceptions \
	-O3 $(INCLUDES) $(DEFINES) $(DEF) -DNDEBUG

else ifeq (spy, $(CONF))  # Spy configuration ................................

BIN_DIR := build_spy

# gcc options:
CFLAGS := -c -g -fno-pie -std=c11 -pedantic -Wall -Wextra -W \
	-O $(INCLUDES) $(DEFINES) $(DEF) -DQ_SPY

CPPFLAGS := -c -g -fno-pie -std=c++11 -pedantic -Wall -Wextra \
	-fno-rtti -fno-exceptions \
	-O $(INCLUDES) $(DEFINES) -DQ_SPY

else # default Debug configuration .........................................

BIN_DIR := build

# gcc options:
CFLAGS := -c -g -fno-pie -std=c11 -pedantic -Wall -Wextra -W \
	-O $(INCLUDES) $(DEFINES) $(DEF)

CPPFLAGS := -c -g -fno-pie -std=c++11 -pedantic -Wall -Wextra \
	-fno-rtti -fno-exceptions \
	-O $(INCLUDES) $(DEFINES) $(DEF)

endif  # .....................................................................

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

.PHONY: clean show

all: $(TARGET_EXE)

ifeq (spy, $(CONF))

$(TARGET_EXE) : $(C_OBJS_EXT) $(CPP_OBJS_EXT)
	$(CC) $(CFLAGS) $(QPC)/src/qs/qstamp.c -o $(BIN_DIR)/qstamp.o
	$(LINK) $(LINKFLAGS) $(LIB_DIRS) -o $@ $^ $(BIN_DIR)/qstamp.o $(LIBS)

else

$(TARGET_EXE) : $(C_OBJS_EXT) $(CPP_OBJS_EXT)
	$(LINK) $(LINKFLAGS) $(LIB_DIRS) -o $@ $^ $(LIBS)

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

clean :
	-$(RM) $(BIN_DIR)/*.*

show :
	@echo PROJECT      = $(PROJECT)
	@echo TARGET_EXE   = $(TARGET_EXE)
	@echo MAKECMDGOALS = $(MAKECMDGOALS)
	@echo VPATH        = $(VPATH)
	@echo INCLUDES     = $(INCLUDES)
	@echo DEFINES      = $(DEFINES)
	@echo DEF          = $(DEF)
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
