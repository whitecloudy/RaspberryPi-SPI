#include "Phase_Attenuator_controller.h"
#include <string>
#include <cmath>

int Phase_Attenuator_controller::load_cal_data(void){
  //loading the calibration data
  for(int i = 0; i<16; i++){
    std::string filename = "calibration_data/ant" + std::to_string(i) + "_cal_data";
    io::CSVReader<4> csv_reader(filename);

    float ph_V, po_V, phase, power;
    int j = 0;
    while(csv_reader.read_row(phase, power, ph_V, po_V)){
      V_preset[i][j].phase = phase;
      V_preset[i][j].power = power;
      V_preset[i][j].ph_V = ph_V;
      V_preset[i][j].po_V = po_V;
    }
  }

  return 0;
 
 
}

inline int preset_finder(struct cal_ref * V_preset, int start, int end, float phase){
  int middle = (start + end)/2;

  if((V_preset[middle].phase < phase) && (V_preset[middle+1].phase >= phase)){
    if((phase - V_preset[middle].phase) > (V_preset[middle+1].phase - phase))
      return middle+1;
    else
      return middle;
  }

  if(V_preset[middle].phase <= phase)
    return preset_finder(V_preset, start, middle, phase);
  else
    return preset_finder(V_preset, middle+1, end, phase);
  
}

int Phase_Attenuator_controller::find_matched_preset(int ant, float phase){
  return preset_finder(V_preset[ant], 0, CAL_data_length-1, phase);
}

int Phase_Attenuator_controller::set_integer_index(void){
  for(int ant = 0; ant < ANT_num; ant++){
    for(int i = 0; i < 360; i++){
      index_V_preset[ant][i] = find_matched_preset(ant, i);
    }
  }
}

int Phase_Attenuator_controller::voltage_index_search(int ant, int phase){
  return index_V_preset[ant][phase%360];
}

int Phase_Attenuator_controller::voltage_index_search(int ant, float phase){
  while(phase > 360)
    phase -= 360.0;
  while(phase < 0)
    phase += 360.0;

  return find_matched_preset(ant, phase);
}

int Phase_Attenuator_controller::phase_setup(int ant, int index){
  int at_result = V.voltage_modify(ATTENUATOR[ant], V_preset[ant][index].po_V);
  int ph_result = V.voltage_modify(PHASE[ant], V_preset[ant][index].ph_V);

  return at_result&&ph_result;
}

int Phase_Attenuator_controller::phase_control(int ant, int phase){
  int index = voltage_index_search(ant, phase);

  return phase_setup(ant, index);
}

int Phase_Attenuator_controller::phase_control(int ant, float phase){
  int index = voltage_index_search(ant, phase);

  return phase_setup(ant, index);
}

int Phase_Attenuator_controller::data_apply(void){
  return V.data_apply();
}


Phase_Attenuator_controller::Phase_Attenuator_controller(void){
  if(load_cal_data())
    std::cout<<"Error : Loading calibration data failed"<<std::endl;
  set_integer_index();
}

Phase_Attenuator_controller::Phase_Attenuator_controller(int phase){
  if(load_cal_data())
    std::cout<<"Error : Loading calibration data failed"<<std::endl;
  set_integer_index();

  int result = 0;
  for(int i = 0; i<ANT_num; i++){
    result = result || phase_control(i, phase);
  }
  result = result || data_apply();

  if(result)
    std::cout<<"Error : Phase initalize error"<<std::endl;
}

Phase_Attenuator_controller::Phase_Attenuator_controller(float phase){
  if(load_cal_data())
    std::cout<<"Error : Loading calibration data failed"<<std::endl;
  set_integer_index();

  int result = 0;
  for(int i = 0; i<ANT_num; i++){
    result = result || phase_control(i, phase);
  }
  result = result || data_apply();

  if(result)
    std::cout<<"Error : Phase initalize error"<<std::endl;
}
