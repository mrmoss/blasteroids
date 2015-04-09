CXX=g++
OPTS=-O
CFLAGS=$(OPTS) -Wall -std=c++11
LIBS=-lpthread

all: database

database: database.cpp msl/json.cpp msl/string.cpp msl/time.cpp msl/webserver.cpp msl/mongoose/mongoose.c msl/jsoncpp/json_reader.cpp msl/jsoncpp/json_tool.h msl/jsoncpp/json_value.cpp msl/jsoncpp/json_writer.cpp
	$(CXX) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	- rm -f database
