#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

void getuidoid(struct stat *buf,char *uname,char *gname)
{                                           //通过etc/passwd文件  通过uid oid 找到对应字段的文件所有者所属组
	int uid;
	int gid;
	char message[50];
	FILE *fp;
	fp=fopen("/etc/passwd","r");
	while((fscanf(fp,"%s",message))!=EOF)
	    {
	  	 sscanf(message,"%*[^:]%*c%*c%*c%d%*c%d",&uid,&gid);
	  	 	if(buf->st_uid==uid)
	  	 	{	
			sscanf(message,"%[^:]",uname);
	  		sscanf(message,"%[^:]",gname);
			}	
		}
	fclose(fp);
	
		
}
void printdir(char *buf,int depth)
{
    int mode;
    DIR *dp; 
    char per[11]={"----------"};
    char uname[20];
    char gname[20];
    struct stat statbuf;
    struct dirent *entry;
    if((dp=opendir(buf))== NULL)            //先判断目录是否存在，获取目录中有关信息内容
      	{
		fprintf(stderr,"cannot open directory %s\n",buf);
		return ;
		}
    chdir(buf);                            //切换到要遍历的目录
    while((entry=readdir(dp))!=NULL)       //读取到目录下的所有文件
     {
	lstat(entry->d_name,&statbuf);        //获取目录的文件信息通过文件名filename获取文件详细信息，并保存在buf所指的结构体stat中   
	if(S_ISDIR(statbuf.st_mode))          //is a directorty
	 {
	     if(strcmp(entry->d_name,".")==0 ||             //如果是. 或者 .. 跳过        
	     	strcmp(entry->d_name,"..")==0)
			continue;
	     printf(" %*s%s/\n",depth,"",entry->d_name);      
	     printdir(entry->d_name,depth+4);                //递归
	 }
	 else                                 // is a file 
	 {	
	 	/*检查权限*/
	     if((mode=(statbuf.st_mode & S_IRUSR))==S_IRUSR)
	     	per[1]='r';
	     if((mode=(statbuf.st_mode & S_IWUSR))==S_IWUSR)
	     	per[2]='w';
	     if((mode=(statbuf.st_mode & S_IXUSR))==S_IXUSR)
			per[3]='x';
	     if((mode=(statbuf.st_mode & S_IRGRP))==S_IRGRP)
	     	per[4]='r';
	     if((mode=(statbuf.st_mode & S_IWGRP))==S_IWGRP)
	     	per[5]='w';
	     if((mode=(statbuf.st_mode & S_IXGRP))==S_IXGRP)
	     	per[6]='x';
	     if((mode=(statbuf.st_mode & S_IROTH))==S_IROTH)
	     	per[7]='r';
	     if((mode=(statbuf.st_mode & S_IWOTH))==S_IWOTH)
	     	per[8]='w';
	     if((mode=(statbuf.st_mode & S_IXOTH))==S_IXOTH)
	     	per[9]='x';
	     per[10]='\0';
	     getuidoid(&statbuf,uname,gname);     //获取文件的uid oid
	     printf("%*s %s %s %s %s\n",depth,"",per,uname,gname,entry->d_name);
      	}
      }
      chdir("..");
      closedir(dp);
}


int main(int argc,char *argv[])
{
      char *topdir=".";                 //默认是当然目录  可以通过命令行更改要遍历目录的位置
      if(argc>=2)
      	topdir=argv[1];
      printdir(topdir,0);
      return 0;
}
