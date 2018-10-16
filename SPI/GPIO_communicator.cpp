#include "GPIO_communicator.h"

GPIO_communicator::GPIO_communicator(int pin_number, int start_status){
  wiringPiSetup();
  pinMode(pin_number, OUTPUT);
  pin_num = pin_number;
  default_status = start_status;
  digitalWrite(pin_num, default_status);
}

int GPIO_communicator::give_signal(int nano_time){
  struct timespec t1;
  t1.tv_sec = 0;
  t1.tv_nsec = nano_time;

  digitalWrite(pin_num, !default_status);
  int result = nanosleep(&t1,NULL);
  digitalWrite(pin_num, default_status);
  result &= nanosleep(&t1,NULL);
  

  return result;
}

GPIO_communicator::~GPIO_communicator(){
  digitalWrite(pin_num, 0);
}
