
##############################
# Targets to compile all individual codes
##############################

compile_S: S/main.cpp
	g++ $< -o S/main -D SIZE=${SIZE}

compile_SCO: SCO/main.cpp
	g++ $< -o SCO/main -D SIZE=${SIZE}

compile_ISPC: ISPC-CO/main.cpp
	ispc ISPC-CO/main.ispc -h ISPC-CO/main_ispc.h -o ISPC-CO/main_ispc.o 
	g++ $< ISPC-CO/main_ispc.o -o ISPC-CO/main -D SIZE=${SIZE}

compile_PTH: PTH-CO/main.cpp
	g++ $< -pthread -o PTH-CO/main -D SIZE=${SIZE} -D NUM_OF_THREADS=$(THREADS)

compile_OMP: OMP-CO/main.cpp
	g++ $< -fopenmp -o OMP-CO/main -D SIZE=${SIZE} -D NUM_OF_THREADS=$(THREADS)

compile_PTHI: PTH-CO-ISPC/main.cpp
	ispc PTH-CO-ISPC/main.ispc -h PTH-CO-ISPC/main_ispc.h -o PTH-CO-ISPC/main_ispc.o 
	g++ $< PTH-CO-ISPC/main_ispc.o -pthread -o PTH-CO-ISPC/main -D SIZE=${SIZE} -D NUM_OF_THREADS=$(THREADS)

compile_OMPI: OMP-CO-ISPC/main.cpp
	ispc OMP-CO-ISPC/main.ispc -h OMP-CO-ISPC/main_ispc.h -o OMP-CO-ISPC/main_ispc.o 
	g++ $< OMP-CO-ISPC/main_ispc.o -fopenmp -o OMP-CO-ISPC/main -D SIZE=${SIZE} -D NUM_OF_THREADS=$(THREADS)

compile_ISPCT: ISPC-CO-Tasks/main.cpp
	ispc ISPC-CO-Tasks/main.ispc -h ISPC-CO-Tasks/main_ispc.h -o ISPC-CO-Tasks/main_ispc.o -DSIZE=${SIZE} -DNUM_OF_TASKS=${THREADS}
	g++ -c ISPC-CO-Tasks/tasksys.cpp -o ISPC-CO-Tasks/tasksys.o -pthread
	g++ $< ISPC-CO-Tasks/main_ispc.o ISPC-CO-Tasks/tasksys.o -pthread -o ISPC-CO-Tasks/main -D SIZE=${SIZE} -D NUM_OF_TASKS=${THREADS}

##############################
# Targets to run all the individual codes
##############################

run_S: S/main
	$<

run_SCO: SCO/main
	$<

run_ISPC: ISPC-CO/main
	$<

run_PTH: PTH-CO/main
	$<

run_OMP: OMP-CO/main
	$<

run_PTHI: PTH-CO-ISPC/main
	$<

run_OMPI: OMP-CO-ISPC/main
	$<

run_ISPCT: ISPC-CO-Tasks/main
	$<

##############################
# Combined targets
##############################

S: compile_S run_S

SCO: compile_SCO run_SCO

ISPC: compile_ISPC run_ISPC

PTH: compile_PTH run_PTH

OMP: compile_OMP run_OMP

PTHI: compile_PTHI run_PTHI

OMPI: compile_OMPI run_OMPI

ISPCT: compile_ISPCT run_ISPCT

##############################
# global target to make all
##############################

all: S SCO ISPC PTH OMP PTHI OMPI ISPCT

