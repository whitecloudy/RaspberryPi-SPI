#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>

#define LDAC 6
#define SYNC 5

class GPIO_communicator{
  private:
    int pin_num;
    int default_status;
  public:
    GPIO_communicator(int pin_number, int start_status);
    int give_signal(int nano_time);
    ~GPIO_communicator();
};
