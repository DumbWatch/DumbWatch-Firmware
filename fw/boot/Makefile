#
# Makefile for DumbWatch Bootloader
#

NORDIC_COMMON = ../../nordic

AS_OBJS = \
	$(NORDIC_COMMON)/startup/gcc_startup_nrf52.o
	
CXX_OBJS = \
	$(NORDIC_COMMON)/startup/startup_nrf52.o \
	main.o

AS = arm-none-eabi-gcc
CC 	= arm-none-eabi-gcc
CXX	= arm-none-eabi-g++
LD	= arm-none-eabi-gcc
OBJDUMP = arm-none-eabi-objdump
OBJCOPY = arm-none-eabi-objcopy

CXXFLAGS += -std=c++17 -Wall -Wextra -Wold-style-cast -Wconversion -Werror
CXXFLAGS += -ffreestanding -march=armv7e-m -mthumb
CXXFLAGS += -Og -g3 -ggdb
CXXFLAGS += -fno-exceptions -fno-rtti -fno-stack-protector
CXXFLAGS += -I../nordic/include
CXXFLAGS += -I./
CXXFLAGS += -DBOOTLOADER

ASFLAGS += -march=armv7e-m

LDFLAGS += -T nrf52_xxaa.ld -nostdlib -L$(NORDIC_COMMON)/ld

ELF_IMAGE = boot.elf

ELF_IMAGE : $(CXX_OBJS) $(AS_OBJS)
	@echo "LINK $@"; $(LD) $(LDFLAGS) $(CXX_OBJS) $(AS_OBJS) -o $(ELF_IMAGE)
	
.cpp.o:
	@echo "CXX $@"; $(CXX) $(CXXFLAGS) -o $@ -c $<

.S.o:
	@echo "AS $@"; $(AS) $(ASFLAGS) -o $@ -c $<
	
clean:
	rm -rf $(CXX_OBJS)
	rm -rf $(AS_OBJS)
	rm -rf $(ELF_IMAGE)