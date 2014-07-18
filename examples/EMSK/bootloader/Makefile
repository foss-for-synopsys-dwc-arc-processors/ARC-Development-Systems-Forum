CC=arc-elf32-gcc
AS=arc-elf32-as
LD=$(CC)
ED=arc-elf32-objdump

include Makefile.os_specific

V=
#V=@

COMP_SRCS_S=crt0.s

TARGET=bootloader.elf

HWFLAGS=-mav2em -mlittle-endian
HW_FLAGS_MWDT=-av2em

OPTFLAGS=-g -Os
CFLAGS=$(HWFLAGS) $(OPTFLAGS)
AFLAGS=$(HWFLAGS) $(OPTFLAGS)

LDFLAGS=$(HWFLAGS) -nostartfiles -Wl,-Map,$(TARGET).map.txt
LINKER_CMD_FILE=link_arcem6.ln

OBJECTS=$(COMP_SRCS:.c=.o)
OBJECTS_S=$(COMP_SRCS_S:.s=.o)

#
EDFLAGS=-d -S

all: $(COMP_SRCS) $(COMP_SRCS_S) $(TARGET)


$(TARGET): $(COMP_SRCS) $(COMP_SRCS_S) $(OBJECTS) $(OBJECTS_S)
	$(V)echo "Link $@"
	$(V)$(LD) -T $(LINKER_CMD_FILE) $(LDFLAGS) -o $@ $(OBJECTS) $(OBJECTS_S)
	$(V)$(ED) $(EDFLAGS) $@ > $(TARGET).elfdump.txt

%.o: %.s
	$(V)echo "Assembling $<"
	$(V)$(CC) -c $(AFLAGS) $< -o $@

%.o: %.c
	$(V)echo "Compiling $<"
	$(V)$(CC) -c $(CFLAGS) $< -o $@

clean:
	-$(RM) $(OBJECTS) $(OBJECTS_S) $(TARGET).map.txt $(TARGET).elfdump.txt

cleanall: clean
	-$(RM) $(TARGET)

