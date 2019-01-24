include master.mk

CC = /opt/intel/bin/icc
CFLAGS += -std=c11 -Wall -Wextra -pedantic -Ofast
#-msse4.2 -xsse4.2 -axsse4.2 -ipo -ansi-alias -funroll-loops -no-inline-max-size
CFLAGS_OMP = $(CFLAGS) -qopenmp
LDFLAGS_OMP = -L/opt/intel/lib/intel64/
LDLIBS += -lrt
LDLIBS_OMP = -liomp5
DEFS += -DCC_INTEL

dft-ir: $(SRC_DFT)
	$(CC) $(CFLAGS) $(DEFS) $(SRC_DFT) -fdump-tree-gimple # or other IRs

fft-ir: $(SRC_FFT)
	$(CC) $(CFLAGS) $(DEFS) $(SRC_FFT) -fdump-tree-gimple # or other IRs

all: dft dft-omp-in dft-omp-out fft fft-omp-in fft-omp-out fftw3f
.PHONY: all
