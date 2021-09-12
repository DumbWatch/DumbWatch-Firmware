include ../../common/Makefile.common

NORDIC_COMMON = ../../nordic

AS_OBJS = \
	$(NORDIC_COMMON)/startup/gcc_startup_nrf52.o
	
CXX_OBJS = \
	$(NORDIC_COMMON)/startup/startup_nrf52.o \
	blinky.o

ELF_IMAGE = blinky.elf

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