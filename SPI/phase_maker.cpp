#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <wiringPi.h>
#include "Vout_controller.h"
#include "phase_attenuator_controller.h"

#define DEBUG_MODE false

int main(int argc, char *argv[]){
  Vout_controller v_con;

  if(argc != 3){
    std::cout<<"notenough argument"<<std::endl;
    return 0;
  }

  int antenna_num = std::atoi(argv[1]);
  int sleep_microtime = std::atoi(argv[2]);

  int attenuator_num = ATTENUATOR[antenna_num];
  int phase_num = PHASE[antenna_num];
  int target_num_p, target_num_a;

  target_num_p = phase_num;
  target_num_a = attenuator_num;


  v_con.voltage_modify(target_num_p, 0);
  v_con.voltage_modify(target_num_a, 5);
  v_con.data_apply();

  float voltage = 0.1;
  int dummy;
  std::cin>>dummy;


  if(!DEBUG_MODE){
    v_con.voltage_modify(target_num_a, 10);
    v_con.data_apply();
    delayMicroseconds(sleep_microtime);

    v_con.voltage_modify(target_num_a, 5);
    v_con.data_apply();
    delayMicroseconds(sleep_microtime);

    v_con.voltage_modify(target_num_a, 10);
    v_con.data_apply();
    delayMicroseconds(sleep_microtime);

    v_con.voltage_modify(target_num_a, 5);
    v_con.data_apply();
    delayMicroseconds(sleep_microtime);

    for(; voltage <= 10; voltage += 0.1){
      v_con.voltage_modify(target_num_p, voltage);
      v_con.data_apply();
      delayMicroseconds(sleep_microtime);
    }

    v_con.voltage_modify(target_num_p, 0);
    v_con.data_apply();
    delayMicroseconds(sleep_microtime);

    for(voltage = 5.1; voltage <= 10; voltage += 0.1){
      v_con.voltage_modify(target_num_a, voltage);
      v_con.data_apply();
      delayMicroseconds(sleep_microtime);
    }

    v_con.voltage_modify(target_num_a, 5);
    v_con.data_apply();
    delayMicroseconds(sleep_microtime);

    v_con.voltage_modify(target_num_a, 10);
    v_con.data_apply();
    delayMicroseconds(sleep_microtime);

    v_con.voltage_modify(target_num_a, 5);
    v_con.data_apply();
    delayMicroseconds(sleep_microtime);



  }else{
    v_con.voltage_modify(target_num_a, 5);
    v_con.data_apply();
    delayMicroseconds(250);

    v_con.voltage_modify(target_num_a, 0);
    v_con.data_apply();
    delayMicroseconds(250);

    v_con.voltage_modify(target_num_a, 5);
    v_con.data_apply();
    delayMicroseconds(500);

    v_con.voltage_modify(target_num_a, 0);
    v_con.data_apply();
    delayMicroseconds(500);

    v_con.voltage_modify(target_num_a, 5);
    v_con.data_apply();
    delayMicroseconds(1000);

    v_con.voltage_modify(target_num_a, 0);
    v_con.data_apply();
    delayMicroseconds(1000);



  } 
  std::cout <<"done"<<std::endl;
  std::cin >> dummy;
  return 0;
}
