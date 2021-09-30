##
##

C_OBJS += \
	string.o \
    
AR = ar
AS = arm-none-eabi-gcc
CC 	= arm-none-eabi-gcc
CXX	= arm-none-eabi-g++
LD	= arm-none-eabi-gcc
OBJDUMP = arm-none-eabi-objdump
OBJCOPY = arm-none-eabi-objcopy

CFLAGS += -std=c11 -Wall -Wextra -Werror
CFLAGS += -ffreestanding -march=armv7e-m -mthumb
CFLAGS += -Og -g3 -ggdb
CFLAGS += -fno-stack-protector
CFLAGS += -mthumb -mabi=aapcs -specs=nosys.specs
CFLAGS += -Wall -Werror
CFLAGS += -mfloat-abi=softfp -mfpu=fpv4-sp-d16

LIBRARY = libuc.a

LIBRARY : $(C_OBJS)
	@echo "AR $@"; $(AR) cr $(LIBRARY) $(C_OBJS)
	
.c.o:
	@echo "CC $@"; $(CC) $(CFLAGS) -o $@ -c $<

clean:	
	rm -rf $(C_OBJS)
	rm -rf $(LIBRARY)
