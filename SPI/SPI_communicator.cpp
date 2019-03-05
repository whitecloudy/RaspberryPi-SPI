#include "SPI_communicator.h"
#include <cstdio>

#define DEBUG false

SPI_communicator::SPI_communicator(int channel, int kHz_speed, int mode){
  std::cout<<"SPI communicator initialize"<<std::endl;
  spi_fd = wiringPiSPISetupMode(channel, kHz_speed*1000, mode);
}

SPI_communicator::~SPI_communicator(){
  close(spi_fd);
}

int SPI_communicator::transmit(const unsigned char * buffer, int size){
  int result = write(spi_fd, buffer, size);

  if(DEBUG)
    std::cout<<(int)buffer[0]<<" "<<(int)buffer[1]<<" "<<(int)buffer[2]<<std::endl;
  if(result != size)
    return 1;
  return 0;
}

int SPI_communicator::transmit(std::string buffer){
  int result = write(spi_fd, buffer.c_str(), buffer.length());

  if(result != (int)buffer.length())
    return 1;
  return 0;
}


