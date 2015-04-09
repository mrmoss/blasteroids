CXX=g++
OPTS=-O -std=c++11
CFLAGS=$(OPTS) -Wall
LIBS=-lpthread

all: server

backend: server.cpp
	$(CXX) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	- rm server