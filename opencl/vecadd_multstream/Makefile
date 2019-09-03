EXEC = vecadd

CFLAGS = -Wall -O0 -g3 -ggdb -std=c++11 -I/home/moon/local/include
LDFLAGS =

LDFLAGS += -L/home/zp/local/lib64/ -lltdl -lOpenCL -lm

CC = g++
RM = rm -f

.PHONY: clean 

all: $(EXEC)

$(EXEC): vecadd.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(EXEC)
