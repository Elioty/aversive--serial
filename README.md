Aversive++Serial
================

Aversive++ serial communication library for Linux-based OS.

Dependencies
============

You need Aversive++ to compile this library.
https://github.com/astralien3000/aversive--

Your Aversive++ local copy must in the same directory as your Aversive++Serial local copy.
Otherwise, you might want to change AVERSIVE_ROOT variable in the makefile.

Compilation
===========

Just type: make

How to use
==========

Include device/stream/serial_stream.hpp.

Declare a SerialStream object with the following constructor:
SerialStream(const char* path, speed_t baudrate, ParityMode parity)

Parameters explaination:
- path is the path to the device, e.g. /dev/ttyACM0;
- baudrate, see "man termios", "Line speed" paragraph for possible values,
  (default value is B9600);
- parity, possible values: SerialStream::NONE, SerialStream::PARITY and SerialStream::ODD,
  (default value is SerialStream::NONE).

Call opened method on your object to be sure it was initialized correctly (returns true if so).

And then, you can use your object as you would use any other Stream under Aversive++
(so please refer to Stream's documentation for further details).

To compile your code, add the following parameters:
-Ipath_to_aversive++/include/common -Ipath_to_aversive++serial/include

To link against the library, add the following parameters:
-Lpath_to_aversive++serial/build -laversive_serial
