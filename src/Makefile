
#CC=arm-linux-gcc
CC=gcc
LIBRARY=.
TARGET=moxa_api
CFLAGS=-I$(LIBRARY)
CXXFLAGS=
OBJS=main.o
GTEST_DIR=/home/erich/src/gtest/

CPPFLAGS += -isystem $(GTEST_DIR)/include
CXXFLAGS += -g -Wall -Wextra -pthread

TESTS = serial_dongle_api

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
		$(GTEST_DIR)/include/gtest/internal/*.h


# Stuff need for gtest compilation

# gtest-all.o : $(GTEST_SRCS_)
#          $(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
#             $(GTEST_DIR)/src/gtest-all.cc

# gtest_main.o : $(GTEST_SRCS_)
#         $(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
#             $(GTEST_DIR)/src/gtest_main.cc

# gtest.a : gtest-all.o
#         $(AR) $(ARFLAGS) $@ $^

# gtest_main.a : gtest-all.o gtest_main.o
#         $(AR) $(ARFLAGS) $@ $^


# Makefile targets

all: $(OBJS)
	$(CC) -g *.c -o $(TARGET) -lpthread -DDEBUG

serial.o: $(LIBRARY)/serial.c
	$(CC) -c $(LIBRARY)/serial.c

socket.o: $(LIBRARY)/socket.c
	$(CC) -c $(LIBRARY)/socket.c

# tests: $(TESTS)

# serial_dongle_api: serial_dongle.o serial_dongle_api.o gtest_main.a

clean:
	rm *.o $(TARGET) > /dev/null 2>&1
