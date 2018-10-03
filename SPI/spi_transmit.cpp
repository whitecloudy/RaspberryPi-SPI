#include <iostream>
#include <errno.h>
#include <wiringPiSPI.h>
#include <unistd.h>

#define CHANNEL 0

int main(void){
	int fd, result;
	unsigned char buffer[100];
	buffer[0] = 0xCC;
	buffer[1] = 0xCC;

	fd = wiringPiSPISetupMode(CHANNEL, 100000, 1);

	result = write(fd, buffer, 2);

	std::cout<<result<<std::endl;
	std::cout<<(int)buffer[0]<<" "<<(int)buffer[1]<<std::endl;
	return 0;
}
