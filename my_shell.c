#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>


void free_arg(char ** arg)
{
	free(arg[0]);
	free(arg);
}


char ** make_com(char * str, int n_com )
{
	int i = 0;
	
	char ** com = calloc (n_com + 1, sizeof(char*));
	
	for(int j; j <= n_com + 1; j++)
	{
		com[j] = (char*) calloc(strlen(str) + 1,sizeof(char));
	}
	
	char * t = str;
	char * p = t;
	
	while(1)
	{
		p = strchr(t, '|'); 
		
		if (p == NULL)
		{
			com[i] = t;
			p = strchr(com[i], '\n');
			*p = '\0';
			break;
		}
		
		*p = '\0';
		com[i] = t;
		i++;
		t = p + 1;
		
	}
	
	return  com;
}


int number_com(char * str){
	int n_com = 1;
	char *ptr = NULL;
	char *str2 = NULL;
	ptr = strchr (str, '|');
	while(ptr){
		n_com++;
		str2 = ptr + 1;
		ptr = strchr (str2, '|');
	}
	//printf("3433 %d\n",  n_com);
	return n_com;
}


char * del_spase(char * args)
{
	
	while (isspace(*args)) args ++;
	
	return args;
}


int main()
{
	char * str = calloc (1000, sizeof(char));
	size_t len = 1;
	ssize_t r ;
	
	while(1)
	{	
		printf("my_shell$ ");
		
		r = getline(&str, &len, stdin);
		
		int length = number_com(str);
		
		char ** args = calloc (length, sizeof(char*));
		args = make_com(str, length);
		
		for (int i = 0; i < length; i ++)
		{
			args[i] = del_spase(args[i]);
			
			if(strcmp(args[i], "exit") == 0)
				return 0;
			//printf("%s\n", args[i]);
		}
			
		int fd[2], fdt = -1;
		
		for(int i = 0; i < length; i++)
		{
			pipe(fd);
			int fk = fork();
			
			if(fk == -1)
			{
				perror(strerror(errno));
				return 0;
			}
			
			if(fk == 0)
			{
				close(fd[0]);
				
				if (i != 0)
				{
					close(0);
					dup(fdt);
					close(fdt);	
				}	
					
				if (i != length-1)
				{
					close(1);
					dup(fd[1]);
					close(fd[1]);
				}
				
				int l = 1;
				
				char ** argv = calloc(100,sizeof(char *));
				argv[0] = args[i];
				
				for(int j = 0; args[i][j] != '\0'; j++)
				
					if (args[i][j] == ' ')
					{
						argv[l++] = args[i]+j+1;
						args[i][j++] = '\0';
					}
				execvp(argv[0], argv);
				perror(strerror(errno));
				return 0;
			}
			else
			{
				if(fdt != -1)
					close(fdt);
				fdt = fd[0];
				close(fd[1]);
			}
		}
		for(int i = 0; i < length; i++)
			wait(0);
		free_arg(args);
	}
	return 0;	
}

