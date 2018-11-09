#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <wiringPi.h>
#include "Vout_controller.h"
#include "phase_attenuator_controller.h"

#define sleep_microtime 500

int main(int argc, char *argv[]){
  Vout_controller v_con;

  if(argc != 2){
	  std::cout<<"notenough argument"<<std::endl;
	  return 0;
  }

  int antenna_num = std::atoi(argv[1]);
  int attenuator_num = ATTENUATOR[antenna_num];
  int phase_num = PHASE[antenna_num];

  v_con.voltage_modify(ALL_vout_num, 0);
  v_con.voltage_modify(attenuator_num, 1);
  v_con.data_apply();

  int dummy;
  std::cin>>dummy;

  v_con.voltage_modify(attenuator_num, 0);
  v_con.data_apply();
  delayMicroseconds(sleep_microtime);

  v_con.voltage_modify(attenuator_num, 1);
  v_con.data_apply();
  delayMicroseconds(sleep_microtime);

  v_con.voltage_modify(attenuator_num, 0);
  v_con.data_apply();
   delayMicroseconds(sleep_microtime);

  v_con.voltage_modify(attenuator_num, 1);
  v_con.data_apply();
    delayMicroseconds(sleep_microtime);

  for(float voltage = 0.0; voltage <= 10; voltage += 0.05){
    v_con.voltage_modify(phase_num, voltage);
    v_con.data_apply();
    delayMicroseconds(sleep_microtime);
}




  return 0;
}
