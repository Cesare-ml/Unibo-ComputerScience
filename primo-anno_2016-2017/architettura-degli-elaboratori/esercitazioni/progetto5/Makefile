main: main.o asm_assembler.o symbol_table.o
	gcc -o assembler main.o asm_assembler.o symbol_table.o

main.o: main.c
	gcc -c main.c

asm_assembler.o: asm_assembler.c
	gcc -c asm_assembler.c

symbol_table.o: symbol_table.c
	gcc -c symbol_table.c

clean:
	rm *.o main
