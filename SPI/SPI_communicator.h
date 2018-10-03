#include <iostream>
#include <errno.h>
#include <wiringPiSPI.h>
#include <unistd.h>
#include <string>

#define CHANNEL 0

#define SPI_DOWN_EDGE 1
#define SPI_UP_EDGE 0

class SPI_communicator{
  private:
    int spi_fd = 0;
  public:
    SPI_communicator(int channel, int kHz_speed, int mode);
    int transmit(const unsigned char * buffer, int size);
    int transmit(std::string buffer);
    ~SPI_communicator();
};
