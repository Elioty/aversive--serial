#include <aversive.hpp>
#include <device/stream/uart_stream.hpp>

int main(int argc, char** argv) {
  (void) argc;
  (void) argv;
  
  Aversive::init();
  
  UartStream<0> io("io");
  
  u8 i = 0;
  while(Aversive::sync()) {
    i = io.getValue();
    io.setValue(i);
  }
  
  return Aversive::exit();
}
