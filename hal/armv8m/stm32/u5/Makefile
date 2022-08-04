#
# Makefile for Phoenix-RTOS loader (ARMv7M HAL stm32l4x6)
#
# Copyright 2021 Phoenix Systems
#
# %LICENSE%
#

LDFLAGS:=$(filter-out -Tbss% , $(LDFLAGS))
LDFLAGS:=$(filter-out -Tdata% , $(LDFLAGS))

CFLAGS:=$(filter-out -mfloat-abi% , $(CFLAGS))
CFLAGS+= -mfloat-abi=soft

GCCLIB := $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)

PLO_COMMANDS = alias app call console copy dump echo go help kernelimg map phfs reboot script wait

include devices/uart-stm32u5/Makefile
include devices/flash-stm32/Makefile

OBJS += $(addprefix $(PREFIX_O)hal/$(TARGET_SUFF)/stm32/u5/, _init.o stm32u5.o timer.o console.o)
