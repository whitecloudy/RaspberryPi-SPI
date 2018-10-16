CC = g++
CC_VERSION = -std=c++11
CFLAGS = -W -Wall -O0
TARGET = $(TARGET1)
TARGET1 = offset_compensator
TARGET1_obj = offset_compensator.o
LINK = -lwiringPi -lpthread
OBJECTS = SPI_communicator.o GPIO_communicator.o Vout_controller.o

all : $(TARGET)

$(TARGET1) : $(TARGET1_obj) $(OBJECTS)
	$(CC) $(CFLAGS) $(LINK) $(CC_VERSION) -o $@ $^

clean : 
	rm *.o $(TARGET)
