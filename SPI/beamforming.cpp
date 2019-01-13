#include <iostream>
#include "Phase_Attenuator_controller.h"
#include <cstdlib>
#include <ctime>

#define BEAMFORMING true

int main(int argc, char ** argv) {
  int ant_amount = argc - 1;
  int * ant_nums = new int[ant_amount];
  Phase_Attenuator_controller ctrl;

  for(int i = 1; i <= ant_amount; i++){
    ant_nums[i-1] = atoi(argv[i]);
    ctrl.ant_off(ant_nums[i-1]);
  }

  ctrl.data_apply();

  //do beamforming
  if(BEAMFORMING){
    //first antenna doesn't need phase shift
    ctrl.phase_control(ant_nums[0],0);
    for(int i = 1; i<ant_amount; i++){
      std::cout<<ant_nums[i]<<" antenna"<<std::endl;

      int phase;
      //change phase 10 degree step
      for(phase = 0; phase<360;l phase+=10){
        ctrl.phase_control(ant_nums[i], phase);
        ctrl.data_apply();
        std::cout<<phase<<" degree"<<std::endl;
        std::cin.get();    
      }

      //set best phase
      std::cout<<"insert best phase"<<std::endl;
      std::cin>>phase;
      ctrl.phase_control(ant_nums[i], phase);
      ctrl.data_apply();
    }
  }
  //do random phase multi antenna
  else{
    srand(time(NULL));

    for(int i = 0; i<ant_mount; i++){
       std::cout<<ant_nums[i]<<" antenna"<<std::endl;
       phase = rand() % 360;
       ctrl.phase_control(ant_nums[i], phase);
       ctrl.data_apply();
       std::cout<<phase<<" degree"<<std::endl;
    }
  }

  std::cin.get();

  delete(ant_nums);
  return 0;
}
