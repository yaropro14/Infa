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
	int optind;
};


struct files_dir
{
	int n_files;
	int n_dirs;
	char ** files;
	char ** dirs;
};


typedef struct
{
	const char * name;
	int rights;
	char * owner; //
	char * group; //
	int size;
	char str_t[128];
	nlink_t n_link;
	ino_t inode;
	int own_id;
	int gr_id;
	char type;
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


struct files_dir make_ls(int argc, char * argv[]);
struct keys get_keys(int argc, char *argv[]);
struct files_dir file_or_dir(char * object, struct files_dir f_d);
files * read_files_from_dir(DIR * dir, int * n_files);
int print_ls(files * str_files, int n_files, print pr, struct keys key);

int do_ls_f(struct files_dir f_d, struct keys key, print pr);
int do_ls_d(struct files_dir f_d, struct keys key, print pr);

files make_structs_file(files file);
files make_structs_file(files file);

int cmp_char_beginning(const void * st1, const void * st2);
char display_file_type ( int st_mode );


int main (int argc, char *argv[])
{
	struct files_dir f_d =  make_ls(argc, argv);

}


struct files_dir make_ls(int argc, char * argv[])
{
	struct files_dir f_d;
	
	f_d.n_files = 0;
	f_d.n_dirs = 0;
	f_d.files = (char **) calloc(1, sizeof(char*));
	f_d.dirs = (char **) calloc(1, sizeof(char*));
	
	print pr = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	struct keys key = {0, 0, 0, 0, 0, 0};
	
	key = get_keys(argc, argv);
	
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
	
	if(key.l == -1)
	{
		printf("Unknown key\n");
		f_d.n_files = -1;
		f_d.n_dirs = -1;
		return f_d;
	}
	
	for(int i = key.optind + 1; i < argc; i ++)
	{
		f_d = file_or_dir(argv[i], f_d);
	}
	
	if (key.optind + 1 == argc)
	{
		f_d.n_dirs ++;
		f_d.dirs = realloc(f_d.dirs, f_d.n_dirs);
		f_d.dirs[f_d.n_dirs - 1] = ".";
	}
	
	int d = 0;
	
	if(f_d.n_files != 0)
		d = do_ls_f(f_d, key, pr);
		
	if(f_d.n_dirs != 0)
		d = do_ls_d(f_d, key, pr);
	
	return f_d;
}

int do_ls_f(struct files_dir f_d, struct keys key, print pr)
{
	files * str_files = (files *) calloc (1, sizeof(files));
	
	for(int i = 0; i < f_d.n_files; i ++)
	{
		str_files = (files *) realloc(str_files, (i + 1) * sizeof(files));
		str_files[i].name = f_d.files[i];
	}
	
	for(int i = 0; i < f_d.n_files; i ++)
		{
			str_files[i] = make_structs_file(str_files[i]);
		}
		
	qsort(str_files, f_d.n_files, sizeof(files), cmp_char_beginning);
	return print_ls(str_files, f_d.n_files, pr, key);
}


int do_ls_d(struct files_dir f_d, struct keys key, print pr)
{
	int n_files;
	
	for(int i = 0; i < f_d.n_dirs; i ++)
	{
		DIR * dir = opendir(f_d.dirs[i]);
		if(dir != NULL)
		{
			files * str_files = read_files_from_dir(dir, &n_files);
			
			for(int j = 0; j < n_files; j ++)
			{
				str_files[j] = make_structs_file(str_files[j]);
			}
			
			qsort(str_files, n_files, sizeof(files), cmp_char_beginning);
			if(f_d.n_files != 0) printf("\n%s:\n", f_d.dirs[i]);
			print_ls(str_files, n_files, pr, key);
			
			closedir(dir);
		}
	}
	return 1;
}

files * read_files_from_dir(DIR * dir, int * n_files)
{	
	int n = 0;
	files * str_files = (files *) calloc (1, sizeof(files));
	struct dirent * c;

	
	while((c = readdir(dir))!= NULL)
	{
		n++;
		str_files = (files *) realloc(str_files, n * sizeof(files));
		str_files[n - 1].name = c->d_name;
	}
	
	* n_files = n;
	
	return str_files;
}

files make_structs_file(files file)
{
//----------------------------------------------------------------------stat	
	struct stat buf;
	int data;
//----------------------------------------------------------------------	
	struct tm *m_time;

	printf("stat = %d", stat(file.name, &buf));
	
	m_time = localtime(&buf.st_mtimespec.tv_sec);
	strftime(file.str_t, 128, "%d %h %R", m_time);
	//printf ("%s\n", str_files[i].str_t);
	file.size = buf.st_size;
	file.rights = buf.st_mode;
	file.n_link = buf.st_nlink;
	file.inode = buf.st_ino;
	file.own_id = buf.st_uid;
	file.gr_id = buf.st_gid;
	file.type = display_file_type (buf.st_mode);
	//if( S_ISDIR( buf.st_mode ))
		printf ("   %s\n",file.name );
	
	//printf("Type = %d\n", str_files[i].type);
	file.owner = "Yar";
	file.group = "Sta";
	//printf ("%s,\t%d,\t%s%s%s,\t%hu,\t%s\n",str_files[i].name, str_files[i].size, str[(str_files[i].rights >> 6) & 7], str[(str_files[i].rights >> 3) & 7], str[str_files[i].rights & 7], str_files[i].n_link, str_files[i].str_t);
	//printf("%s   %d\n", str_files[i].name, str_files[i].type);
	
	return file;
}



struct keys get_keys(int argc, char *argv[])
{
	struct keys key = {0, 0, 0, 0, 0, 0};
	int longindex = 0;
	char * optstring = "alnRid";


	int opt;
	while((opt = getopt_long(argc, argv, optstring, longopts, &longindex)) != -1)
	{
		switch(opt)
		{
			case 'l':
				key.l = 1;
				break;
			case 'd':
				key.d = 1;
				break;
			case 'a':
				key.a= 1;
				break;
			case 'n':
				key.n = 1;
				break;
			case 'R':
				key.R = 1;
				break;
			case 'i':
				key.i = 1;
				break;
			default:
				key.l = -1;
		}
	}
	
	key.optind = optind;

	return key;
}


int cmp_char_beginning(const void * st1, const void * st2)
{
	assert(st1);
    assert(st2);
    
    files s1 = *(files *) st2;
    
    files s2 = *(files *) st1;
    
    return -strcmp(s1.name, s2.name);
}


struct files_dir file_or_dir(char * object, struct files_dir f_d)
{
	struct stat buf;
	if(stat(object, &buf) == -1)
	{ 
		printf("ls: %s: No such file or directory\n", object);
		return f_d;
	}
	
	int type = (buf.st_mode >> 9);
	
	if(type == 32)
	{
		f_d.n_dirs ++;
		f_d.dirs = realloc(f_d.dirs, f_d.n_dirs);
		f_d.dirs[f_d.n_dirs - 1] = object;
		printf ("32   %s\n", object);
	}
	else
	{
		f_d.n_files ++;
		f_d.files = realloc(f_d.files, f_d.n_files);
		f_d.files[f_d.n_files - 1] = object;
		printf ("64    %s\n", object);
		//printf("1\n");
	}
	
	return f_d;
}


int print_ls(files * str_files, int n_files, print pr, struct keys key)
{
	int j = 0;
	char ** dirs = (char **) calloc (j, sizeof(char *));
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
			
			if(str_files[i].type == 'd')
			{
				if(strcmp(str_files[i].name,".") != 0 && strcmp(str_files[i].name,"..") != 0 && strcmp(str_files[i].name,".DS_Store") != 0)
				{
					dirs = (char **) realloc (dirs, (j + 1) * sizeof(char *));
					//dirs[j] = str_files[i].name;
					j++;
					//printf("1\n");
					//printf("%s\n", dirs[j - 1]);
				}
			}
			
			//printf("2%s\n", dirs[1]);
			
			if(pr.rig == 1)
			{
				if(S_ISDIR(str_files[i].rights))
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
			//printf("%d\n", str_files[i].type);
			//printf("%s\n", dirs[0]);
		}
	}
	
	//printf("%s\n", dirs[0]);
	
	struct files_dir fd;
	fd.n_dirs = 1;
	fd.n_files = 0;
	
	fd.dirs =(char **) calloc(1, sizeof(char*));
	
	for(int i = 0; i < j; i ++)
	{
		printf("\n./%s:\n", dirs[i]);
		fd.dirs[0] = dirs[i];
		int d = do_ls_d(fd, key, pr);
	}
	
	return 1;
	
}


char display_file_type ( int st_mode ) {                                   
    // тут все ясно
    switch ( st_mode & S_IFMT ) {
        case S_IFDIR:  
            return 'd';
            break;
		default:
			return 'f';
    }
    return '1';
}

