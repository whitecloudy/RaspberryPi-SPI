CC = g++
CC_VERSION = -std=c++11
CFFLAGS = -W -WALL
TARGET = spi_com
LINK = -lwiringPi
OBJECTS = sample_main.o SPI_communicator.o

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(LINK) $(CC_VERSION) -o $@ $^

clean : 
	rm *.o $(TARGET)
