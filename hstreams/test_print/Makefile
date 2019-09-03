COI_DIR ?= /usr/include/intel-coi

CC = icc -Wall -Werror-all

COI_INC ?= -I$(COI_DIR)

COMMON_FLAGS ?= $(COI_INC) -I./ -I../../common/ -mkl -diag-disable 13368 -diag-disable 15527

CFG = RELEASE
DEBUG_FLAGS = -g3 -ggdb -O0 -D_DEBUG
RELEASE_FLAGS = -DNDEBUG -O3

ifeq (RELEASE,$(CFG))
OPT = $(RELEASE_FLAGS)
else
OPT = $(DEBUG_FLAGS)
endif

CC_FLAGS = $(COMMON_FLAGS) $(OPT) -Wl,--enable-new-dtags
HOST_FLAGS = $(CC_FLAGS) -lcoi_host
DEV_FLAGS = $(CC_FLAGS) -lcoi_device -rdynamic

BIN_HOST = ./bin/host
BIN_DEV  = ./bin/dev

HOST_CC ?= $(CC)       -openmp
DEV_CC  ?= $(CC) -mmic -openmp

all: $(BIN_HOST) $(BIN_DEV) $(BIN_HOST)/vecadd $(BIN_DEV)/kernel.so

$(BIN_HOST) $(BIN_DEV):
	mkdir -p $@
clean:
	rm -fr $(BIN_HOST)/vecadd $(BIN_DEV)/kernel.so *.o 

HOST_CPP_SRCS     = main.cpp 
DEV_CPP_SRCS      = kernel.cpp
HOST_CPP_OBJS     = $(HOST_CPP_SRCS:.cpp=.host.o)
DEV_CPP_OBJS      = $(DEV_CPP_SRCS:.cpp=.dev.o)

$(HOST_CPP_OBJS): %.host.o: %.cpp
	$(HOST_CC) $(HOST_FLAGS) -c $^ -o $@

$(DEV_CPP_OBJS): %.dev.o: %.cpp
	$(DEV_CC) $(DEV_FLAGS) -fPIC -shared -c $^ -o $@

$(BIN_HOST)/test_print: $(HOST_CPP_OBJS)
	$(HOST_CC) $(HOST_FLAGS) -o $@ $^ -L$(BIN_HOST)  -lhstreams_source

$(BIN_DEV)/kernel.so: $(DEV_CPP_OBJS)
	$(DEV_CC) $(DEV_FLAGS) -fPIC -shared -o $@ $^ -L$(BIN_DEV) -lhstreams_sink -Wl,-soname,kernel_cpu.so -static-intel

