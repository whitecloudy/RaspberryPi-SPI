#include <iostream>
#include <cstdlib>
#include "Vout_controller.h"

int main(int argc, char * argv[]){
  
  if(argc < 3){
    std::cout<<"not enough arguments"<<std::endl;
    return 1;
  }
  int vout_num = atoi(argv[1]);
  float voltage = atof(argv[2]);


  Vout_controller v1;

  v1.voltage_modify(vout_num, voltage);
  v1.data_apply();


	return 0;
}
