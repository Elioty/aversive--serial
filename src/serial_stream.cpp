#include <device/stream/serial_stream.hpp>

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

// Function initially from from http://stackoverflow.com/questions/6947413#answer-6947758
bool setInterfaceAttr(int fd, speed_t speed, SerialStream::ParityMode p, bool should_block) {
  termios tty;
  memset(&tty, 0, sizeof(tty));
  
  if(tcgetattr(fd, &tty) < 0) {
    return false;
  }
  
  if(cfsetospeed(&tty, speed) < 0) {
    return false;
  }
  
  if(cfsetispeed(&tty, speed) < 0) {
    return false;
  }
  
  tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
  // disable IGNBRK for mismatched speed tests; otherwise receive break
  // as \000 chars
  tty.c_iflag &= ~IGNBRK; // ignore break signal
  tty.c_lflag = 0; // no signaling chars, no echo,
  // no canonical processing
  tty.c_oflag = 0; // no remapping, no delays
  tty.c_cc[VMIN] = should_block ? 1 : 0; // this line has been changed
  tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout
  
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
  
  tty.c_cflag |= (CLOCAL | CREAD); // ignore modem controls,
  // enable reading
  tty.c_cflag &= ~(PARENB | PARODD); // shut off parity
  tty.c_cflag |= (p == SerialStream::PARITY || p == SerialStream::ODD) ? PARENB : 0; // this line has been changed
  tty.c_cflag |= p == SerialStream::ODD ? PARODD : 0; // this line has been added
  // originally: tty.c_cflag |= parity;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CRTSCTS;
  
  if(tcsetattr(fd, TCSANOW, &tty) < 0) {
    return false;
  }
  
  return true;
}

SerialStream::SerialStream(const char* path, speed_t baudrate, SerialStream::ParityMode parity)
  : _fd(INVALID_FD) {
  _fd = open(path, O_RDWR | O_NOCTTY | O_SYNC);
  
  // Could not open
  if(!opened()) {
    return;
  }
  
  // Is not a tty
  if(!isatty(_fd)) {
    close(_fd);
    _fd = INVALID_FD;
    return;
  }

  // Could not set the interface properly
  if(!setInterfaceAttr(_fd, baudrate, parity, true)) {
    close(_fd);
    _fd = INVALID_FD;
    return;
  }
}

SerialStream::~SerialStream(void) {
  if(opened()) {
    close(_fd);
    _fd = INVALID_FD;
  }
}

bool SerialStream::opened(void) const {
  return _fd != INVALID_FD;
}

void SerialStream::flush(void) const {
  if(opened()) {
    tcdrain(_fd);
  }
}

char SerialStream::getValue(void) {
  if(!opened()) {
    return 0;
  }
  
  char c;
  while(read(_fd, &c, 1) == 0) {
  }
  return c;
}

void SerialStream::setValue(char c) {
  if(!opened()) {
    return;
  }
  
  while(write(_fd, &c, 1) == 0) {
  }
}
