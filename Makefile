CXX = g++
CXXCFLAGS = `pkg-config --cflags opencv` -std=c++11
CXXLFLAGS = `pkg-config --libs opencv`
CC = gcc
INCLUDE = -I./inc/

TARGET_SEND = ./src/send
TARGET_RECV = ./src/recv

OBJS = ./src/bpf.o ./src/rawsock.o ./src/lib.o ./src/etherimg.o

TARGETS = $(TARGET_SEND) $(TARGET_RECV)

all: $(TARGETS)

.c.o:
	$(CC) $(INCLUDE) $< -c -o $@

.cpp.o:
	$(CXX) $(CXXCFLAGS) $(INCLUDE) $< -c -o $@

$(TARGET_SEND): $(TARGET_SEND).o $(OBJS)
	$(CXX) $(TARGET_SEND).o $(CXXLFLAGS) $(INCLUDE) $(OBJS) -o $@

$(TARGET_RECV): $(TARGET_RECV).o $(OBJS)
	$(CXX) $(TARGET_RECV).o $(CXXLFLAGS) $(INCLUDE) $(OBJS) -o $@

clean:
	rm $(OBJS) $(TARGETS)
	rm $(TARGET_SEND).o $(TARGET_RECV).o
