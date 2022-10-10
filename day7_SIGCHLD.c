#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//SIGCHLD信号处理函数
void waitchild(int signum)
{
    //当收到SIGCHLD信号时，使用waitpid函数回收进程
    pid_t wpid = waitpid(-1,NULL, WNOHANG);//
    if(wpid)
    {
        printf("已利用信号SIGCHLD回收子进程资源,pid=[%d]\n", wpid);
    }
}

//创造出三个进程，每个进程执行自己的事。
//当父进程接收到子进程的SIGCHLD信号时，转向执行信号处理函数，对子进程进程资源的释放
int main_old()
{
    int i;
    for(i=0;i<3;++i)//循环创建三个子进程
    {
        pid_t pid=fork();
        if(pid < 0)
        {
            perror("fork error");
            return -1;
        }
        if(pid > 0)//父进程
        {
            printf("子进程创建成功,子进程pid=[%d]\n", pid);
        }
        if(pid==0)//子进程
            break;
    }
    if(i==0)//第一个子进程
    {
        printf("the first child, pid=[%d],ppid=[%d]\n", getpid(), getppid());
        sleep(1);//模拟子进程在执行其他事
    }
    if(i==1)//第二个子进程
    {
        printf("the second child, pid=[%d],ppid=[%d]\n", getpid(), getppid());
        sleep(1);//模拟子进程在执行其他事
    }
    if(i==2)//第三个子进程
    {
        printf("the third child, pid=[%d],ppid=[%d]\n", getpid(), getppid());
        sleep(1);//模拟子进程在执行其他事
    }
    if(i==3)//父进程
    {
        printf("the father, pid=[%d]\n", getpid());
        //父进程注册SIGCHLD信号处理函数
        struct sigaction act;
        act.sa_handler=waitchild;//信号处理函数
        //设置sa_mask：在执行信号处理函数时需要阻塞的信号
        sigemptyset(&act.sa_mask);
        act.sa_flags=0;
        //注册信号处理函数
        sigaction(SIGCHLD,&act, NULL);
        while(1)//这个while函数模拟父进程在执行其他事情。
        {
            sleep(5);
        }

    }
}

//SIGCHLD信号处理函数
void waitchild_new(int signum)
{
    //当收到SIGCHLD信号时，使用waitpid函数回收进程
    pid_t wpid ;//
    while(1)//利用循环回收解决僵尸进程问题
    {
        wpid=waitpid(-1,NULL, WNOHANG);
        if(wpid>0)
        {
            printf("已利用信号SIGCHLD回收子进程资源,pid=[%d]\n", wpid);
        }
        if(wpid==0)//目前没有子进程退出
        {
            printf("还有子进程存活\n");
            break;
        }
        if(wpid==-1)
        {
            printf("子进程已经全部回收\n");
            break;
        }
    }
}

//创造出三个进程，每个进程执行自己的事。
//当父进程接收到子进程的SIGCHLD信号时，转向执行信号处理函数，对子进程进程资源的释放
int main()
{
    int i;

    //将SIGCHLD信号阻塞。
    //创建信号集
    sigset_t set;
    //初始化信号集
    sigemptyset(&set);
    //将SIGCHLD加入set集合中
    sigaddset(&set, SIGCHLD);
    //将set集合加入到阻塞信号集
    sigprocmask(SIG_BLOCK, &set,NULL);

    for(i=0;i<3;++i)//循环创建三个子进程
    {
        pid_t pid=fork();
        if(pid < 0)
        {
            perror("fork error");
            return -1;
        }
        if(pid > 0)//父进程
        {
            printf("子进程创建成功,子进程pid=[%d]\n", pid);
        }
        if(pid==0)//子进程
            break;
    }
    if(i==0)//第一个子进程
    {
        printf("the first child, pid=[%d],ppid=[%d]\n", getpid(), getppid());
        sleep(1);//模拟子进程在执行其他事
    }
    if(i==1)//第二个子进程
    {
        printf("the second child, pid=[%d],ppid=[%d]\n", getpid(), getppid());
        sleep(2);//模拟子进程在执行其他事
    }
    if(i==2)//第三个子进程
    {
        printf("the third child, pid=[%d],ppid=[%d]\n", getpid(), getppid());
        sleep(2);//模拟子进程在执行其他事
    }
    if(i==3)//父进程
    {
        printf("the father, pid=[%d]\n", getpid());
        //父进程注册SIGCHLD信号处理函数
        struct sigaction act;
        act.sa_handler=waitchild_new;//信号处理函数
        //设置sa_mask：在执行信号处理函数时需要阻塞的信号
        sigemptyset(&act.sa_mask);
        act.sa_flags=0;
        //注册信号处理函数
        sigaction(SIGCHLD,&act, NULL);
        //SIGCHLD解除阻塞
        sigprocmask(SIG_UNBLOCK, &set, NULL);
        while(1)//这个while函数模拟父进程在执行其他事情。
        {
            sleep(5);//模拟父进程在执行其他事情。
            printf("asd");
        }

    }
}

