#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <getopt.h>

/*struct option
 * {
	const char *name;
	int has_arg;
	int *flag;
	int val;
};*/


static struct option longopts[] = 
			{
                   {"long",    			no_argument,	0,  'l' },
                   {"directory",		no_argument,	0,  'd' },
                   {"all",  			no_argument,	0,  'a' },
                   {"numeric-uid-gid",	no_argument,	0,  'n' },
                   {"recursive",  		no_argument,	0,	'R' },
                   {"inode",    		no_argument,	0,  'i' },
                   {0,					0,				0,  0	}
             };


struct keys
{
	int l;
	int d;
	int a;
	int n;
	int R;
	int i;
};


struct keys get_keys(int argc, char *argv[])
{
	//printf("1");
	struct keys key = {0, 0, 0, 0, 0, 0};
	//printf("2");
	int longindex = 0;
	//printf("3");
	char * optstring = "alnRid";
	//printf("4");
	//int opt = getopt_long_only(argc, argv, optstring, longopts, &longindex);
	//printf("5");
	//printf ("%c\n", opt);
	int opt;
	while((opt = getopt_long(argc, argv, optstring, longopts, &longindex)) != -1)
	{
		//printf("i = , opt = %c\n", opt);
		switch(opt)
		{
			case 'l':
				//printf("l");
				key.l = 1;
				break;
			case 'd':
				//printf("l");
				key.d = 1;
				break;
			case 'a':
				//printf("l");
				key.a= 1;
				break;
			case 'n':
				//printf("l");
				key.n = 1;
				break;
			case 'R':
				//printf("l");
				key.R = 1;
				break;
			case 'i':
				//printf("l");
				key.i = 1;
				break;
			default:
			printf("Unknown key\n");
				break;
		}
	}
	return key;
}


int main (int argc, char *argv[])
{
	struct keys key = {0, 0, 0, 0, 0, 0};
	key = get_keys(argc, argv);
	printf("%d\n", key.l);
	return 0;
}
