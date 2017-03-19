TARGETS=bin/gcc_intrinsics bin/x86_asm

FLAGS=-O2 -fopenmp

all: $(TARGETS)

bin/gcc_intrinsics: src/gcc_intrinsics.c
	gcc $(FLAGS) -o $@ $^

bin/x86_asm: src/x86_asm.c
	gcc $(FLAGS) -o $@ $^

clean:
	rm -f bin/*
