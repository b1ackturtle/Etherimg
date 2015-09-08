CXX = g++
CXXCFLAGS = `pkg-config --cflags opencv` -std=c++11
CXXLFLAGS = `pkg-config --libs opencv`
CC = gcc
INCLUDE = -I./inc/

OBJ_SEND = ./src/send.o
OBJ_RECV = ./src/recv.o

TARGET_SEND = ./send
TARGET_RECV = ./recv

OBJS = ./src/bpf.o ./src/rawsock.o ./src/lib.o ./src/etherimg.o

TARGETS = $(TARGET_SEND) $(TARGET_RECV)

all: $(TARGETS)

.c.o:
	$(CC) $(INCLUDE) $< -c -o $@

.cpp.o:
	$(CXX) $(CXXCFLAGS) $(INCLUDE) $< -c -o $@

$(TARGET_SEND): $(OBJ_SEND) $(OBJS)
	$(CXX) $(OBJ_SEND) $(CXXLFLAGS) $(INCLUDE) $(OBJS) -o $@

$(TARGET_RECV): $(OBJ_RECV) $(OBJS)
	$(CXX) $(OBJ_RECV) $(CXXLFLAGS) $(INCLUDE) $(OBJS) -o $@

clean:
	rm $(OBJS) $(TARGETS)
	rm $(OBJ_SEND) $(OBJ_RECV)
