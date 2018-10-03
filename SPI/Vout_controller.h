#include <iostream>
#include "SPI_communicator.h"
#include "GPIO_communicator.h"

#define ANT1_phase 24
#define ANT2_phase 23
#define ANT3_phase 22
#define ANT4_phase 21
#define ANT5_phase 20
#define ANT6_phase 19
#define ANT7_phase 18
#define ANT8_phase 17
#define ANT9_phase 8
#define ANT10_phase 7
#define ANT11_phase 6
#define ANT12_phase 5
#define ANT13_phase 4
#define ANT14_phase 3
#define ANT15_phase 2
#define ANT16_phase 1

#define ANT1_attenuator 32
#define ANT2_attenuator 31
#define ANT3_attenuator 30
#define ANT4_attenuator 29
#define ANT5_attenuator 28
#define ANT6_attenuator 27
#define ANT7_attenuator 26
#define ANT8_attenuator 25
#define ANT9_attenuator 40
#define ANT10_attenuator 39
#define ANT11_attenuator 38
#define ANT12_attenuator 37
#define ANT13_attenuator 36
#define ANT14_attenuator 35
#define ANT15_attenuator 34
#define ANT16_attenuator 33

#define DAC_offset 0x1555
#define DAC_offset_reset false

#define Serial_Word_Size 3
#define MODE_MASK 0xC0
#define ADDRESS_MASK 0x3F
#define DATA_MASK 0xFFFC

const char PHASE[] = {ANT1_phase, ANT2_phase, ANT3_phase, ANT4_phase, ANT5_phase, ANT6_phase, ANT7_phase, ANT8_phase, ANT9_phase, ANT10_phase, ANT11_phase, ANT12_phase, ANT13_phase, ANT14_phase, ANT15_phase, ANT16_phase};
const char ATTENUATOR[] = {ANT1_attenuator, ANT2_attenuator, ANT3_attenuator, ANT4_attenuator, ANT5_attenuator, ANT6_attenuator, ANT7_attenuator, ANT8_attenuator, ANT9_attenuator, ANT10_attenuator, ANT11_attenuator, ANT12_attenuator, ANT13_attenuator, ANT14_attenuator, ANT15_attenuator, ANT16_attenuator};

class Vout_contrller{
  private:
    char buffer[3];
  
};
