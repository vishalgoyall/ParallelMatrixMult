
compile_S: S/main.cpp
	gcc $< -o S/main -D SIZE=${SIZE}

compile_SCO: SCO/main.cpp
	gcc $< -o SCO/main -D SIZE=${SIZE}

compile_ISPC: ISPC-CO/main.cpp
	ispc ISPC-CO/main.ispc -h ISPC-CO/main_ispc.h -o ISPC-CO/main_ispc.o 
	gcc $< ISPC-CO/main_ispc.o -o ISPC-CO/main -D SIZE=${SIZE}

compile_PTH: PTH-CO/main.cpp
	gcc $< -pthread -o PTH-CO/main -D SIZE=${SIZE} -D NUM_OF_THREADS=$(THREADS)

compile_PTHI: PTH-CO-ISPC/main.cpp
	ispc PTH-CO-ISPC/main.ispc -h PTH-CO-ISPC/main_ispc.h -o PTH-CO-ISPC/main_ispc.o 
	gcc $< PTH-CO-ISPC/main_ispc.o -pthread -o PTH-CO-ISPC/main -D SIZE=${SIZE} -D NUM_OF_THREADS=$(THREADS)

run_S: S/main
	$<

run_SCO: SCO/main
	$<

run_ISPC: ISPC-CO/main
	$<

run_PTH: PTH-CO/main
	$<

run_PTHI: PTH-CO-ISPC/main
	$<




S: compile_S run_S

SCO: compile_SCO run_SCO

ISPC: compile_ISPC run_ISPC

PTH: compile_PTH run_PTH

PTHI: compile_PTHI run_PTHI

all: S SCO ISPC PTH PTHI
