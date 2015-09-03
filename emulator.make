CXXFLAGS = -std=c++11 -g

C_FILES += $(wildcard *.c)
CPP_FILES += $(wildcard *.cpp)
OBJS := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)

all: test

test: $(OBJS)
	g++ -o test $(OBJS)

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean: 
	rm *.o test

.PHONY: clean
