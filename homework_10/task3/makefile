process_3: process_3.o process_3_func.o
	gcc process_3.o process_3_func.o -g -o process_3

process_3.o: process_3.c
	gcc process_3.c -g -c

process_3_func.o: process_3_func.c
	gcc process_3_func.c -g -c

.PHONY: clean

clean:
	rm -rf *.o process_3
