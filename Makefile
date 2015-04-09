CXX=g++
OPTS=-O
CFLAGS=$(OPTS) -Wall -std=c++11
LIBS=-lpthread

all: server

server: server.cpp msl/time.cpp msl/webserver.cpp msl/mongoose/mongoose.c
	$(CXX) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	- rm server
