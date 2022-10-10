#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <sys/errno.h>

void test1()
{
    for(int i=0;i<3;++i)
    {
        pid_t pid=fork();
        if(pid<0)
        {
            printf("子进程创建失败");
            exit(0);
        }
        if(pid>0)
        {
            printf("father:pid=%d\n", getpid());
        }
        if(pid==0)
        {
            printf("child:pid=%d,father:pid=%d\n", getpid(),getppid());
            break;
        }
    }

}

int homework()
{
    pid_t pid;
    int i;
    for(i=0;i<3;++i)
    {
        pid=fork();
        if(pid < 0)
        {
            perror("fork error");
            return -1;
        }
        if(pid>0)
        {
            printf("this is father process,father_pid=[%d],cpid=[%d]\n", getpid(),pid);
        }
        if(pid==0) {
            printf("this is child process,father_pid=[%d],cpid=[%d]\n", getppid(), getpid());
            break;//子进程直接退出循环
        }
    }

    if(i==3)//父进程
    {
        pid_t w_pid;
        while (1)
        {
            w_pid=waitpid(-1,NULL,WNOHANG);
            if(w_pid==-1)
            {
                break;
            }
            if(w_pid>0)
            {
                printf("已回收子进程pid=%d\n",w_pid);
            }
        }
    }
//    if(i==0)//第一个子进程
//    {
//        execlp("ps","ps","aux",NULL);
//    }
    if(i==1)//第二个子进程
    {
        execl("./main", "main",NULL);
        perror("perror error");
    }
//    if(i==2)//第三个子进程
//    {
//        printf("这是第三个子进程,fpid=[%d],cpid=[%d]",getppid(),getpid());
//    }
}

int main()
{
    homework();

    return 0;
}
