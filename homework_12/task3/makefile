chanel_3: chanel_3.o chanel_3_func.o
	gcc chanel_3.o chanel_3_func.o -g -o chanel_3

chanel_3.o: chanel_3.c
	gcc chanel_3.c -g -c

chanel_3_func.o: chanel_3_func.c
	gcc chanel_3_func.c -g -c

.PHONY: clean chanel_3

clean:
	rm -rf *.o chanel_3
