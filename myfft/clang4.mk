include master.mk

CC = clang
CFLAGS += -std=c11 -Wall -Wextra -Wpedantic -Weverything -Ofast -march=native -funroll-loops -fslp-vectorize-aggressive -fvectorize -flto
#-Rpass-analysis=unroll -Rpass-analysis=vectorize
CFLAGS_OMP = $(CFLAGS) -I/usr/local/Cellar/libiomp/20150701/include/libiomp/ -openmp
LDFLAGS_OMP = -L/usr/local/Cellar/libiomp/20150701/lib/
LDLIBS += -lrt
LDLIBS_OMP = -liomp5
DEFS += -DCC_CLANG

dft-ir: $(SRC_DFT)
	$(CC) $(CFLAGS) $(DEFS) $(SRC_DFT) -S -emit-llvm
	/usr/local/opt/llvm/bin/llvm-link -S -v -o $@.ll *.s

fft-ir: $(SRC_FFT)
	$(CC) $(CFLAGS) $(DEFS) $(SRC_FFT) -S -emit-llvm
	/usr/local/opt/llvm/bin/llvm-link -S -v -o $@.ll *.s
# /usr/local/opt/llvm/bin/opt -dot-cfg fft.ll
# |/usr/local/opt/llvm/bin/opt -std-link-opts -view-cfg-only
all: dft fft fftw3f
.PHONY: all
