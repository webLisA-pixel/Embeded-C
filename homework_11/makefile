thread: thread.o thread_func.o
	gcc pthread.o threadfunc.o -g -o thread

thread.o: thread.c
	gcc process_3.c -g -c

thread_func.o: thread_func.c
	gcc thread_func.c -g -c

.PHONY: clean 

clean:
	rm -rf *.o thread
