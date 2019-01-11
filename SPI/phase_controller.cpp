#include <iostream>
#include "Phase_Attenuator_controller.h"

int main(void){
  int ant = 0, double phase = 0.0;
  Phase_Attenuator_controller ctrl(0);
  
   while(true){
    std::cout << "Ant : "<<std::endl;
    std::cin >> ant;
    if(ant = -1)
      break;
    std::cout << "Phase : "<<std::endl;
    std::cin >> phase;

    ctrl.phase_control(ant, phase);    
    ctrl.data_apply();
  }

  return 0;
}
