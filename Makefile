AVERSIVE_ROOT=../aversive--

CC=g++
CFLAG=--std=c++11 -Wall -Wextra -Iinclude -I$(AVERSIVE_ROOT)/include/common -O3

OBJ=binary_stream_read binary_stream_write formatted_stream_read formatted_stream_write stream serial_stream
LIB=libaversive_serial.a

all: build/$(LIB)

build/$(LIB): $(addprefix build/,$(addsuffix .o,$(OBJ)))
	ar r $@ $^

build/serial_stream.o: src/serial_stream.cpp
	$(CC) $(CFLAG) -c $< -o $@

build/binary_stream_read.o: $(AVERSIVE_ROOT)/src/common/device/stream/binary_stream_read.cpp
	$(CC) $(CFLAG) -c $< -o $@

build/binary_stream_write.o: $(AVERSIVE_ROOT)/src/common/device/stream/binary_stream_write.cpp
	$(CC) $(CFLAG) -c $< -o $@

build/formatted_stream_read.o: $(AVERSIVE_ROOT)/src/common/device/stream/formatted_stream_read.cpp
	$(CC) $(CFLAG) -c $< -o $@

build/formatted_stream_write.o: $(AVERSIVE_ROOT)/src/common/device/stream/formatted_stream_write.cpp
	$(CC) $(CFLAG) -c $< -o $@

build/stream.o: $(AVERSIVE_ROOT)/src/common/device/stream/stream.cpp
	$(CC) $(CFLAG) -c $< -o $@

clean:
	@rm -f $(shell find . -name "*~" -o -name "#*#" -o -name "*.o")

mrproper: clean
	@rm -f build/$(LIB)
