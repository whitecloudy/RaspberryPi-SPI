#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <wiringPi.h>
#include "Vout_controller.h"
#include "phase_attenuator_controller.h"

#define sleep_microtime 500
#define DEBUG_MODE true

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

  float voltage = 0.1;
  int dummy;
  std::cin>>dummy;


  if(!DEBUG_MODE){
    v_con.voltage_modify(attenuator_num, 0);
    v_con.data_apply();
    delayMicroseconds(sleep_microtime);

    v_con.voltage_modify(attenuator_num, 1);
    v_con.data_apply();
    delayMicroseconds(sleep_microtime);

    v_con.voltage_modify(attenuator_num, 0);
    v_con.data_apply();
    delayMicroseconds(sleep_microtime);

    for(; voltage <= 10; voltage += 0.1){
      v_con.voltage_modify(phase_num, voltage);
      v_con.data_apply();
      delayMicroseconds(sleep_microtime);
    }
    v_con.voltage_modify(phase_num, 0);
    v_con.data_apply();
    delayMicroseconds(sleep_microtime);
  }else{
    v_con.voltage_modify(phase_num, 5);
    v_con.data_apply();
    delayMicroseconds(250);

    v_con.voltage_modify(phase_num, 0);
    v_con.data_apply();
    delayMicroseconds(250);

    v_con.voltage_modify(phase_num, 5);
    v_con.data_apply();
    delayMicroseconds(500);

    v_con.voltage_modify(phase_num, 0);
    v_con.data_apply();
    delayMicroseconds(500);

    v_con.voltage_modify(phase_num, 5);
    v_con.data_apply();
    delayMicroseconds(1000);

    v_con.voltage_modify(phase_num, 0);
    v_con.data_apply();
    delayMicroseconds(1000);



  }



  return 0;
}
