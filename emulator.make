CXXFLAGS = -std=gnu++11 -g -pthread -I. -Wall
CFLAGS = -I. -Wall
LDFLAGS = -lpthread 

C_FILES += $(wildcard *.c)
CPP_FILES += $(wildcard *.cpp)
OBJS := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)

all: test

test: $(OBJS)
	g++ -g -o test $(OBJS) $(LDFLAGS) 

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean: 
	rm *.o test

.PHONY: clean
