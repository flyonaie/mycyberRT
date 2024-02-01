#include "cyber/transport/shm/shm_transmitter.h"

using apollo::cyber::transport::ShmTransmitter;

int main(int argc, char *argv[]) {
  // init cyber framework
  std::cout << "1234566677" << std::endl;
  printf("INIIIIIIIIIIIIIIIIIIIIII\n");
  auto shm_tx = new ShmTransmitter(1234);
 
  return 0;
}
