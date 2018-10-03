#include <iostream>
#include "SPI_communicator.h"

int main(void){
	int result;
	unsigned char buffer[100];
  SPI_communicator comm1(0, 100000, SPI_DOWN_EDGE);
	buffer[0] = 0xCC;
	buffer[1] = 0xCC;

  result = comm1.transmit(buffer, 2);

	std::cout<<result<<std::endl;
	std::cout<<(int)buffer[0]<<" "<<(int)buffer[1]<<std::endl;
	return 0;
}
