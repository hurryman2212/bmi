all:
	# gcc -c -O2 -DNDEBUG -march=native -mbmi -fverbose-asm -save-temps bmi.c
	# gcc -c -O2 -DNDEBUG -march=native -mbmi -fverbose-asm -save-temps test.c
	gcc -c -mbmi -fverbose-asm -save-temps bmi.c
	gcc -c -mbmi -fverbose-asm -save-temps test.c
	gcc bmi.o test.o

clean:
	rm -f *.o *.s *.i a.out
