include ../../../common/Makefile.common

NORDIC_COMMON = ../../../nordic
FREERTOS_PATH = ../../../third_party/freertos

AS_OBJS = \
	$(NORDIC_COMMON)/startup/gcc_startup_nrf52.o
	
C_OBJS += \
	$(FREERTOS_PATH)/source/croutine.o \
	$(FREERTOS_PATH)/source/event_groups.o \
	$(FREERTOS_PATH)/source/portable/MemMang/heap_1.o \
	$(FREERTOS_PATH)/source/list.o \
	$(FREERTOS_PATH)/portable/GCC/nrf52/port.o \
	$(FREERTOS_PATH)/portable/CMSIS/nrf52/port_cmsis.o \
	$(FREERTOS_PATH)/portable/CMSIS/nrf52/port_cmsis_systick.o \
	$(FREERTOS_PATH)/source/queue.o \
	$(FREERTOS_PATH)/source/stream_buffer.o \
	$(FREERTOS_PATH)/source/tasks.o \
	$(FREERTOS_PATH)/source/timers.o \
	
CXX_OBJS += \
	$(NORDIC_COMMON)/startup/abi/cxa_pure.o \
	$(NORDIC_COMMON)/startup/abi/cxa_operator.o \
	../../../drivers/irq_handler.o \
	../../../drivers/uart/uart.o \
	hello_world.o
	
CXXFLAGS += -I../hello_world
CXXFLAGS += -I$(FREERTOS_PATH)/source/include
CXXFLAGS += -I$(FREERTOS_PATH)/portable/GCC/nrf52
CXXFLAGS += -I$(FREERTOS_PATH)/portable/CMSIS/nrf52

CFLAGS += -I../hello_world
CFLAGS += -I$(FREERTOS_PATH)/source/include
CFLAGS += -I$(FREERTOS_PATH)/portable/GCC/nrf52
CFLAGS += -I$(FREERTOS_PATH)/portable/CMSIS/nrf52

ELF_IMAGE = hello_world.elf

ELF_IMAGE : $(C_OBJS) $(CXX_OBJS) $(AS_OBJS)
	@echo "LINK $@"; $(LD) $(LDFLAGS) $(CXX_OBJS) $(C_OBJS) $(AS_OBJS) -o $(ELF_IMAGE)
	
.cpp.o:
	@echo "CXX $@"; $(CXX) $(CXXFLAGS) -o $@ -c $<
	
.c.o:
	@echo "CC $@"; $(CC) $(CFLAGS) -o $@ -c $<
	
.S.o:
	@echo "AS $@"; $(AS) $(ASFLAGS) -o $@ -c $<
	
clean:
	rm -rf $(CXX_OBJS)
	rm -rf $(C_OBJS)
	rm -rf $(AS_OBJS)
	rm -rf $(ELF_IMAGE)

flash:
	nrfjprog -f nrf52 --program $(ELF_IMAGE) --sectorerase
	nrfjprog -f nrf52 --reset
