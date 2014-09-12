CC=g++

AVERSIVE_ROOT=../../../aversive--

CFLAGS=-Wall -Wextra --std=c++11 -I$(AVERSIVE_ROOT)/include/sasiae -I../../include
LDFLAGS= -L$(AVERSIVE_ROOT)/build -L../../build -laversive_sasiae -laversive_serial

EXEC=tester

all: $(EXEC)

$(EXEC): main.cpp ../../build/libaversive_serial.a
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f *~ \#*\# *.o

mrproper: clean
	rm -f $(EXEC)
