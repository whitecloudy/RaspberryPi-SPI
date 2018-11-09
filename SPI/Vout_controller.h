#ifndef __VOUT_CONTROLLER__
#define __VOUT_CONTROLLER__

#include <iostream>
#include "SPI_communicator.h"
#include "GPIO_communicator.h"

#define MAX_offset_gain 0x3FFF
#define Vout_offset_default  0x2000
#define Vout_gain_default    0x3FFF
#define Group0_offset_default 0x1555
#define Group1_offset_default 0x1555
#define Group234_offset_default 0x1555
#define DAC_offset_switch true

#define Serial_Word_Size 3
#define MODE_MASK 0xC0
#define ADDRESS_MASK 0x3F
#define DATA_MASK0 0xFFFC

#define MAX_channel_num 40
#define ALL_vout_num -1
#define Group0_ALL_vout_num -2
#define Group1_ALL_vout_num -3
#define Group2_ALL_vout_num -4
#define Group3_ALL_vout_num -5
#define Group4_ALL_vout_num -6

#define Group_bit(vout_num) ((vout_num/8)+1)
#define Channel_bit(vout_num) (vout_num%8)
#define Address_bit_assembly(group, channel) (((group<<3)+channel)&0x3F)

#define LDAC_pin 5
#define SYNC_pin 6

#define SPI_speed 10000

#define FILE_name "offset_saved_file.csv"

enum offset_types {Group0_offset = 0, Group1_offset, Group234_offset, Vout_offset, Vout_gain};


class Vout_controller{
  private:
    SPI_communicator spi_comm;
    GPIO_communicator ldac, sync;
    unsigned char buffer[Serial_Word_Size]= {0};
    int Vout_offset_value[MAX_channel_num] = {0};
    int Vout_gain_value[MAX_channel_num] = {0};
    
    int Group0_offset_value = Group0_offset_default;
    int Group1_offset_value = Group1_offset_default;
    int Group234_offset_value = Group234_offset_default;
    
  private:
    int serial_word_maker(int mode_bits, int address_function, int data);
    int addres_maker(int vout_num);
    int data_sender();
    int offset_data_reader();
    int offset_data_writer();

  public:
    Vout_controller();
    Vout_controller(int calibrated_offset_value[], int calibrated_gain_value[]);
    ~Vout_controller();
    int offset_modify(offset_types offset_num, int vout_function, int value);
    int offset_refresh();
    int voltage_modify(int vout_num, float voltage);
    int voltage_modify_bin(int vout_num, int code);
    int data_apply();
    int print_all_offset();
};

#endif
