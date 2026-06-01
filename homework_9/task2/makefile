virtual_fs: virtual_fs.o virtual_fs_func.o
	gcc virtual_fs.o virtual_fs_func.o -g -o virtual_fs -lncurses

virtual_fs.o: virtual_fs.c
	gcc virtual_fs.c -g -c

virtual_fs_func.o: virtual_fs_func.c
	gcc virtual_fs_func.c -g -c

.PHONY: clean virtual_fs

clean:
	rm -rf *.o virtual_fs
