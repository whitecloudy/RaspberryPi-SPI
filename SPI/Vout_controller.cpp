#include "Vout_controller.h"

Vout_controller::Vout_controller(){
  for(int i = 0; i<MAX_channel_num; i++){
    DAC_trim_offset_value[i] = DAC_trim_offset_default;
    DAC_trim_gain_value[i] = DAC_trim_gain_default;
  }
}

Vout_controller::Vout_controller(int calibrated_offset_value[], int calibrated_gain_value[]){
  for(int i = 0; i<MAX_channel_num; i++){
    DAC_trim_offset_value[i] = calibrated_offset_value[i];
    DAC_trim_gain_value[i] = calibrated_gain_value[i];
  }
}

int Vout_controller::serial_word_maker(int mode_bits, int address_function, int data){
  //checking parameters
  if((mode_bits >= 0)&&(mode_bits <= 0x3)
      && ((address_function>=0)&&(address_function<=0x3F))
      && (((mode_bits == 0)&&((data>=0)&&(data<=0xFFFF)))
         || ((data>=0)&&(data<=0x3FFF))))
  {
    if(mode_bits != 0)
      data = (data<<2)&(DATA_MASK0);
    //make buffer
    buffer[0] = ((mode_bits << 6)&(MODE_MASK)) + (address_function & ADDRESS_MASK);
    buffer[1] = (data>>8)&0xFF;
    buffer[2] = data&0xFF;

    return 0;
  }else //if values are wrong
    return 1;
}

int Vout_controller::addres_maker(int vout_num){
  switch(vout_num) {
    case ALL_vout_num:
      return Address_bit_assembly(0, 0);
    case Group0_ALL_vout_num:
      return Address_bit_assembly(0, 1);
    case Group1_ALL_vout_num:
      return Address_bit_assembly(0, 2);
    case Group2_ALL_vout_num:
      return Address_bit_assembly(0, 3);
    case Group3_ALL_vout_num:
      return Address_bit_assembly(0, 4);
    case Group4_ALL_vout_num:
      return Address_bit_assembly(0, 5);
    default:
      return Address_bit_assembly(Group_bit(vout_num), Channel_bit(vout_num));
  }
}

int Vout_controller::offset_modify(offset_values offset_num, int vout_function, int value){

  if((value < 0)||(value > MAX_offset_gain)||(offset_num < DAC0_offset) ||(offset_num > DAC_trim_gain))
    return 1;
  else if(!DAC_offset_switch)
    return 0;

  int mode_bits = 0;

  switch(offset_num){
    case DAC_trim_offset:
      mode_bits = 2; //10
      vout_function = addres_maker(vout_function);
      break;
    case DAC_trim_gain:
      mode_bits = 1; //01
      vout_function = addres_maker(vout_function);
      break;
    case DAC0_offset:
      mode_bits = 0;
      vout_function = 0x2;
      break;
    case DAC1_offset:
      mode_bits = 0;
      vout_function = 0x3;
      break;
    case DAC2_offset:
      mode_bits = 0;
      vout_function = 0x4;
      break;
    default:
      break;
  }

}
