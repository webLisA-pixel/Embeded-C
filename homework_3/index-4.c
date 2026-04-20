#include <stdio.h>
#define size_line 150
#define size_subline 15

int main(void)
{
	char line[size_line];
	char sub_line[size_subline];
	char *ptr_line=(char *)&line;
	char *ptr_sub_line=(char *)&sub_line;
	char *ptr_flag=(char *)&sub_line;
	char *ptr_link=NULL;
	printf("Input text: ");
	fgets(line,size_line,stdin);
	printf("Input subtext: ");
	fgets(sub_line,size_subline,stdin);
	while(*ptr_line!='\n')
	{
		if(*(ptr_sub_line+1)=='\n')
			break;
		if(*ptr_line==*ptr_sub_line)
		{
			if(ptr_sub_line==ptr_flag)
				ptr_link=ptr_line;
			ptr_sub_line++;
			ptr_line ++;
		}
		if(*ptr_line!=*ptr_sub_line)
		{
			if(ptr_sub_line!=ptr_flag)
				ptr_sub_line=ptr_flag;
			ptr_line++;
			continue;
		}
	}
	printf("%c", *ptr_link);
	return 0;
}