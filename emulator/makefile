CXXFLAGS = -std=gnu++11 -Iarduino -Iemu -fpermissive -Wall
CFLAGS = -Iarduino -Iemu -I/usr/include/python3.4 -fpermissive -Wall
LDFLAGS = -lpthread

# Local sources
C_FILES += $(wildcard *.c)
CPP_FILES += $(wildcard *.cpp)

# Arduino sources
C_FILES += $(wildcard arduino/*.c)
CPP_FILES += $(wildcard arduino/*.cpp)

# Emulator sources
C_FILES += $(wildcard emu/*.c)
CPP_FILES += $(wildcard emu/*.cpp)

OBJS := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)

all: test

test: $(OBJS)
	g++ -g -o test $(OBJS) $(LDFLAGS)

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean:
	rm *.o test

.PHONY: clean
