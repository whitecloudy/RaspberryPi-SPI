#include "Phase_Attenuator_controller.h"
#include <string>
#include <cmath>


/* Loading Calibration Data */
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
      j++;
    }
  }

  return 0;
 
 
}

/* 
 *  Recursive function to find the best phase fit voltage setting of phase shifter and attenuator
 */
int preset_finder(struct cal_ref * V_preset, int start, int end, float phase){
  
  //index right in the middle of start and end
  int middle_index = (start + end)/2;

  
  /*
   * minus : one minus from middle index
   * center : middle index
   * plus : one plus from middle index
   */
  float minus, center, plus;                  //phase
  int minus_index, center_index, plus_index;  //index

  //when preset finder reaches minimum index end
  if(middle_index==0){      
    minus_index = CAL_data_length-1;
    center_index = 0;
    plus_index = 1;
    minus = V_preset[minus_index].phase -360;
    center = V_preset[center_index].phase;
    plus = V_preset[plus_index].phase;
  }
  //when preset finder reaches maximum index end
  else if(middle_index==CAL_data_length-1){      
    minus_index = CAL_data_length-2;
    center_index = CAL_data_length -1;
    plus_index = 0;
    minus = V_preset[minus_index].phase;
    center = V_preset[center_index].phase;
    plus = V_preset[plus_index].phase + 360;
  }
  //otherwise(preset finder doesn't reaches any end)
  else{
    minus_index = middle_index-1;
    center_index = middle_index;
    plus_index = middle_index+1;
    minus = V_preset[minus_index].phase;
    center = V_preset[center_index].phase;
    plus = V_preset[plus_index].phase;
  }


  //if phase is in the middle of center and center+1 index
  if((center < phase) && (plus >= phase)){
    if((phase - center) > (plus - phase))
      return plus_index;
    else
      return center_index;
  }
  //if phase is in the middle of center-1 and center index
  else if((center >= phase) && (minus < phase)){
    if((phase - minus) > (center - phase))
      return center_index;
    else
      return minus_index;
  }

  //more recursive if we don't return at above
  if(V_preset[middle_index].phase > phase)
    return preset_finder(V_preset, start, middle_index, phase);
  else
    return preset_finder(V_preset, middle_index+1, end, phase);
}

/* 
 *  recursive function starter
 */
int Phase_Attenuator_controller::find_matched_preset(int ant, float phase){
  return preset_finder(V_preset[ant], 0, CAL_data_length-1, phase);
}

/*
 *  Find every integer phase values's index before start(to search faster)
 */
int Phase_Attenuator_controller::set_integer_index(void){
  for(int ant = 0; ant < ANT_num; ant++){
    for(int i = 0; i < 360; i++){
      index_V_preset[ant][i] = find_matched_preset(ant, i);
    }
  }
}

/*
 *  if user want integer phase's voltage preset
 */
int Phase_Attenuator_controller::voltage_index_search(int ant, int phase){
  return index_V_preset[ant][phase%360];
}

/*
 *  if user want float phase's voltage preset
 */
int Phase_Attenuator_controller::voltage_index_search(int ant, float phase){
  while(phase > 360)
    phase -= 360.0;
  while(phase < 0)
    phase += 360.0;

  return find_matched_preset(ant, phase);
}

/*
 *  phase setup by index
 */
int Phase_Attenuator_controller::phase_setup(int ant, int index){
  int at_result = V.voltage_modify(ATTENUATOR[ant], V_preset[ant][index].po_V);
  int ph_result = V.voltage_modify(PHASE[ant], V_preset[ant][index].ph_V);

  return at_result&&ph_result;
}

/*
 *  phase control with integer phase value
 */
int Phase_Attenuator_controller::phase_control(int ant, int phase){
  int index = voltage_index_search(ant, phase);

  return phase_setup(ant, index);
}

/*
 *  phase control with float phase value
 */
int Phase_Attenuator_controller::phase_control(int ant, float phase){
  int index = voltage_index_search(ant, phase);

  return phase_setup(ant, index);
}

/*
 *  Apply phase shifter and attenuator voltage setting
 */
int Phase_Attenuator_controller::data_apply(void){
  return V.data_apply();
}

/*
 *  Turn off antenna(actually we just maximise attenuate value)
 */
int Phase_Attenuator_controller::ant_off(int ant_num){
  return V.voltage_modify(ATTENUATOR[ant_num], 0);
}


/*
 * -----------------initializer----------------------------
 * if there is no parameter we just load calibration data and intialize every other class
 *
 * if there is a parameter we initialize every antenna phase with the parameter value.
 */
Phase_Attenuator_controller::Phase_Attenuator_controller(void){
  std::cout << "Phase Attenuator controller intialized" <<std::endl;
  if(load_cal_data())
    std::cout<<"Error : Loading calibration data failed"<<std::endl;
  set_integer_index();
}

Phase_Attenuator_controller::Phase_Attenuator_controller(int phase){
  std::cout << "Phase Attenuator controller intialized" <<std::endl;
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
  std::cout << "Phase Attenuator controller intialized" <<std::endl;
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
