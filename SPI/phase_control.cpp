#include <iostream>
#include <cstdlib>
#include "Phase_Attenuator_controller.h"

int main(int argc, char **argv){
  int ant = 0;
  float phase = 0;
  Phase_Attenuator_controller ctrl(0);
 
  int dummy = 0;
  ant = atoi(argv[1]);

  for(int i = 0; i<360; i+=30){
    std::cout<<"phase = "<<i<<std::endl;

    ctrl.phase_control(ant,i);
    ctrl.data_apply();

    std::cin>>dummy;
  }
  

  return 0;
}
