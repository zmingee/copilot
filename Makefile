include config.mk

######################
# TARGET AND SOURCES #
######################

TARGET := copilot
BUILD_DIR := build
C_SRCS := $(wildcard src/*.c)
C_OBJS := $(C_SRCS:.c=.o)
TESTS := $(wildcard minunit_tests/*.c)
INCLUDE_DIRS := /usr/local/include
LIBRARY_DIRS := /usr/local/lib
LIBRARIES :=


####################
# BUILD PARAMETERS #
####################

MCU = atmega328
F_CPU = 16000000
BAUD = 9600
BINFORMAT = ihex


##########################
# PROGRAMMING PARAMETERS #
##########################

PROGRAMMER = avrisp
PART = m328p
PORT = $(shell ls /dev/ttyACM? | head -1)
AVRDUDE = avrdude -v -p $(PART) -c $(PROGRAMMER) -P $(PORT)
AVRDUDE_FLASH = -b 19200 -U flash:w:$(BUILD_DIR)/$(TARGET).hex

OPTIMIZATION := -Os
WARNINGS := -Wall -W -Wextra -pedantic

CFLAGS += $(foreach includedir,$(bin_INCLUDE_DIRS),-I$(includedir))
CFLAGS +=  $(WARNINGS) $(OPTIMIZATION) -DF_CPU=$(F_CPU) -mmcu=$(MCU) \
			-DBAUD=$(BAUD) -DVERSION=\"${VERSION}\"
LDFLAGS += $(foreach bindir,$(bin_bin_DIRS),-L$(bindir))
LDFLAGS += $(foreach bin,$(bin_LIBRARIES),-l$(bin))


##################
# BUILD COMMANDS #
##################

.PHONY: all check clean distclean $(TARGET)

all: builddir $(TARGET).hex

$(TARGET).elf: $(C_OBJS)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(TARGET).elf $(C_OBJS)
	$(STRIP) $(BUILD_DIR)/$(TARGET).elf

$(TARGET).hex: $(TARGET).elf
	${OBJCOPY} -O $(BINFORMAT) -R .eeprom $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex
	$(SIZE) -C --mcu=$(MCU) $(BUILD_DIR)/$(TARGET).elf

program: all
	@echo installing hex file to Arduino ATMEGA328P
	$(AVRDUDE) $(AVRDUDE_FLASH)


####################
# TESTS AND CHECKS #
####################

check: $(filter-out src/main.o,$(C_OBJS)) $(TESTS) | builddir remove_optimization
	$(CC) -g $(filter-out src/main.o,$(C_OBJS)) $(TESTS) -o $(BUILD_DIR)/test $(LDFLAGS)
	build/test || echo $?

valgrind: $(filter-out src/main.o,$(C_OBJS)) $(TESTS) | builddir remove_optimization
	$(CC) -g -L/usr/local/lib/ $(filter-out src/main.o,$(C_OBJS)) $(TESTS) -o $(BUILD_DIR)/test
	valgrind --track-origins=yes --suppressions=.valgrind.sup build/test || echo $?

cppcheck:
	cppcheck --inline-suppr --language=c --enable=all --template=gcc bin/ || echo $?


########
# MISC #
########

builddir:
	@mkdir -p build

remove_optimization:
	$(eval CFLAGS := $(filter-out $(bin_OPTIMIZATION), $(CFLAGS)))

clean:
	@- $(RM) $(C_OBJS)
	@- $(RM) -r build
	@- find . -name "build" | xargs $(RM) -rf
	@- find . -name "vgcore*" | xargs $(RM) -rf
	@- find . -name "*.hex" | xargs $(RM) -rf
	@- find . -name "*.elf" | xargs $(RM) -rf
	@- find . -name "*.eeprom" | xargs $(RM) -rf

distclean: clean

define OBJECT_DEPENDS_ON_CORRESPONDING_HEADER
	$(1) : ${1:.o=.h}
endef

$(foreach object_file,$(program_OBJS),$(eval $(call OBJECT_DEPENDS_ON_CORRESPONDING_HEADER,$(object_file))))
