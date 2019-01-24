include master.mk

CC = /opt/pgi/linux86-64/2016/bin/pgcc
CFLAGS += -fast -fastsse -Mipa=fast,inline,safe,libopt,libinline -Msafeptr #-Minfo
CFLAGS_OMP = $(CFLAGS) -Mconcur=cncall,allcores
LDFLAGS_OMP = -mp
LDLIBS_OMP =
DEFS += -DCC_PGI

all: dft dft-omp-in dft-omp-out fft fft-omp-in fft-omp-out fftw3f
.PHONY: all
