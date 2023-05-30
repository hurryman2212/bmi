all: release

SRCS_C = $(wildcard *.c)

release: $(SRCS_C)
	gcc -O2 -DNDEBUG -mbmi -fverbose-asm -save-temps $^

debug: $(SRCS_C)
	gcc -mbmi -fverbose-asm -save-temps $^

clean:
	rm -f *.o *.s *.i a.out
