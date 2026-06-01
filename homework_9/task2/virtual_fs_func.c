#include "virtual_fs.h"

char *path_dir;
struct dirent **dir_list;
/*Function print contant*/
void print_dir(WINDOW *win, int choice)
{
	int i=0;
	scandir(path_dir, &dir_list, NULL, alphasort);	//Reciving contant directory
	i=0;
	while(dir_list[i]!=NULL)
	{
		if(strcmp(dir_list[i]->d_name, ".")==0)
		{
			i++;
			continue;
		}
		if(i==choice) wattron(win, A_REVERSE);
		mvwprintw(win, i, 1, "%s", dir_list[i]->d_name);
		wattroff(win, A_REVERSE);
		i++;
	}
	wrefresh(win);
}
/*Function change dir*/
void change_path(char *new_dir)
{
	char *new_path;
	int i=0;
	int length=0;
	/*Moving down to the catalog*/
	if(strcmp(new_dir, "..")==0)
	{
		if(path_dir==NULL)
		{
			perror("No path_dir\n");
			exit(1);
		}
		length=strlen(path_dir);
		while(path_dir[length]!='/') 
			length--;
		if(length!=1) length--;
		new_path=malloc(length);
		if(strcmp(new_path, "")!=0)
		{
			free(new_path);
			new_path=malloc(length);
		}
		if(new_path<0)
		{
			perror("Out of memory");
			exit(1);
		}
		for(i=0;i<=length;i++)
		{
			new_path[i]=path_dir[i];
		}
		path_dir="";
		path_dir=strdup(new_path);
		free(new_path);
	}
	else
	{
		/*Moving down to the catalog*/
		i=0;
		while(dir_list[i]!=NULL)
		{
			if(strcmp(dir_list[i]->d_name, new_dir)==0)
			{
				if(dir_list[i]->d_type==4)
				{
					length=strlen(new_dir);
					new_path=malloc(strlen(path_dir)+length);
					if(new_path<0)
					{
						perror("Out of memory");
						exit(1);
					}
					if(strcmp(new_path, "")!=0)
					{
						free(new_path);
						new_path=malloc(strlen(path_dir)+length);
					}
					sprintf(new_path, "%s/%s", path_dir, new_dir);
					path_dir="";
					path_dir=strdup(new_path);
					free(new_path);
					break;
				}
				i++;
			}
			else
			{
				i++;
				continue;
			}
		}
		if(dir_list[i]==NULL) printf("Non action\n");
	}
}