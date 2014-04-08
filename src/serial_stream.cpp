#include <device/stream/serial_stream.hpp>

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

// Function from http://stackoverflow.com/questions/6947413#answer-6947758
// changed speed's type from int to speed_t
// changed parity's type from int to bool
// add should_block argument
int set_interface_attribs(int fd, speed_t speed, SerialStream::ParityMode p, bool should_block) {
  struct termios tty;
  memset(&tty, 0, sizeof(tty));
  if(tcgetattr(fd, &tty) != 0) {
    return -1;
  }
  
  cfsetospeed(&tty, speed);
  cfsetispeed(&tty, speed);
  
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
  tty.c_cflag |= p == SerialStream::PARITY ? PARENB : 0; // this line has been changed
  tty.c_cflag |= p == SerialStream::ODD ? PARODD : 0; // this line has been added
  // originally: tty.c_cflag |= parity;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CRTSCTS;
  
  if(tcsetattr(fd, TCSANOW, &tty) != 0) {
    return -1;
  }
  return 0;
}

SerialStream::SerialStream(const char* path, speed_t baudrate, SerialStream::ParityMode parity)
  : _fd(-1) {
  _fd = open(path, O_RDWR | O_NOCTTY | O_SYNC);
  
  // Could not open
  if(!opened()) {
    return;
  }
  
  // Could not set the interface properly
  if(set_interface_attribs(_fd, baudrate, parity, true) != 0) {
    close(_fd);
    _fd = 0;
    return;
  }
}

SerialStream::~SerialStream(void) {
  if(opened()) {
    close(_fd);
  }
}

bool SerialStream::opened(void) const {
  return _fd != -1;
}

char SerialStream::getValue(void) {
  if(!opened()) {
    return 0;
  }
  char c;
  int unused __attribute__((unused));
  unused = read(_fd, &c, 1);
  return c;
}

void SerialStream::setValue(char c) {
  if(!opened()) {
    return;
  }
  int unused __attribute__((unused));
  unused = write(_fd, &c, 1);
}
