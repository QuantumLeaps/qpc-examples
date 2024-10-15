##############################################################################
# Makefile for QUTEST-QP/C on NUCLEO-U545RE-Q, GNU-ARM
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
# make -f nucleo-u545re.mak USB=g: # make, upload to USB drive, run the tests
# make -f nucleo-u545re.mak USB=g: TESTS=philo*.py  # make and run the selected tests
# make -f nucleo-u545re.mak norun   # only make but not run the tests
# make -f nucleo-u545re.mak clean   # cleanup the build
# make -f nucleo-u545re.mak debug   # only run tests in DEBUG mode
#
# NOTE:
# To use this Makefile on Windows, you will need the GNU make utility, which
# is included in the QTools collection for Windows, see:
#    https://github.com/QuantumLeaps/qtools

#-----------------------------------------------------------------------------
# project:
#
PROJECT := test_blinky

VPATH := . \
	../src

INCLUDES := -I. \
	-I../src

C_SRCS := \
	bsp.c \
	blinky.c \
	test_blinky.c

DEFINES  :=

CPP_SRCS :=
LIB_DIRS :=
LIBS     :=
OUTPUT   := $(PROJECT)

#-----------------------------------------------------------------------------
# target
#
TARGET     := nucleo-u545re
3RD_PARTY  := ../../../../3rd_party
TARGET_DIR := $(3RD_PARTY)/$(TARGET)/qutest

VPATH += \
	$(TARGET_DIR) \
	$(3RD_PARTY)/$(TARGET) \
	$(3RD_PARTY)/$(TARGET)/gnu

INCLUDES += \
	-I$(3RD_PARTY)/CMSIS/Include \
	-I$(3RD_PARTY)/$(TARGET)

C_SRCS += \
	system_stm32u5xx.c \
	startup_stm32u545retxq.c

DEFINES += -DQ_SPY -DQ_UTEST \
	-DSTM32U545xx

ARM_CPU   := -mcpu=cortex-m33
ARM_FPU   := -mfpu=fpv5-sp-d16
FLOAT_ABI := -mfloat-abi=hard

LD_SCRIPT := $(TARGET_DIR)/qutest.ld

ifeq ($(USB),)
$(error USB drive not provided for the NUCLEO board.)
endif

BIN_DIR := build_$(TARGET)

FLASH1 = $(CP) $(TARGET_BIN) $(USB)
FLASH2 = $(SLEEP) 4

#-----------------------------------------------------------------------------
# QP framework
#

# location of the QP/C framework (if not provided in an env. variable)
ifeq ($(QPC),)
QPC := ../../../..
endif

# QP port used in this project
QP_PORT_DIR := $(QPC)/ports/arm-cm/qutest


VPATH += \
	$(QPC)/src/qs \
	$(QP_PORT_DIR)

INCLUDES += \
	-I$(QPC)/include \
	-I$(QP_PORT_DIR)

#-----------------------------------------------------------------------------
# QP/C framework sources
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
	$(QPC)/src/qf \
	$(QPC)/src/qs $(QP_PORT_DIR)

INCLUDES += \
	-I$(QPC)/include \
	-I$(QP_PORT_DIR)

# add the QP sources to the build
C_SRCS += $(QP_SRCS)

#-----------------------------------------------------------------------------
# GNU toolset:
# GNU-ARM toolset (NOTE: You need to adjust to your machine)
# see https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
#
ifeq ($(GNU_ARM),)
GNU_ARM := $(QTOOLS)/gnu_arm-none-eabi
endif

# make sure that the GNU-ARM toolset exists...
ifeq ("$(wildcard $(GNU_ARM))","")
$(error GNU_ARM toolset not found. Please adjust the Makefile)
endif

CC    := $(GNU_ARM)/bin/arm-none-eabi-gcc
CPP   := $(GNU_ARM)/bin/arm-none-eabi-g++
AS    := $(GNU_ARM)/bin/arm-none-eabi-as
LINK  := $(GNU_ARM)/bin/arm-none-eabi-gcc
BIN   := $(GNU_ARM)/bin/arm-none-eabi-objcopy

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
CP         := cp
SLEEP      := sleep


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
CFLAGS := -c -g $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb -Wall \
	-ffunction-sections -fdata-sections \
	-O $(INCLUDES) $(DEFINES) $(DEF)

CPPFLAGS := -c -g $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb -Wall \
	-ffunction-sections -fdata-sections -fno-rtti -fno-exceptions \
	-O $(INCLUDES) $(DEFINES) $(DEF)


LINKFLAGS := -T$(LD_SCRIPT) $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb \
	-specs=nosys.specs -specs=nano.specs \
	-Wl,-Map,$(BIN_DIR)/$(OUTPUT).map,--cref,--gc-sections $(LIB_DIRS)

C_OBJS       := $(patsubst %.c,%.o,  $(notdir $(C_SRCS)))
CPP_OBJS     := $(patsubst %.cpp,%.o,$(notdir $(CPP_SRCS)))

TARGET_BIN   := $(BIN_DIR)/$(OUTPUT).bin
TARGET_ELF   := $(BIN_DIR)/$(OUTPUT).elf
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

.PHONY : norun flash debug

# norun target ...............................................................
ifeq ($(MAKECMDGOALS),norun)

norun : $(TARGET_BIN)

$(TARGET_BIN) : $(TARGET_ELF)
	$(BIN) -O binary $< $@

# flash target ..............................................................
else ifeq ($(MAKECMDGOALS),flash)

all : $(TARGET_BIN)

$(TARGET_BIN) : $(TARGET_ELF)
	$(BIN) -O binary $< $@

flash : all
	$(FLASH1)
	$(FLASH2)
	$(QUTEST) -q$(QSPY) -l$(LOG) -o$(OPT) -- $(TESTS)

# default target ..............................................................
else

all : $(TARGET_BIN)
	$(QUTEST) -q$(QSPY) -l$(LOG) -o$(OPT) -- $(TESTS)

$(TARGET_BIN) : $(TARGET_ELF)
	$(BIN) -O binary $< $@
	$(FLASH1)
	$(FLASH2)

endif
#..............................................................................

debug :
	$(QUTEST) -edebug -q$(QSPY) -l$(LOG) -o$(OPT) -- $(TESTS)

$(TARGET_ELF) : $(C_OBJS_EXT) $(CPP_OBJS_EXT)
	$(CC) $(CFLAGS) $(QPC)/src/qs/qstamp.c -o $(BIN_DIR)/qstamp.o
	$(LINK) $(LINKFLAGS) -o $@ $^ $(BIN_DIR)/qstamp.o $(LIBS)

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

.PHONY : clean show

clean :
	-$(RM) $(BIN_DIR)/*.o \
	$(BIN_DIR)/*.d \
	$(BIN_DIR)/*.bin \
	$(BIN_DIR)/*.elf \
	$(BIN_DIR)/*.map

show :
	@echo PROJECT      = $(PROJECT)
	@echo BIN_DIR      = $(BIN_DIR)
	@echo TARGET_ELF   = $(TARGET_ELF)
	@echo MAKECMDGOALS = $(MAKECMDGOALS)
	@echo TESTS        = $(TESTS)
	@echo VPATH        = $(VPATH)
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
