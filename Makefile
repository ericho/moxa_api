
#CC=arm-linux-gcc
CC=gcc
LIBRARY=.
TARGET=moxa_api
CFLAGS=-I$(LIBRARY)
CXXFLAGS=
OBJS=main.o



all: $(OBJS)
	$(CC) *.c -o $(TARGET) -lpthread

serial.o: $(LIBRARY)/serial.c
	$(CC) -c $(LIBRARY)/serial.c

socket.o: $(LIBRARY)/socket.c
	$(CC) -c $(LIBRARY)/socket.c
    
clean:
	rm *.o $(TARGET) > /dev/null 2>&1
