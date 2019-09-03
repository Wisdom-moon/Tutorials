EXEC = vecadd

#CFLAGS = -Wall -g3 -ggdb -O0 -I/home/moon/local/include
CFLAGS = -Wall -O3 -ffast-math -I/home/moon/local/include
LDFLAGS =

LDFLAGS += -L/home/zp/local/lib/ -lltdl -lm /home/moon/local/lib/libOpenCL.so.1.7.0

CC = gcc
RM = rm -f

.PHONY: clean 

all: $(EXEC) vecadd_kernel.so

$(EXEC): vecadd.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

vecadd.o: vecadd.c
	$(CC) -c $(CFLAGS) -o $@ $^ 

vecadd_kernel.so: vecadd_kernel.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $^

clean:
	$(RM) $(EXEC) vecadd_kernel.so vecadd.o
