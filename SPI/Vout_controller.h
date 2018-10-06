#include <iostream>
#include "SPI_communicator.h"
#include "GPIO_communicator.h"

#define ANT1_phase 23
#define ANT2_phase 22
#define ANT3_phase 21
#define ANT4_phase 20
#define ANT5_phase 19
#define ANT6_phase 18
#define ANT7_phase 17
#define ANT8_phase 16
#define ANT9_phase 7
#define ANT10_phase 6
#define ANT11_phase 5
#define ANT12_phase 4
#define ANT13_phase 3
#define ANT14_phase 2
#define ANT15_phase 1
#define ANT16_phase 0

#define ANT1_attenuator 31
#define ANT2_attenuator 30
#define ANT3_attenuator 29
#define ANT4_attenuator 28
#define ANT5_attenuator 27
#define ANT6_attenuator 26
#define ANT7_attenuator 25
#define ANT8_attenuator 24
#define ANT9_attenuator 39
#define ANT10_attenuator 38
#define ANT11_attenuator 37
#define ANT12_attenuator 36
#define ANT13_attenuator 35
#define ANT14_attenuator 34
#define ANT15_attenuator 33
#define ANT16_attenuator 32

#define MAX_offset_gain 0xCFFF
#define DAC_trim_offset_default  0x2000
#define DAC_trim_gain_default    0x3FFF
#define DAC0_offset_default 0x1555
#define DAC1_offset_default 0x1555
#define DAC2_offset_default 0x1555
#define DAC_offset_switch false

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
#define SPI_speed 20000

enum offset_values {DAC0_offset = 0, DAC1_offset, DAC2_offset, DAC_trim_offset, DAC_trim_gain};


const char PHASE[] = {ANT1_phase, ANT2_phase, ANT3_phase, ANT4_phase, ANT5_phase, ANT6_phase, ANT7_phase, ANT8_phase, ANT9_phase, ANT10_phase, ANT11_phase, ANT12_phase, ANT13_phase, ANT14_phase, ANT15_phase, ANT16_phase};
const char ATTENUATOR[] = {ANT1_attenuator, ANT2_attenuator, ANT3_attenuator, ANT4_attenuator, ANT5_attenuator, ANT6_attenuator, ANT7_attenuator, ANT8_attenuator, ANT9_attenuator, ANT10_attenuator, ANT11_attenuator, ANT12_attenuator, ANT13_attenuator, ANT14_attenuator, ANT15_attenuator, ANT16_attenuator};

class Vout_controller{
  private:
    SPI_communicator spi_comm;
    GPIO_communicator ldac, sync;
    unsigned char buffer[Serial_Word_Size]= {0};
    int DAC_trim_offset_value[MAX_channel_num] = {0};
    int DAC_trim_gain_value[MAX_channel_num] = {0};
    
    int DAC0_offset_value = DAC0_offset_default;
    int DAC1_offset_value = DAC1_offset_default;
    int DAC2_offset_value = DAC2_offset_default;
    
  private:
    int serial_word_maker(int mode_bits, int address_function, int data);
    int addres_maker(int vout_num);

    int data_sender();

  public:
    Vout_controller();
    Vout_controller(int calibrated_offset_value[], int calibrated_gain_value[]);
    int offset_modify(offset_values offset_num, int vout_function, int value);
    int voltage_modify(int vout_num, float voltage);
    int data_apply();

};
