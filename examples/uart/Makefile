include ../../common/Makefile.common

NORDIC_COMMON = ../../nordic
	
CXX_OBJS += \
	$(NORDIC_COMMON)/startup/abi/cxa_pure.o \
	$(NORDIC_COMMON)/startup/abi/cxa_operator.o \
	../../drivers/irq_handler.o \
	../../drivers/timer/timer.o \
	../../drivers/uart/uart.o \
	uart.o

ELF_IMAGE = uart.elf

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

flash:
	nrfjprog -f nrf52 --program $(ELF_IMAGE) --sectorerase
	nrfjprog -f nrf52 --reset
