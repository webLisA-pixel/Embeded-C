#include <ncurses.h>
#include <curses.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define KEY_ENT 10

void print_dir(WINDOW *win, int choice);
void change_path(char *new_dir);
