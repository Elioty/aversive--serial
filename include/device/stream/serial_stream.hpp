#ifndef SERIAL_STREAM_HPP
#define SERIAL_STREAM_HPP

#include <device/stream/stream.hpp>
#include <termios.h>

class SerialStream : public Stream {
  int _fd;

  static constexpr int INVALID_FD = -1;
  
public:
  enum ParityMode { NONE, PARITY, ODD };
  
  SerialStream(const char* path, speed_t baudrate = B9600, ParityMode parity = NONE);
  
  virtual ~SerialStream(void);
  
  bool opened(void) const;
  
  void flush(void) const;
  
  char getValue(void);
  
  void setValue(char c);
};

#endif//SERIAL_STREAM_HPP
