CFLAGS =
LDLIBS = -lm
#-lrt
DEFS = -DPRINT_RESULTS -DINTERNAL_COMPLEX_STRUCT -DINTERNAL_LONG
# -DINTERNAL_DOUBLE #-DFFT_IN_PLACE #-DN=128 -DR=16384 #-DN=65536 -DR=1
DEFS_OMP = $(DEFS)
DEFS_FFTW3 = $(DEFS) -DFFTW

SRC = common.c complex.c ft_tools.c ft_common.c

SRC_DFT = $(SRC) dft.c run_dft.c
SRC_DFT_IN = $(SRC) dft_omp.c run_dft.c
SRC_DFT_OUT = $(SRC) dft.c run_dft_omp.c

SRC_FFT = $(SRC) fft.c run_fft.c
SRC_FFT_IN = $(SRC) fft_omp.c run_fft.c
SRC_FFT_OUT = $(SRC) fft.c run_fft_omp.c

SRC_FFTW3 = common.c ft_tools.c run_fftw3.c

dft: $(SRC_DFT)
	$(CC) $(CFLAGS) $(DEFS) $(SRC_DFT) -o $@.x $(LDLIBS)

dft-cf: $(SRC_DFT)
	cflow --include=x -n -o $@.txt $(SRC_DFT)

dft-omp-in: $(SRC_DFT_IP)
	$(CC) $(CFLAGS_OMP) $(DEFS_OMP) $(SRC_DFT_IN) -o $@.x $(LDLIBS) $(LDFLAGS_OMP) $(LDLIBS_OMP)

dft-omp-out: $(SRC_DFT_OP)
	$(CC) $(CFLAGS_OMP) $(DEFS_OMP) $(SRC_DFT_OUT) -o $@.x $(LDLIBS) $(LDFLAGS_OMP) $(LDLIBS_OMP)


fft: $(SRC_FFT)
	$(CC) $(CFLAGS) $(DEFS) $(SRC_FFT) -o $@.x $(LDLIBS)

fft-cf: $(SRC_FFT)
	cflow --include=x -n -o $@.txt $(SRC_FFT)

fft-omp-in: $(SRC_FFT_IP)
	$(CC) $(CFLAGS_OMP) $(DEFS_OMP) $(SRC_FFT_IN) -o $@.x $(LDLIBS) $(LDFLAGS_OMP) $(LDLIBS_OMP)

fft-omp-out: $(SRC_FFT_OP)
	$(CC) $(CFLAGS_OMP) $(DEFS_OMP) $(SRC_FFT_OUT) -o $@.x $(LDLIBS) $(LDFLAGS_OMP) $(LDLIBS_OMP)


fftw3f:
	$(CC) $(CFLAGS) $(DEFS_FFTW3) $(SRC_FFTW3) -o $@.x -lfftw3f $(LDLIBS)

fftw3d:
	$(CC) $(CFLAGS) $(DEFS_FFTW3) $(SRC_FFTW3) -o $@.x -lfftw3 $(LDLIBS)


.PHONY: clean
clean:
	rm *.x *.o *.oo

.PHONY: clean-out
clean-out:
	rm out/*.*
