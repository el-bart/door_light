PROGRAM:=$(notdir $(CURDIR))
OUT:=$(PROGRAM).out
SRC:=$(wildcard *.cpp *.c)
MCU_TYPE:=attiny13

OBJ:=$(SRC:.c=.o)
OBJ:=$(OBJ:.cpp=.o)
HEX_FILE:=$(PROGRAM).hex
BIN_FILE:=$(PROGRAM).bin

CC:=avr-gcc
CXX:=avr-g++
OBJ_CP:=avr-objcopy

CFLAGS_COMMON:=-Wall -mmcu=$(MCU_TYPE)
CFLAGS:=$(CFLAGS_COMMON) -std=c99
CXXFLAGS:=$(CFLAGS_COMMON) -std=c++11
LDFLAGS:=

.SECONDARY:

.PHONY: all
all: release

.PHONY: debug
debug: CFLAGS+=-g3
debug: CXXFLAGS+=-g3
debug: hex bin

.PHONY: release
release: CFLAGS+=-Os -DNDEBUG
release: CXXFLAGS+=-Os -DNDEBUG
release: hex bin

.PHONY: hex
hex: $(HEX_FILE)

.PHONY: bin
bin: $(BIN_FILE)

$(HEX_FILE): $(OUT)
	$(OBJ_CP) -O ihex $< $@

$(BIN_FILE): $(OUT)
	$(OBJ_CP) -O binary $< $@

$(OUT): $(OBJ)
	$(CXX) $(LDFLAGS) $(OBJ) -o $@


.PHONY: flash_mcu
flash_mcu:
	#uisp -dprog=stk200 --erase --upload if=$(HEX_FILE) --verify
	avrdude -P avrdoper -c stk500v2 -p t13 -U flash:w:$(PROGRAM).bin

.PHONY: clean
clean:
	rm -fv $(OUT) $(BIN_FILE) $(HEX_FILE) $(OBJ)
