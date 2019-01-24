include master.mk

CC = gcc-7
CFLAGS += -std=c11 -Wall -Wextra -Wpedantic -O3 -march=native -flto -ftree-vectorize -ftree-vectorizer-verbose=7 -finline-functions
# -ftree-parallelize-loops=8
#-funroll-loops -fipa-pta -floop-nest-optimize -fstrict-overflow -fsingle-precision-constant -fprofile-use -funsafe-math-optimizations -ffloat-store -ffp-contract=fast -fmodulo-sched -funsafe-loop-optimizations -fwhole-program -freorder-blocks-and-partition -fvect-cost-model -floop-flatten
# -fuse-linker-plugin
CFLAGS_OMP = $(CFLAGS) -fopenmp
LDFLAGS_OMP = -L/usr/local/lib/gcc/7/
LDLIBS_OMP = -lgomp
DEFS += -DCC_GCC

dft-ir: $(SRC_DFT)
	$(CC) $(CFLAGS) $(DEFS) $(SRC_DFT) -fdump-tree-gimple # or other IRs

fft-ir: $(SRC_FFT)
	$(CC) $(CFLAGS) $(DEFS) $(SRC_FFT) -fdump-tree-gimple # or other IRs

dft-asm: $(SRC_DFT)
	$(CC) $(CFLAGS) -g -S -fverbose-asm $(DEFS) $(SRC_DFT)

fft-asm: $(SRC_FFT)
	$(CC) $(CFLAGS) -g -S -fverbose-asm $(DEFS) $(SRC_FFT)
#  -masm=intel doesnt work on Darwin

all: dft dft-omp-in dft-omp-out fft fft-omp-in fft-omp-out fftw3f
.PHONY: all