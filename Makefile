all:
	gcc -c -mbmi bmi.c
	gcc -c -mbmi test.c
	gcc bmi.o test.o

clean:
	rm -f *.o a.out
