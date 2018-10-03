#include "SPI_communicator.h"

SPI_communicator::SPI_communicator(int channel, int kHz_speed, int mode){
  spi_fd = wiringPiSPISetupMode(channel, kHz_speed, mode);
}

SPI_communicator::~SPI_communicator(){
  close(spi_fd);
}

int SPI_communicator::transmit(const unsigned char * buffer, int size){
  return write(spi_fd, buffer, size);
}

int SPI_communicator::transmit(std::string buffer){
  return write(spi_fd, buffer.c_str(), buffer.length());
}


