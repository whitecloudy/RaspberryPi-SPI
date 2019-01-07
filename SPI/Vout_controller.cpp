#include "Vout_controller.h"
#include <fstream>
#include <time.h>
#include "csv/csv.h"
#include <string>

Vout_controller::Vout_controller() : spi_comm(0, SPI_speed, SPI_DOWN_EDGE), ldac(LDAC_pin, GPIO_UP), sync(SYNC_pin, GPIO_DOWN){
  if(offset_data_reader())
    std::cout<<"Error : file input output crashed"<<std::endl;
  if(offset_refresh())
    std::cout <<"Initialize Failed"<<std::endl;
}

Vout_controller::Vout_controller(int calibrated_offset_value[], int calibrated_gain_value[]) : spi_comm(0, SPI_speed, SPI_DOWN_EDGE), ldac(LDAC_pin, GPIO_UP), sync(SYNC_pin, GPIO_DOWN){
  for(int i = 0; i<MAX_channel_num; i++){
    Vout_offset_value[i] = calibrated_offset_value[i];
    Vout_gain_value[i] = calibrated_gain_value[i];
  }
  if(offset_refresh())
    std::cout <<"Initialize Failed"<<std::endl;
}


Vout_controller::~Vout_controller(){
  if(voltage_modify(ALL_vout_num,0))
    std::cout << "voltage 0V error!"<<std::endl;
  if(data_apply())
    std::cout << "voltage 0V apply error!"<<std::endl;
  if(offset_data_writer())
    std::cout << "Offset saving error!"<<std::endl;
}


int Vout_controller::offset_data_reader(){
  std::fstream file_checker(FILE_name,std::fstream::in);
  if(file_checker.is_open()){
    file_checker.close();
    io::CSVReader<3> file(FILE_name);
    file.read_header(io::ignore_extra_column, "type","number", "value");
    std::string type; int num, value;
    while(file.read_row(type, num, value)){
      if(type == "Group"){
        switch(num){
          case 0:
            Group0_offset_value = value;
            break;
          case 1:
            Group1_offset_value = value;
            break;
          case 2:
            Group234_offset_value = value;
            break;
          default:
            std::cout<<"Group offset number error"<<std::endl;
            return 1;
        }
      }else if(type == "Offset"){
        if((num >=0) && (num< MAX_channel_num))
          Vout_offset_value[num] = value;
        else{
          std::cout<<"Offset number error"<<std::endl;
          return 1;
        }
      }else if(type == "Gain"){
        if((num >=0) && (num< MAX_channel_num))
          Vout_gain_value[num] = value;
        else{
          std::cout<<"Offset number error"<<std::endl;
          return 1;
        }
      }else{
        std::cout<<"file read error"<<std::endl;
        return 1;
      }
    }
    std::cout<<"Offset Saved file load complete"<<std::endl;
  }else{
    file_checker.close();
    std::cout<<"No offset file found"<<std::endl;
    for(int i = 0; i<MAX_channel_num; i++){
      Vout_offset_value[i] = Vout_offset_default;
      Vout_gain_value[i] = Vout_gain_default;
    }
    offset_data_writer();
  }
  return 0;
}

int Vout_controller::offset_data_writer(){
  std::fstream file(FILE_name, std::fstream::out);
  file<<"type"<<","<<"number"<<","<<"value"<<std::endl;

  file<<"Group"<<","<<0<<","<<Group0_offset_value<<std::endl;
  file<<"Group"<<","<<1<<","<<Group1_offset_value<<std::endl;
  file<<"Group"<<","<<2<<","<<Group234_offset_value<<std::endl;

  for(int i = 0; i<MAX_channel_num; i++){
    file<<"Offset"<<","<<i<<","<<Vout_offset_value[i]<<std::endl;
    file<<"Gain"<<","<<i<<","<<Vout_gain_value[i]<<std::endl;
  }

  file.close();
  std::cout<<"Offset save complete"<<std::endl;

  return 0;
}

int Vout_controller::offset_refresh(){
  int result = 0;
  result = result||offset_modify(Group0_offset,0, Group0_offset_value);
  result = result||offset_modify(Group1_offset,0, Group1_offset_value);
  result = result||offset_modify(Group234_offset,0, Group234_offset_value);

  for(int i = 0;i<MAX_channel_num;i++){
    result = result||offset_modify(Vout_offset, i, Vout_offset_value[i]);
    result = result||offset_modify(Vout_gain, i, Vout_gain_value[i]);
  }

  result = result||data_apply();

  return result;
}

int Vout_controller::serial_word_maker(int mode_bits, int address_function, int data){
  //checking parameters
  if(((address_function>=0)&&(address_function<=0x3F))
      && ((((mode_bits>0)&&(mode_bits<=0x3)) && ((data>=0)&&(data<=0x3FFF)))
        ||((mode_bits==0)&&((data>=0)&&(data<=0xFFFF))))
    )
  {
    if(mode_bits != 0)
      data = (data<<2)&(DATA_MASK0);
    //make buffer
    buffer[0] = ((mode_bits << 6)&(MODE_MASK)) + (address_function & ADDRESS_MASK);
    buffer[1] = (data>>8)&0xFF;
    buffer[2] = data&0xFF;

    return 0;
  }else //if values are wrong
  {
    std::cout<<"serial word making error!"<<std::endl;
    std::cout<<mode_bits<<" "<<address_function<<" "<<data<<std::endl;
    return 1;
  }
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

int Vout_controller::data_sender(){
  sync.give_signal();
  int result = spi_comm.transmit(buffer, Serial_Word_Size);
  return result;
}

int Vout_controller::data_apply() {
  sync.give_signal();
  int result = ldac.give_signal();
  return result;
}


int Vout_controller::offset_modify(offset_types offset_num, int vout_function, int value){

  if((value < 0)||(value > MAX_offset_gain)||(offset_num < Group0_offset) ||(offset_num > Vout_gain))
    return 1;
  else if(!DAC_offset_switch)
    return 0;

  int mode_bits = 0;

  switch(offset_num){
    case Vout_offset:
      mode_bits = 2; //10
      Vout_offset_value[vout_function] = value;
      vout_function = addres_maker(vout_function);
      break;
    case Vout_gain:
      mode_bits = 1; //01
      Vout_gain_value[vout_function] = value;
      vout_function = addres_maker(vout_function);
      break;
    case Group0_offset:
      mode_bits = 0;
      vout_function = 0x2;
      Group0_offset_value = value;
      break;
    case Group1_offset:
      mode_bits = 0;
      vout_function = 0x3;
      Group1_offset_value = value;
      break;
    case Group234_offset:
      mode_bits = 0;
      vout_function = 0x4;
      Group234_offset_value = value;
      break;
    default:
      return 1;
  }

  serial_word_maker(mode_bits, vout_function, value);
  return data_sender();
}

int Vout_controller::voltage_modify_bin(int vout_num, int code){
  int mode_bits = 3;
  
  serial_word_maker(mode_bits, addres_maker(vout_num), code);
  int result = data_sender();
  if(result)
    std::cout<<"data send error : "<<result<<std::endl;

  return result;

}

int Vout_controller::voltage_modify(int vout_num, float voltage){
  int offset_code;
  switch(Group_bit(vout_num)){
    case 1:
      offset_code = Group0_offset_value;
      break;
    case 2:
      offset_code = Group1_offset_value;
      break;
    case 3:
    case 4:
    case 5:
      offset_code = Group234_offset_value;
      break;
    default:
      std::cout<<"wrong group bit"<<std::endl;
      return 1;
  }

  int dac_value = voltage*16384/12 + offset_code;

  if(dac_value > 0x3FFF)
    dac_value = 0x3FFF;
  else if(dac_value < 0)
    dac_value = 0;

  dac_value = dac_value & MAX_offset_gain;

  return voltage_modify_bin(vout_num, dac_value);
}

int Vout_controller::print_all_offset(){
  std::cout<<"Group_offset"<<std::endl<<Group0_offset_value<<" "<<Group1_offset_value<<" "<<Group234_offset_value<<std::endl<<std::endl;
  for(int i = 0;i<MAX_channel_num;i++){
    std::cout<<"Vout number "<<i<<std::endl;
    std::cout<<Vout_offset_value[i]<<" "<<Vout_gain_value[i]<<std::endl;
  }
  return 0;
}
