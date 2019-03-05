#include "GPIO_communicator.h"

GPIO_communicator::GPIO_communicator(int pin_number, int start_status){
  std::cout <<"GPIO communicator intializer"<<std::endl;
  wiringPiSetup();
  pinMode(pin_number, OUTPUT);
  pin_num = pin_number;
  default_status = start_status;
  digitalWrite(pin_num, default_status);
}

int GPIO_communicator::give_signal(){

  digitalWrite(pin_num, !default_status);
  delayMicroseconds(1);
  digitalWrite(pin_num, default_status);
  delayMicroseconds(1);
  

  return 0;
}

GPIO_communicator::~GPIO_communicator(){
  digitalWrite(pin_num, 0);
}
