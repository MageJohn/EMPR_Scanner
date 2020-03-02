#!/usr/bin/make -f

# Ensure consistent environment
SHELL := /bin/bash
PATH := /opt/york/cs/net/bin:$(PATH)

# Path to the GCC toolbox
PKG := /opt/york/cs/net

# Specify the commands needed from the tool chain, CC is the C-compiler,
# OBJCOPY converts the resulting program binary into a format we can load
# into the MBED board 
ARCH := arm-none-eabi
CC := $(ARCH)-gcc
OBJCOPY := $(ARCH)-objcopy

# make install - Installs the resulting binary file to the MBED board, remember
# to sync the file systems, so the copy finishes
# need to know user to install into correct directory
USER := $(shell whoami)

SOURCERY := $(PKG)/sourcery-g++-lite-arm-eabi-2010.09.51-i686-1
GNU_VERSION := 4.5.0
THUMB2GNULIB := $(SOURCERY)/$(ARCH)/lib/$(GNU_VERSION)/thumb2
THUMB2GNULIB2 := $(SOURCERY)/$(ARCH)/lib/thumb2

# "Cortex Microcontroller Software Interface Standard" Startup files, also the
# flags passed to the C compiler, and linker
CMSIS := $(PKG)/lpc1700-cmsis-lite-2011.01.26-i686-1
CMSISINCLUDES := -I$(CMSIS)/include
CMSISFL := $(CMSIS)/lib/core_cm3.o \
	$(CMSIS)/lib/system_LPC17xx.o \
	$(CMSIS)/lib/startup_LPC17xx.o
LDSCRIPT := $(CMSIS)/lib/ldscript_rom_gnu.ld

CFLAGS := -mcpu=cortex-m3  -mthumb  -Wall  -O0  -mapcs-frame  -D__thumb2__=1 \
  -msoft-float  -gdwarf-2  -mno-sched-prolog  -fno-hosted  -mtune=cortex-m3 \
  -march=armv7-m  -mfix-cortex-m3-ldrd   -ffunction-sections  -fdata-sections \
          -D__RAM_MODE__=0 $(CMSISINCLUDES) -I. -I./lib

LDFLAGS := $(CMSISFL) -static -mcpu=cortex-m3 -mthumb -mthumb-interwork \
	   -Wl,--start-group -L$(THUMB2GNULIB) -L$(THUMB2GNULIB2) \
           -lc -lg -lstdc++ -lsupc++  -lgcc -lm  -Wl,--end-group \
	   -Xlinker -Map -Xlinker bin/lpc1700.map -Xlinker -T $(LDSCRIPT)

LDFLAGS += -L$(CMSIS)/lib -lDriversLPC17xxgnu

CFLAGS += -MMD

ROOT_DIR := $(shell pwd)
BIN_DIR := $(ROOT_DIR)/bin
SRC_DIR := $(ROOT_DIR)/src
LIB_DIR := $(ROOT_DIR)/lib
PROGRAMS := $(basename $(notdir $(wildcard $(SRC_DIR)/*.c)))
OBJ_FILES := $(patsubst %.c,%.o,$(shell find -type f -name *.c -printf "%f "))


#VPATH := $(BIN_DIR) $(SRC_DIR) $(LIB_DIR) $(wildcard $(LIB_DIR)/*)
vpath %.c $(SRC_DIR) $(LIB_DIR) $(shell find $(LIB_DIR)/* -type d -printf "%p ")
vpath %.h $(SRC_DIR) $(LIB_DIR) $(shell find $(LIB_DIR)/* -type d -printf "%p ")
vpath %.o $(BIN_DIR)
vpath %.a $(BIN_DIR)
vpath % $(BIN_DIR)

platform_deps := platform.a time.o ioboard.a leds.a
stepper_test_deps := time.o ioboard.a leds.o
motors_test_deps := $(platform_deps)
stop_motors_deps := ioboard.a
struct_test_deps := serial.o
led_mux_test_deps := leds.a time.o
rgb_sensor_test_deps := ioboard.a time.o serial.o
raster_scan_B2_deps := $(platform_deps) serial.o
motor_patterns_deps := $(platform_deps) serial.o
lib_sensor_test_deps := $(platform_deps) serial.o
lcd_test_deps := $(platform_deps) platform_lcd.a serial.o
flag_detect_deps := $(platform_deps) serial.o
draw_mbed_deps := $(platform_deps) serial.o
C1_deps := $(platform_deps) platform_lcd.a serial.o

STATIC_LIBS := 

.PHONY: clean default $(addsuffix .install,$(PROGRAMS))

# Commands handled by this makefile
default: $(PROGRAMS)

$(OBJ_FILES): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $(BIN_DIR)/$(notdir $@)

$(addsuffix .install,$(PROGRAMS)): %.install: %
	@echo "Attempting to copy $(BIN_DIR)/$*.bin to the MBED file system"
	@echo "If successful, press the reset button on the MBED to load the new binary"
	cp $(BIN_DIR)/$*.bin /media/$(USER)/MBED &
	sync

bin:
	mkdir bin

-include $(wildcard $(BIN_DIR)/*.d)

# make clean - Clean out the source tree ready to re-build the project
clean:
	rm -f bin/*

.INTERMEDIATE: mp2_demo.elf %.o

.SECONDEXPANSION:
$(PROGRAMS): %: bin %.elf
	$(OBJCOPY) -I elf32-little -O binary $(BIN_DIR)/$@.elf $(BIN_DIR)/$@.bin

$(addsuffix .elf,$(PROGRAMS)): %.elf: %.o $$($$*_deps)
	$(CC) -o $(BIN_DIR)/$@ $(addprefix $(BIN_DIR)/,$(notdir $^)) $(LDFLAGS)

pc := %
%.a: $$(notdir $$(patsubst $$(pc).c,$$(pc).o,$$(wildcard $(LIB_DIR)/%/*.c)))
	ar rcs $(BIN_DIR)/$@ $(addprefix $(BIN_DIR)/,$(notdir $^))
