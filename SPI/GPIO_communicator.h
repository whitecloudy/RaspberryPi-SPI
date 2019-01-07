#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>

#define GPIO_UP 1
#define GPIO_DOWN 0

class GPIO_communicator{
  private:
    int pin_num;
    int default_status;
  public:
    GPIO_communicator(int pin_number, int start_status);
    int give_signal();
    ~GPIO_communicator();
};
