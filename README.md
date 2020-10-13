# ParallelMatrixMult
An analysis of matrix multiplication code run on multi-core processors using different parallel programming models: pthreads, openMP and ISPC.

To run the competition, run the command
	make competition SIZE=4096 THREADS=8

Below is the list of Linux commands that can be run from the root directory (./ParallelMatrixMult) to compile and run any of the codes.
S: 
	make S SIZE=4096

SCO: 
	make SCO SIZE=4096

ISPC-CO:
	make ISPC SIZE=4096

PTH-CO: 
	make PTH SIZE=4096 THREADS=6

OMP-CO: 
	make OMP SIZE=4096 THREADS=6

PTH-CO-ISPC: 
	make PTHI SIZE=4096 THREADS=6

OMP-CO-ISPC: 
	make OMPI SIZE=4096 THREADS=6

ISPC-CO-Tasks:
	make ISPCT SIZE=4096 THREADS=6

Below command can be used to run all these individual models for the same input matrices:
	make all SIZE=4096 THREADS=6

