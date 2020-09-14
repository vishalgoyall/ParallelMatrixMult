
compile_S: S/main.cpp
	gcc $< -o S/main -D SIZE=${SIZE}

compile_SCO: SCO/main.cpp
	gcc $< -o SCO/main -D SIZE=${SIZE}

compile_ISPC: ISPC-CO/main.cpp
	ispc ISPC-CO/main.ispc -h ISPC-CO/main_ispc.h -o ISPC-CO/main_ispc.o 
	gcc $< ISPC-CO/main_ispc.o -o ISPC-CO/main -D SIZE=${SIZE}

run_S: S/main
	$<

run_SCO: SCO/main
	$<

run_ISPC: ISPC-CO/main
	$<

