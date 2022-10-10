#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
//dup2函数的使用
int dup2_test()
{
    int oldfd=open("test.log", O_RDWR);
    if(oldfd<0)
    {
        perror("oppen error");
        return -1;
    }
    int newfd=open("test1.log", O_RDWR);
    if(newfd<0)
    {
        perror("oppen error");
        return -1;
    }
    int result= dup2(oldfd,newfd);

    if(result<0)
    {
        perror("dup2 error");
        return -1;
    }
    //newfd写文件
    result=write(newfd,"Hello word",sizeof("Hello word"));
    if(result<0)
    {
        perror("write error");
        return -1;
    }
    //oldfd读文件
    lseek(oldfd,0,SEEK_SET);
    char buf[100];
    memset(buf,0x00,sizeof(buf));
    result=read(oldfd,buf,sizeof(buf));
    if(result<0)
    {
        perror("write error");
        return -1;
    }
    printf("buf=[%s]", buf);
}

int main(int argc, char * argv[])
{
    //dup2实现文件重定向操作
    //打开文件
    int fd = open(argv[1], O_RDWR | O_CREAT, 0755);
    if(fd<0)
    {
        perror("open error");
        return -1;
    }
    //调用dup2实现文件重定向
    dup2(fd, STDOUT_FILENO);
    printf("你好宝贝");
    return 0;
}