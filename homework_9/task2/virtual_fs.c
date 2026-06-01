#include "virtual_fs.h"

extern char *path_dir;
extern struct dirent **dir_list;

int main(void)
{
	path_dir=realpath(".", NULL);	//attach current dir
	char *new_dir;
	int y;
	int x;
	int key;
	int choice=1;
	/*Initialize windows, and split for several areas */
	initscr();
	cbreak();
	noecho();
	getmaxyx(stdscr, y, x);
	WINDOW *win=newwin(y, x/2, 0, 0);
	if(win<0)
	{
		perror("Out of memory!|n");
		exit(1);
	}
	refresh();
	box(win, 0, 0);
	mvwprintw(win, 0, 1, "%s", path_dir);
	wrefresh(win);
	keypad(win, true);

	while(1)
	{
		print_dir(win, choice);	//print content directory
		key=wgetch(win);
		/*processing press key*/
		switch(key)
		{		
			case KEY_UP:	//Key move up
				choice--;
				if(strcmp(dir_list[choice]->d_name, ".")==0) choice=1;
				break;
			case KEY_DOWN:	//Key move down
				if(dir_list[choice+1]==NULL) break;
				choice++;
				break;
			case KEY_ENT:	//change dir
				if(dir_list[choice]->d_type==4)
				{
					new_dir=strdup(dir_list[choice]->d_name);
					change_path(new_dir);
					choice=1;
					wclear(win);
					box(win, 0, 0);
					mvwprintw(win, 0, 1, "%s", path_dir);
					wrefresh(win);
					break;
				}
				else break;
		}
		//if(strcmp(new_dir, "exit")==0) break;
		//else change_path(new_dir);
	}
	delwin(win);
	endwin();
	exit(0);
}