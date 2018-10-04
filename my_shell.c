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
	char * p;
	
	while(p)
	{
		p = strchr(t, ' ');
		*p = '\0';
		argv[i] = t;
		i++;
		t = p + 1;
	}
	
	argv[i] = t;
	
	return  argv;
}

char ** make_com(char * str, int n_com )
{
	int i = 0;
	
	char ** com = calloc (n_com, sizeof(char*));
	
	char * t = strdup(str);
	printf("1");
	char * p = t;
	printf("1");
	while(p)
	{
		printf("1");
		p = strchr(t, '|');
		*p = '\0';  //         FALLS HERE!!!
		com[i] = t;
		i++;
		t = p + 1;
		printf("i = %d, s = %s\n", i, com[i]);
		p = NULL;
	}
	
	com[i] = t;
	
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
		int n_space = number_space(str);
		char** argv = calloc (n_com, sizeof(char*));
		argv = make_argv(str, n_com);
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
