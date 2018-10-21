#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <dirent.h>
#include <time.h>
#include <assert.h>


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


typedef struct
{
	char * name;
	int rights;
	char * owner; //
	char * group; //
	int size;
	char str_t[128];
	nlink_t n_link;
	ino_t inode;
	int own_id;
	int gr_id;
	int type;
} files;


typedef struct{
	int nam;
	int rig;
	int lin;
	int id_ow;
	int id_gr;
	int nam_ow;
	int nam_gr;
	int ino;
	int tim;
	int siz;
}print;


char *str[8] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};


struct keys get_keys(int argc, char *argv[]);

void print_ls(files * str_files, int n_files, print pr, struct keys key);

files * make_structs(int * n_files, DIR * dir);

files * make_ls(int * n_files);

int cmp_char_beginning(const void * st1, const void * st2);


int main (int argc, char *argv[])
{
	int n_files = 0;
	
	struct keys key = {0, 0, 0, 0, 0, 0};
	print pr = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	key = get_keys(argc, argv);
	
	if(key.l == -1)
	{
		printf("Unknown key\n");
		return -1;
	}
	
	files * str_files = make_ls(& n_files);
	
	if (key.l == 1)
	{
		pr.nam = 1;
		pr.rig = 1;
		pr.lin = 1;
		pr.nam_ow = 1;
		pr.nam_gr = 1;
		pr.tim = 1;
		pr.siz = 1;
	}
	if (key.n == 1)
	{
		pr.nam = 1;
		pr.rig = 1;
		pr.lin = 1;
		pr.id_ow = 1;
		pr.id_gr = 1;
		pr.nam_ow = 0;
		pr.nam_gr = 0;
		pr.tim = 1;
		pr.siz = 1;
	}
	if (key.i == 1)
	{
		pr.ino = 1;
	}
	
	/*for(int i = 0; i < n_files; i++)
	{
		printf("I = %d\n", i);
		printf("%d %s\n", str_files[i].type, str_files[i].name);
	}*/
	
	qsort(str_files, n_files, sizeof(files*), cmp_char_beginning);
	print_ls(str_files, n_files, pr, key);
	//print_name(str_files, n_files);
	
	//printf("%d\n", key.l);
	//free (str_files);
	return 0;
}


int cmp_char_beginning(const void * st1, const void * st2)
{
	assert(st1);
    assert(st2);
    
    files s1 = *(files *) st2;
    
    files s2 = *(files *) st1;
    
	//printf("!%s!\n", s2.name);
	//printf("1%s1\n", s1.name);
	//printf ("-1\n");
	//	return -1;
    return strcmp(s1.name, s2.name);
}


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
				//printf("///l\n");
				key.l = 1;
				break;
			case 'd':
				//printf("///d\n");
				key.d = 1;
				break;
			case 'a':
				//printf("///a\n");
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
				key.l = -1;
		}
	}
	return key;
}


void print_ls(files * str_files, int n_files, print pr, struct keys key)
{
	int j = 1;
	//char ** dirs = (char **) calloc (j, sizeof(char *));
	for(int i = 0; i < n_files; i++)
	{
		if(key.a == 0 && str_files[i].name[0] == '.')
		{
		}
		else
		{
			
			if(pr.ino == 1)
			{
				printf("%lld\t", str_files[i].inode);
			}
			
			if(str_files[i].type == 32)
			{
				//char ** dirs = (char **) realloc (dirs, j * sizeof(char *));
				//dirs[i - 1] = str_files[i].name;
				j++;
			}
			
			if(pr.rig == 1)
			{
				if(str_files[i].type == 32)
				{
					printf("d");
				}
				printf("%s", str[(str_files[i].rights >> 6) & 7]);
				printf("%s", str[(str_files[i].rights >> 3) & 7]);
				printf("%s\t", str[str_files[i].rights & 7]);
			}
			
			if(pr.lin == 1)
			{
				printf("%hu\t", str_files[i].n_link);
			}
			
			if(pr.nam_ow == 1)
			{
				printf("%s\t", str_files[i].owner);
			}
			
			if(pr.nam_gr == 1)
			{
				printf("%s\t", str_files[i].group);
			}
			
			if(pr.id_ow == 1)
			{
				printf("%d\t", str_files[i].own_id);
			}
			
			if(pr.id_gr == 1)
			{
				printf("%d\t", str_files[i].gr_id);
			}
			
			if(pr.siz == 1)
			{
				printf("%d\t", str_files[i].size);
			}
			
			if(pr.tim == 1)
			{
				printf("%s\t", str_files[i].str_t);
			}
			
			if(pr.nam == 1)
			{
				printf("%s\t", str_files[i].name);
			}
			
			printf("\n");
			
		}
	}
	//free(dirs);
}

files * make_ls(int * n_files)
{
	DIR * dir = opendir(".");
	files * str_files = make_structs(n_files, dir);
	closedir(dir);
	return str_files;
}

files * make_structs(int * n_files, DIR * dir)
{
	int n = 0;
	files * str_files = (files *) calloc (1, sizeof(files));
	struct dirent * c;
//----------------------------------------------------------------------name	
	while((c = readdir(dir))!= NULL)
	{
		//printf("n = %d\n", n);
		n++;
		str_files = (files *) realloc(str_files, n * sizeof(files));
		str_files[n - 1].name = c->d_name;
	}
	
	*n_files = n;
//----------------------------------------------------------------------stat	
	struct stat buf;
	int data;
//----------------------------------------------------------------------	
	struct tm *m_time;
	
	for(int i = 0; i < n; i ++)
	{
		stat(str_files[i].name, &buf);
		m_time = localtime(&buf.st_mtimespec.tv_sec);
		strftime (str_files[i].str_t, 128, "%d %h %R", m_time);
		//printf ("%s\n", str_files[i].str_t);
		str_files[i].size = buf.st_size;
		str_files[i].rights = buf.st_mode;
		str_files[i].n_link = buf.st_nlink;
		str_files[i].inode = buf.st_ino;
		str_files[i].own_id = buf.st_uid;
		str_files[i].gr_id = buf.st_gid;
		str_files[i].type = (str_files[i].rights >> 9);
		str_files[i].owner = "Yar";
		str_files[i].group = "Sta";
		//printf ("%s,\t%d,\t%s%s%s,\t%hu,\t%s\n",str_files[i].name, str_files[i].size, str[(str_files[i].rights >> 6) & 7], str[(str_files[i].rights >> 3) & 7], str[str_files[i].rights & 7], str_files[i].n_link, str_files[i].str_t);
		//printf("%s   %d\n", str_files[i].name, str_files[i].type);
	}
	
	return str_files;
}
