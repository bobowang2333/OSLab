#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void do_ls(char [],int depth);
void dostat(char *);
void show_file_info(char *,struct stat *);
void mode_to_letters(int,char[]);
char *uid_to_name(uid_t);
char *gid_to_name(uid_t);
char modestr[11];

int main(int argc,char *argv[])
{
        if(argc == 1)
                do_ls(".",0);
        else
/*
 *依次遍历每一个参数，打印目录下的所有文件
 */
                while(--argc){
                        printf("%s:\n",*++argv);
                        do_ls(*argv,0);
                }
}
 
void do_ls(char dirname[],int depth)
{
/*
 *定义一个目录流,和目录流结构体保存读到的结果。
 */
        DIR *dir_ptr;
        struct dirent *direntp;
        if((dir_ptr = opendir(dirname)) == NULL)
                fprintf(stderr,"ls1:cannot open %s\n",dirname);
        else
        {
                     chdir(dirname);
                while((direntp = readdir(dir_ptr)) != NULL)
        //打印结果{
                {      
                      
                        printf("%d ",depth);
                        dostat(direntp->d_name);
                        if(modestr[0] == 'd')
                        {
                            if((strcmp(direntp->d_name, ".") == 0) ||(strcmp(direntp->d_name, "..") == 0))
                            continue;
                            do_ls(direntp->d_name,depth+1);
                        }
                      
                  }
                  if(depth!=0)
                  {
                  chdir("..");
                 // depth -= 1;
                  }
//printf("%s\n",direntp->d_name);
        //关闭目录流
                closedir(dir_ptr);
        }
}
 
//获取文件信息stat结构体
void dostat(char *filename)
{
        struct stat info;
        if(lstat(filename,&info) == -1)
                perror(filename);
        else
 
//分析stat结构体
                show_file_info(filename,&info);
}
 
void show_file_info(char *filename,struct stat *info_p)
{
        char *uid_to_name(),*ctime(),*gid_to_name(),*filemode();
//      void mode_to_letters();
        //char modestr[11];
        mode_to_letters(info_p->st_mode,modestr);
        printf("%s",modestr);
        printf("%4d ",(int)info_p->st_nlink);
        printf("%-8s",uid_to_name(info_p->st_uid));
        printf("%-8s",gid_to_name(info_p->st_gid));
        printf("%8ld ",(long)info_p->st_size);
        printf("%.12s",4+ctime(&info_p->st_mtime));
        printf(" %s\n",filename);
}
 
//分析mode权限
void mode_to_letters(int mode,char str[])
{
//S_IS***测试宏
        strcpy(str,"----------");
        if(S_ISDIR(mode))str[0] = 'd';
        if(S_ISCHR(mode))str[0] = 'c';
        if(S_ISBLK(mode))str[0] = 'b';
 
//与 掩码
        if(mode&S_IRUSR)str[1] = 'r';
        if(mode&S_IWUSR)str[2] = 'w';
        if(mode&S_IXUSR)str[3] = 'x';
 
        if(mode&S_IRGRP)str[4] = 'r';
        if(mode&S_IWGRP)str[5] = 'w';
        if(mode&S_IXGRP)str[6] = 'x';
 
        if(mode&S_IROTH)str[7] = 'r';
        if(mode&S_IWOTH)str[8] = 'w';
        if(mode&S_IXOTH)str[9] = 'x';
}
// uid gid to name group
#include<pwd.h>
char *uid_to_name(uid_t uid)
{
        struct passwd *getpwuid(),*pw_ptr;
        static char numstr[10];
        if((pw_ptr = getpwuid(uid)) == NULL){
                sprintf(numstr,"%d",uid);
                return numstr;
        }
        else
                return pw_ptr->pw_name;
}
 
#include<grp.h>
char *gid_to_name(gid_t gid)
{
         struct group *getgrgid(),*grp_ptr;
        static char numstr[10];
        if((grp_ptr = getgrgid(gid)) == NULL){
                sprintf(numstr,"%d",gid);
                return numstr;
        }
        else
                return grp_ptr->gr_name;
}
