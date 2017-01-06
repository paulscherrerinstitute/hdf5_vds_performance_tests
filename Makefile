# Edit the following variables as needed
HDF_INSTALL = /home/local/tools/hdf5-1.10.0-patch1/hdf5

EXTLIB  = -L$(HDF_INSTALL)/lib
INCLUDE = -I$(HDF_INSTALL)/include
LIBSHDF = $(EXTLIB) -Wl,-R$(HDF_INSTALL)/lib -lhdf5 -lhdf5_hl

CC      = gcc
CFLAGS  = -O2 -std=c99

BIN_DIR = bin/

EXE	= hdf5_test_prepare hdf5_test_execute
HEADERS	= $(wildcard *.h)
COMMON	= $(HEADERS:.h=.c)
OBJS	= $(COMMON:.c=.o)

all: $(EXE)
	rm -f $(OBJS)

$(EXE): $(OBJS)
	mkdir -p bin
	$(CC) $(CFLAGS) $(options) -o $(BIN_DIR)/$@ $@.c $(OBJS) $(INCLUDE) $(LIBSHDF) $(LIB)

%.o: %.c
	$(CC) $(CFLAGS) $(options) -c -o $@ $^ $(INCLUDE) $(LIBSHDF) $(LIB)

clean: 
	rm -f $(OBJS) $(EXE)
	rm -rf $(BIN_DIR) temp/

