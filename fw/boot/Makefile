#
# Makefile for DumbWatch Bootloader
#

AS_OBJS = \
	dummy.s
	
CXX_OBJS = \
	dummy.cpp
	
C_OBJS = \
	dummy.c

CC 	= arm-none-eabi-gcc
CXX	= arm-none-eabi-g++
LD	= arm-none-eabi-gcc
OBJDUMP = arm-none-eabi-objdump
OBJCOPY = arm-none-eabi-objcopy

CFLAGS += -std=c11 -Wall -Wextra
CFLAGS += -ffreestanding -march=armv7e -mcpu=cortex-m4 -mthumb
CXXFLAGS += -std=c++17 -Wall -Wextra -Wold-style-cast -Wconversion -Werror
CXXFLAGS += -ffreestanding -march=armv7e -mcpu=cortex-m4 -mthumb
CXXFKAGS += -Og -g3 -ggdb
CXXFLAGS += -fno-exceptions -fno-rtti -fno-stack-protector
CXXFLAGS += -I../nordic/include
CXXFLAGS += -I./
CXXFLAGS += -DBOOTLOADER

ELF_IMAGE = boot.elf

ELF_IMAGE : $(CXX_OBJS) $(C_OBJS) $(AS_OBJS)
	@echo "LINK $@"; $(LD) $(LDFLAGS) $(CXX_OBJS) $(C_OBJS) $(AS_OBJS) -o $(ELF_IMAGE)
	
.cpp.o:
	@echo "CXX $@"; $(CXX) $(CXXFLAGS) -o $@ -c $<
	
.c.o:
	@echo "CC $@"; $(CC) $(CFLAGS) -o $@ -c $<

.s.o:
	@echo "AS $@"; $(CC) $(ASFLAGS) -o $@ -c $<
	
clean:
	rm -rf $(CXX_OBJS)
	rm -rf $(C_OBJS)
	rm -rf $(AS_OBJS)
	rm -rf $(ELF_IMAGE)