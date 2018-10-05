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
	
	char ** argv = calloc (n_space, sizeof(char*));
	
	char * t = strdup(str);
	char * p = "";
	
	while(p)
	{
		p = strchr(t, ' ');
		
		if (p == NULL)
		{
			argv[i] = t;
			//printf("!!!argv[%d] = %s///\n", i, argv[i]);
			break;
		}
		
		*p = '\0';
		argv[i] = t;
		//printf("argv[%d] = %s;\n", i, argv[i]);
		i++;
		t = p + 1;
		
	}
	//char *** p_argv = &argv;
	return  argv;
}

char ** make_com(char * str, int n_com )
{
	int i = 0;
	//char *** p_com = NULL;
	char ** com = calloc (n_com, sizeof(char*));
	
	char * t = strdup(str);
	char * p = t;
	
	while(1)
	{
		p = strchr(t, '|'); 
		
		if (p == NULL)
		{
			com[i] = t;
			//printf("com[%d] = %s\n", i, com[i]);
			
			if((p = strchr(t, ' ')))
				*p = '\0';
				
			if((p = strchr(t, '\n')))
				*p = '\0';
				
			break;
		}
		 
		*p = '\0';
		com[i] = t;
		//printf("com[%d] = %s\n", i, com[i]);
		i++;
		t = p + 1;
		
	}
	//p_com = &com;
	return  com;
}

int main ()
{
	int wait_p = 0;
	
	size_t len = 0;
	ssize_t read;
	
	printf("my_shel$ ");
	
	char * str = calloc (1000, sizeof(char));
	read = getline(&str, &len, stdin);
	
	//printf("...%s\n", str);
	
	int n_com = number_com(str);
	char** com = calloc (n_com, sizeof(char*));
	
	com = make_com(str, n_com);
	
	for(int i = 0; i < n_com; i++){
		
		char word;
		
		pid_t pid = fork();
	
		int fdt[2] = {0,0};
		pipe(fdt);
		
		int n_space = number_space(str);
		char** argv = calloc (n_com, sizeof(char*));
		argv = make_argv(com[i], n_com);
		
		unsigned int len = 1;
		while (read(fdt[0], &word, len) != 0) //  Error
		{
			int j = 0;
			argv[n_space + (j++)] = strdup(&word);
		}
		
		if(pid)
		{
			close(1);
			dup(fdt[1]);
			close(fdt[1]);
			execv(argv[0], argv);
			printf("error\n");
		}
		
		//printf("!%s\n", argv[0]);
		free(argv);
		break;
	}
	
	//printf("?%s\n", com[0]);
	
	
	free(str);
	free(com);		
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
	return n_com;
}
