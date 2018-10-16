#include <iostream>
#include <cstdlib>
#include <string>
#include "Vout_controller.h"

int main(int argc, char * argv[]){
  
  if(argc != 4)
    return 1;

  Vout_controller vout;
  std::string offset_name = argv[1];
  int vout_number = atoi(argv[2]);
  int value = atoi(argv[3]);
  offset_types types;

  if(offset_name == "o")
    types = Vout_offset;
  else if(offset_name == "g")
    types = Vout_gain;
  else if(offset_name == "g0")
    types = Group0_offset;
  else if(offset_name == "g1")
    types = Group1_offset;
  else if(offset_name == "g2")
    types = Group234_offset;
  else{
    std::cout << "error!"<<std::endl;
    return 1;
  }
  int modify_result = vout.offset_modify(types, vout_number, value);
  int apply_result = vout.data_apply();
  std::cout << "offset modify : " << modify_result << std::endl;
  std::cout << "data apply : "<< apply_result << std::endl;

  std::cin >> value;

  return 0;
}
