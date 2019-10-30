EXEC = covariance

CFLAGS = -Wall -O0 -g3 -ggdb -std=c++11 -fopenmp -I/usr/local/cuda/include -I./
LDFLAGS =

LDFLAGS += -L/usr/local/cuda/lib64/ -lOpenCL -lm

CC = g++
RM = rm -f

.PHONY: clean 

all: $(EXEC)

$(EXEC): $(EXEC).cpp polybench.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(EXEC)
