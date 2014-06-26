CC=arc-elf32-gcc
AS=arc-elf32-as
LD=$(CC)
ED=arc-elf32-objdump

include Makefile.os_specific

V=
#V=@

MDB=mdb

COMP_SRCS=test.c setvect.c cachectrl.c leds.c
COMP_SRCS_S=crt0.s

TARGET=irq_test.elf

HWFLAGS=-mav2em -mlittle-endian -mmpy-option=2
HW_FLAGS_MWDT=-av2em

OPTFLAGS=-g -Os
CFLAGS=$(HWFLAGS) $(OPTFLAGS) -I.$(PS)common$(PS)inc -I.$(PS)gpio -I.$(PS)gpio$(PS)src
AFLAGS=$(HWFLAGS) $(OPTFLAGS)

LDFLAGS=$(HWFLAGS) -nostartfiles -Wl,-Map,$(TARGET).map.txt 
LINKER_CMD_FILE=link_arcem4.ln

OBJECTS=$(COMP_SRCS:.c=.o)
OBJECTS_S=$(COMP_SRCS_S:.s=.o)

#
EDFLAGS=-d -S

SIMULATION_SPECIFIC=


all: $(COMP_SRCS) $(COMP_SRCS_S) $(TARGET) 


$(TARGET): $(COMP_SRCS) $(COMP_SRCS_S) $(OBJECTS) $(OBJECTS_S)
	$(V)echo "Link $@"
	$(V)$(LD) -T $(LINKER_CMD_FILE) $(LDFLAGS) -o $@ $(OBJECTS) $(OBJECTS_S) 
	$(V)$(ED) $(EDFLAGS) $@ > $(TARGET).elfdump.txt

%.o: %.s
	$(V)echo "Assembling $<"
	$(V)$(CC) -c $(AFLAGS) $< -o $@

setvect.o: setvect.c
	$(V)echo "Compiling $<"
	$(V)$(CC) -c $(CFLAGS) -mno-volatile-cache $< -o $@


%.o: %.c
	$(V)echo "Compiling $<"
	$(V)$(CC) -c $(CFLAGS) $< -o $@

clean:
	-$(RM) $(OBJECTS) $(OBJECTS_S) $(TARGET).map.txt $(TARGET).elfdump.txt

cleanall: clean
	-$(RM) $(TARGET) 

run:
	$(MDB) -cl -run $(HW_FLAGS_MWDT) $(SIMULATION_SPECIFIC) $(TARGET)

runhw:
	$(MDB) -cl -run -hard -digilent $(TARGET)

debug:
#	$(MDB) $(HW_FLAGS_MWDT) $(SIMULATION_SPECIFIC) $(TARGET)
	$(MDB) -nogoifmain $(HW_FLAGS_MWDT) $(SIMULATION_SPECIFIC) $(TARGET)

debugcl:
	$(MDB) -cl $(HW_FLAGS_MWDT) $(SIMULATION_SPECIFIC) $(TARGET)


debughw:
	$(MDB) -hard -digilent $(TARGET)

