#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include <device/stream/serial_stream.hpp>

int main(int argc, char** argv) {
  (void) argc;
  (void) argv;
  
  SerialStream io("/dev/ttyACM0");
  
  if(!io.opened()) {
    std::cerr << "Stream could not be properly opened" << std::endl;
    return EXIT_FAILURE;
  }
  
  // Leave some time for the board to boot.
  sleep(1);
  
  u8 r;
  for(u8 i = 0; i < 255; i++) {
    io.setValue(i);
    r = io.getValue();
    if(r != i) {
      std::cerr << "Read " << (int) r << " although " << (int) i << " was expected" << std::endl;
      return EXIT_FAILURE;
    }
  }
  
  std::cout << "OK" << std::endl;
  return EXIT_SUCCESS;
}
