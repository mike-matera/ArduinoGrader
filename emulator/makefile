CXXFLAGS = -std=gnu++11 -Iarduino -Iemu -fpermissive -Wall
CFLAGS = -Iarduino -Iemu -fpermissive -Wall
LDFLAGS = -lpthread $(shell pkg-config --libs protobuf grpc++)

# Local sources
C_FILES += $(wildcard *.c)
CPP_FILES += $(wildcard *.cpp)

# Arduino sources
C_FILES += $(wildcard arduino/*.c)
CPP_FILES += $(wildcard arduino/*.cpp)

# Emulator sources
C_FILES += $(wildcard emu/*.c)
CPP_FILES += $(wildcard emu/*.cpp)

# Protobuf files.
PROTO_CC = proto/emulator.pb.cc proto/emulator.grpc.pb.cc
PROTO_OBJ = $(PROTO_CC:.cc=.o)

OBJS := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o) $(PROTO_OBJ)

all: test

test: $(PROTO_CC) $(OBJS)
	g++ -g -o test $(OBJS) $(LDFLAGS)

proto: emulator/proto/emulator.pb.cc emulator.grpc.pb.cc

proto/emulator.pb.cc: ./proto/emulator.proto
	cd proto && protoc emulator.proto --cpp_out=.

proto/emulator.grpc.pb.cc: ./proto/emulator.proto
	cd proto && protoc emulator.proto --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin`

clean:
	rm *.o test

.PHONY: clean
