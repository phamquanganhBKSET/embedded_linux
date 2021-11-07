//Implement ls
//ls can be used to print the current directory or make a list of directories, display some information of files.
//ls-a can display all files (including hidden files) in the directory.
//ls-l lists long data strings, including data such as file attributes and permissions.
//ls-r outputs the sorting results backward, for example, the original file name from small to large, and the reverse file name from large to small.
//ls-R is displayed together with subdirectories, which means that all files in this directory will be displayed (hiding files should be displayed with-a parameter).

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>

#define PARAM_NONE 0 //Since bitwise operations are used later, let's introduce flag=0 when there are no parameters; then - a,-l,-R,-r, defined as 1, 2, 4, 8.
#define PARAM_A    1 //It happens to be 1, 10, 100, 1000 in binary. This is convenient for | and & in operations, such as having both a and r parameters, then flag is 1001.
#define PARAM_L    2 //Flag & PARAM_r can be used to determine whether or not one of these two parameters (e.g., R parameter) is included. If it is zero, there is no r parameter.
#define PARAM_R    4
#define PARAM_r    8
#define MAXROWLEN  80

char PATH[PATH_MAX+1]; //PATH_MAX is in linux/limits.h, default value is 4096B

int flag;

int g_leave_len = MAXROWLEN;
int g_maxlen;

void my_err(const char* err_string, int line);
void display_dir(char* path);

void my_err(const char* err_string, int line)
{
	fprintf(stderr, "line: %d", __LINE__);
	perror(err_string);
	exit(1);
}

void cprint(char* name, mode_t st_mode)
{
	if(S_ISLNK(st_mode)) //Link file
		printf("\033[1;36m%-*s\033[0m",g_maxlen,name);
	else if(S_ISDIR(st_mode) && (st_mode & 000777) == 0777) //Directories with full permissions
		printf("\033[1;34;42m%-*s  \033[0m",g_maxlen,name);
	else if(S_ISDIR(st_mode))  //Catalog
                printf("\033[1;34m%-*s  \033[0m",g_maxlen,name);
	else if(st_mode&S_IXUSR||st_mode&S_IXGRP||st_mode&S_IXOTH) //Executable file
        	printf("\033[1;32m%-*s  \033[0m",g_maxlen,name);
    	else   //Other documents
        	printf("%*s  ",g_maxlen,name);
}

void  display_attribute(char* name)  //Print the -l parameter in the corresponding format
{
    struct stat buf; //struct stat can be found in man7.org
    char buff_time[32];
    struct passwd* psd;  //Receive the user name of the file owner from this structure
    struct group* grp;   //Get group name
    if(lstat(name,&buf)==-1) //lstat() function gets status information about a specified file and places it in the area of memory pointed to by buf
    {
        my_err("stat",__LINE__);
    }
    if(S_ISLNK(buf.st_mode))
        printf("l");
    else if(S_ISREG(buf.st_mode))
        printf("-");
    else if(S_ISDIR(buf.st_mode))
        printf("d");
    else if(S_ISCHR(buf.st_mode))
        printf("c");
    else if(S_ISBLK(buf.st_mode))
        printf("b");
    else if(S_ISFIFO(buf.st_mode))
        printf("f");
    else if(S_ISSOCK(buf.st_mode))
        printf("s");
    //Get Print File Owner Rights
    if(buf.st_mode&S_IRUSR)
        printf("r");
    else
        printf("-");
    if(buf.st_mode&S_IWUSR)
        printf("w");
    else
        printf("-");
    if(buf.st_mode&S_IXUSR)
        printf("x");
    else
        printf("-");

    //All group permissions
    if(buf.st_mode&S_IRGRP)
        printf("r");
    else
        printf("-");
    if(buf.st_mode&S_IWGRP)
        printf("w");
    else
        printf("-");
    if(buf.st_mode&S_IXGRP)
        printf("x");
    else
        printf("-");

    //Other people's rights
    if(buf.st_mode&S_IROTH)
        printf("r");
    else
        printf("-");
    if(buf.st_mode&S_IWOTH)
        printf("w");
    else
        printf("-");
    if(buf.st_mode&S_IXOTH)
        printf("x");
    else
        printf("-");

    printf("  ");
    //Get the user name and group name of the file owner based on uid and gid
    psd=getpwuid(buf.st_uid);
    grp=getgrgid(buf.st_gid);
    printf("%4lu ",buf.st_nlink);  //Link number
    printf("%-8s ",psd->pw_name);
    printf("%-8s ",grp->gr_name);

    printf("%6ld",buf.st_size);
    strcpy(buff_time,ctime(&buf.st_mtime));
    buff_time[strlen(buff_time)-1]='\0'; //Buffe_time has its own newline, so you need to remove the following newline character
    printf("  %s  ",buff_time);
    cprint(name,buf.st_mode);
    printf("\n");
}
void  displayR_attribute(char* name)  //When l and R are both available, display_attribute is first called to print, and then the function is responsible for recursion.
{
    struct stat buf;

    if(lstat(name,&buf)==-1)
    {
        my_err("stat",__LINE__);
    }
    if(S_ISDIR(buf.st_mode))
    {
            display_dir(name);
            free(name);
            char* p=PATH;
            while(*++p);
            while(*--p!='/');
            *p='\0';             //Return the original path to the previous one after each recursion is completed
            chdir("..");         //Jump to the top level of the current directory
            return;
    }
}
void display_single(char* name)   //Print document
{
    int len ;
    struct stat buf;
    if(lstat(name,&buf)==-1)
    {
        return;
    }

    if(g_leave_len<g_maxlen)
    {
        printf("\n");
        g_leave_len=MAXROWLEN;
    }

    cprint(name,buf.st_mode);  //Display different colors according to different types of files
    g_leave_len=g_leave_len-(g_maxlen+2);

}

void displayR_single(char* name)  //Print the file name and call display_dir when the - R parameter is present
{
    int len ;
    struct stat buf;
    if(lstat(name,&buf)==-1)
    {
        return;
    }
    if(S_ISDIR(buf.st_mode))
    {
            printf("\n");

            g_leave_len=MAXROWLEN;

            display_dir(name);
            free(name);   //Release space from previous filenames[i]
            char*p=PATH;

            while(*++p);
            while(*--p!='/');
            *p='\0';
            chdir("..");  //Return to the upper directory
    }

}

void display(char **name ,int count)  //Call different functions according to flag
{
    switch(flag)
    {
        int i;
        case PARAM_r:
        case PARAM_NONE:
            for(i=0;i<count;i++)
            {
                if(name[i][0]!='.') //Exclude...,..., and hide files
                    display_single(name[i]);
            }
            break;
        case PARAM_r+PARAM_A:
        case PARAM_A:
            for(i=0;i<count;i++)
            {
                display_single(name[i]);
            }
            break;
        case PARAM_r+PARAM_L:
        case PARAM_L:
        for(i=0;i<count;i++)
        {
            if(name[i][0]!='.')
            {
                display_attribute(name[i]);

            }
        }
            break;
        case PARAM_R+PARAM_r:
        case PARAM_R:
            for(i=0;i<count;i++)
            {
                if(name[i][0]!='.')
                {
                    display_single(name[i]);
                }
            }
            for(i=0;i<count;i++)
            {
                if(name[i][0]!='.')   //Exclude the directories of'. 'and'.' to prevent the dead cycle. The same below
                {
                    displayR_single(name[i]);
                }
            }
            break;
        case PARAM_L+PARAM_r+PARAM_R:
        case PARAM_R+PARAM_L:
            for(i=0;i<count;i++)
            {
                if(name[i][0]!='.')
                {
                    display_attribute(name[i]);
                }
            }
            for(i=0;i<count;i++)
            {
                if(name[i][0]!='.')
                {
                    displayR_attribute(name[i]);
                }
            }
            break;
        case PARAM_A+PARAM_r+PARAM_R:
        case PARAM_R+PARAM_A:
            for(i=0;i<count;i++)
            {
                    display_single(name[i]);
            }
            for(i=0;i<count;i++)
            {
                if(name[i][0]!='.')
                {
                    displayR_single(name[i]);
                }
            }
            break;

        case PARAM_A+PARAM_L+PARAM_r:
        case PARAM_A+PARAM_L:
            for(i=0;i<count;i++)
            {
                display_attribute(name[i]);
            }
            break;
        case PARAM_A+PARAM_L+PARAM_R+PARAM_r:
        case PARAM_A+PARAM_L+PARAM_R:
            for(i=0;i<count;i++)
            {
                display_attribute(name[i]);
            }
            for(i=0;i<count;i++)
            {
                if(name[i][0]!='.')
                {
                    displayR_attribute(name[i]);
                }
            }
            break;
        default:
            break;
    }
}


void display_dir(char* path)      //This function is used to process directories
{
    DIR* dir;                     //Accept the file descriptor returned by opendir
    struct dirent* ptr;           //Structures that accept readdir returns
    int count=0;
    //char filenames[300][PATH_MAX+1],temp[PATH_MAX+1]; here is the optimized code, because of the variables defined in the function
    //It allocates space on the stack, so it consumes space on the stack when it is called many times, which eventually leads to stack overflow. The performance on linux is that the core has been dumped.
    //And some directories have far more than 300 files.

    if((flag&PARAM_R)!=0)               //As an obsessive-compulsive disorder, no format problems are allowed.
    {
        int len =strlen(PATH);
        if(len>0)
        {
            if(PATH[len-1]=='/')
                PATH[len-1]='\0';
        }
        if(path[0]=='.'||path[0]=='/')
        {
            strcat(PATH,path);
        }
        else
        {
            strcat(PATH,"/");
            strcat(PATH,path);
        }
        printf("%s:\n",PATH);
    }
    //Get the number of files and the longest file name length 
    dir = opendir(path);
    if(dir==NULL)
        my_err("opendir",__LINE__);
    g_maxlen=0;
    while((ptr=readdir(dir))!=NULL)
    {
        if(g_maxlen<strlen(ptr->d_name))
            g_maxlen=strlen(ptr->d_name);
        count++;
    }
    closedir(dir);
    char **filenames=(char**)malloc(sizeof(char*)*count),temp[PATH_MAX+1];  //Dynamic allocation of storage space on the heap is achieved by the number of files in the directory. First, an array of pointers is defined.
    for(int i=0;i<count;i++)                                                //Then let each pointer in the array point to the allocated space in turn. Here is the optimization, which is effective.
    {                                                                       //It prevents stack overflow and allocates memory dynamically, which saves more space.
        filenames[i]=(char*)malloc(sizeof(char)*PATH_MAX+1);
    }

    int i,j;
    //Get all file names in this directory
    dir=opendir(path);
    for(i=0;i<count;i++)
    {
        ptr=readdir(dir);
        if(ptr==NULL)
        {
            my_err("readdir",__LINE__);
        }
        strcpy(filenames[i],ptr->d_name);
    }
    closedir(dir);
    //Sorting File Names by Bubble Method
    if(flag&PARAM_r)  //Reverse sorting of-r parameters
    {
        for(i=0;i<count-1;i++)
        {
            for(j=0;j<count-1-i;j++)
            {
                if(strcmp(filenames[j],filenames[j+1])<0)
                {
                    strcpy(temp,filenames[j]);
                    strcpy(filenames[j],filenames[j+1]);
                    strcpy(filenames[j+1],temp);
                }
            }
        }
    }
    else //Forward sort
    {
        for(i=0;i<count-1;i++)
                {
                        for(j=0;j<count-1-i;j++)
                        {
                                if(strcmp(filenames[j],filenames[j+1])>0)
                                {
                                        strcpy(temp,filenames[j]);
                                        strcpy(filenames[j],filenames[j+1]);
                                        strcpy(filenames[j+1],temp);
                                }
                        }
                }

    }

    if(chdir(path)<0)
    {
        my_err("chdir",__LINE__);
    }

    display(filenames,count);
    if((flag&PARAM_L==0&&!(flag|PARAM_R)))
        printf("\n");
}

int main(int argc,char** argv)
{
    int i,j,k=0,num=0;
    char param[32]="";     //Used to save command line parameters
    char *path[1];         //Save the path, which I don't want to define as an array of pointers, but it's the only way to define a function parameter corresponding to the char ** type that follows
    path[0]=(char*)malloc(sizeof(char)*(PATH_MAX+1));   //Since it's a pointer type, we need to allocate space PATH_MAX to it. It's a system-defined macro with a value of 4096. Strictly, add 1 to store'\0'.
    flag=PARAM_NONE|PARAM_L;       //Initialize flag=0 (since flag is a full play variable, you don't need to initialize)
    struct stat buf;       //The structure and lstat,stat, in sys/types.h, sys/stat.h, unistd.h, specific man 2 stat

    //Command line parameter parsing, save the following parameters to param
    for(i=1;i<argc;i++)
    {
        if(argv[i][0]=='-')
        {
            for(j=1;j<strlen(argv[i]);j++)
            {
                param[k]=argv[i][j];
                k++;
            }
            num++;
        }
    }
    param[k]='\0';

    /* Judgement parameter */
    for(i=0;i<k;i++)
    {
        if(param[i]=='a')
            flag|=PARAM_A;
        else if(param[i]=='l')
            flag|=PARAM_L;
        else if(param[i]=='R')
            flag|=PARAM_R;
        else if(param[i]=='r')
            flag|=PARAM_r;
        else
        {
            printf("my_ls:invalid option -%c\n",param[i]);
            exit(0);
        }
    }

    //If no target file or directory is entered, the current directory is displayed
    if(num+1==argc)
    {
        strcpy(path[0],".");
        display_dir(path[0]);
        return 0;
    }

    i=1;
    do
    {
        if(argv[i][0]=='-')
        {
            i++;
            continue;
        }
        else
        {
            strcpy(path[0],argv[i]);
            //Determine whether a directory or file exists
            if(stat(argv[i],&buf)==-1)
            {
                my_err("stat",__LINE__);    //The compiler's built-in macro inserts the current source line number
            }
            if(S_ISDIR(buf.st_mode))  //Determine whether it is a catalog
            {
                display_dir(path[0]);
                i++;
            }
            else
            {
                display(path,1);
                i++;
            }
        }
    }while(i<argc);
    printf("\n");

    return 0;

}
