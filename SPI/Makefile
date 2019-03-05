CC = g++
CC_VERSION = -std=c++11
CFLAGS = -W -Wall -g
TARGET = $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4)
TARGET1 = phase_maker
TARGET2 = voltage_control
TARGET3 = phase_control
TARGET4 = beamforming
LINK = -lwiringPi -lpthread
OBJECTS = SPI_communicator.o GPIO_communicator.o Vout_controller.o Phase_Attenuator_controller.o

all : $(TARGET) 

$(TARGET1) : $(TARGET1).o $(OBJECTS)
	$(CC) $(CFLAGS) $(LINK) $(CC_VERSION) -o $@ $^

$(TARGET2) : $(TARGET2).o $(OBJECTS)
	$(CC) $(CFLAGS) $(LINK) $(CC_VERSION) -o $@ $^

$(TARGET3) : $(TARGET3).o $(OBJECTS)
	$(CC) $(CFLAGS) $(LINK) $(CC_VERSION) -o $@ $^

$(TARGET4) : $(TARGET4).o $(OBJECTS)
	$(CC) $(CFLAGS) $(LINK) $(CC_VERSION) -o $@ $^


clean : 
	rm *.o $(TARGET)
