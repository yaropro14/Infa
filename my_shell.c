#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <errno.h>
#include <string.h>


int number_com(char * str);

int number_space(char * str);

char ** make_argv(char * str, int n_space)
{
	
	int i = 0;

	char ** argv = calloc (n_space + 1, sizeof(char*));
	for(int j; j <= n_space + 1; j++)
	{
		argv[j] = (char*) calloc(strlen(str) + 1,sizeof(char));
	}
	
	
	char * t = str;
	char * p = t;
	
	while(1)
	{
		p = strchr(t,' ');
		
		if (p == NULL)
		{
			argv[i] = t;
			
			if((p = strchr(t, '\n')))
				*p = '\0';
			
			argv[i+1] = NULL;
			
			break;
		} 
		
		*p = '\0';
		argv[i] = t;
		i++;
		t = p + 1;
		
	}
	
	return  argv;
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

int main()
{
	int wait_p = 0;
	size_t len = 1;
	ssize_t r ;
	char * str = calloc (1000, sizeof(char));
	printf("my_shell$ ");
	r = getline(&str, &len, stdin);
	while(1)
	{
		
		int n_com = number_com(str);
		
		char** com = calloc (n_com, sizeof(char*));
		com = make_com(str, n_com);
		
		if(!(strcmp(com[0], "exit")))
		{
			break;
		}
		
		int i = 0;
		
		int fdt1[2] = {0,0};
			
		int fdt2[2] = {0,0};
		
		pipe(fdt1);
			
		pipe(fdt2);

		while(i < n_com)
		{
			int n_space = number_space(str);
			char** argv;
			argv = make_argv(com[i], n_space);
			
			//printf("i =%d\n", i);
			
			if (i == n_com - 1)
			{
				//pipe(fdt);
				//printf("last\n");
				
				close(fdt1[0]);
				close(fdt1[1]);
				close(fdt2[0]);
				
				fdt1[1] = fdt2[1];
				
				close(fdt2[1]);
				
				//perror(strerror(errno));
			}
			
			else //               ???
			{
				//printf("central\n");
				close(fdt1[0]);
				close(fdt1[1]);
				close(fdt2[0]);
				fdt1[1] = fdt2[1];
				
				//close(fdt2[0]);
				close(fdt2[1]);
				
				pipe(fdt2);
								
			}
			
			pid_t pid = fork();
			
			if(pid == 0)
			{
				if (i == n_com - 1)
				{
					//pipe(fdt);
					//printf("last\n");
					

				}
				
				else if(i == 0)//               ???
				{
					//pipe(fdt1);
					//pipe(fdt2);
					
					//printf("first\n");
					//close(fdt2[1]);
					//close(fdt2[0]);
					
					close(1);
					dup(fdt2[1]);
					close(fdt2[1]);
					
					//perror(strerror(errno));
				}
				
				else //               ???
				{
					//printf("central\n");

					close(1);
					dup(fdt2[1]);
					close(fdt2[1]);
					
					perror(strerror(errno));						
				}
				execvp(argv[0], argv);
				perror(strerror(errno));
			}
			
			else
			{
				i++;
				//free(argv);
				//free(com);
			}	
				
		}
		
		wait(0);
				
		printf("my_shell$ ");
				
		r = getline(&str, &len, stdin);
		
	}
		return 0;

}


int number_space(char * str){
	int n_space = 0;
	char *ptr = NULL;
	char *str2 = NULL;
	ptr = strchr (str, ' ');
	while(ptr){
		n_space++;
		str2 = ptr + 1;
		ptr = strchr (str2, ' ');
	}
	
	//printf("222 %d\n",  n_space);
	return n_space;
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
