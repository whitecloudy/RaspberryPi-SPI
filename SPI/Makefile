CC = g++
CC_VERSION = -std=c++11
CFLAGS = -W -Wall -O0
TARGET = spi_com
LINK = -lwiringPi
OBJECTS = sample_main.o SPI_communicator.o GPIO_communicator.o Vout_controller.o

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(LINK) $(CC_VERSION) -o $@ $^

clean : 
	rm *.o $(TARGET)
