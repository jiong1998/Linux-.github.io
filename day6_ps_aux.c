#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/fcntl.h>

//实现ps aux|grep bash
int main() {
    // 1. 开通管道
    int fd[2];
    int result=pipe(fd);
    if(result<0)
    {
        perror("pipe error");
        return -1;
    }
    //2. 创建子进程
    pid_t pid=fork();

    if(pid<0)
    {
        perror("pipe error");
        return -1;
    }
    if(pid>0)//父进程
    {
        //父进程关闭读端
        close(fd[0]);
        //标准输出重定向到管道写端
        dup2(fd[1],STDOUT_FILENO);
        execlp("ps", "ps", "aux", NULL);

        perror("execlp error");//当execlp执行成功，则不会执行后续代码。若执行失败，打印错误
        wait(NULL);
    }
    if(pid==0)//子进程
    {
        //子进程关闭写端
        close(fd[1]);
        //标准输入重定向到管道读端
        dup2(fd[0],STDIN_FILENO);
        execlp("grep","grep","bash", NULL);
        perror("execlp error");
    }
    return 0;
}
