//信号集相关函数测试
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <signal.h>
//将 SIGINT 和 SIGQUIT设置阻塞信号集，循环十次后解除阻塞一次，然后在继续阻塞，并且设置信号捕捉函数处理SIGINT 和 SIGQUIT这两个信号，并把所有常规信号的未决状态打印至屏幕

//信号处理函数
void sighandler(int signum)
{
    printf("signal=[%d]\n", signum);
}

int main()
{
    //注册SIGINT和SIGQUIT的信号处理函数
    signal(SIGINT,sighandler);
    signal(SIGQUIT,sighandler);
    //创建信号集
    sigset_t set;//用于将 SIGINT 和 SIGQUIT设置阻塞信号
    sigset_t oldset;//用于保存旧的阻塞信号集
    sigset_t pend;//用于获取当前进程的未决信号集
    //初始化信号集
    sigemptyset(&set);
    sigemptyset(&oldset);

    //将SIGINT和SIGQUIT加入set集合中
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);
    //将信号集set中SIGINT和SIGQUIT信号的加入到阻塞信号集中
    sigprocmask(SIG_BLOCK,&set, &oldset);


    int i;
    int j=1;//计数器
    while(1)
    {

        sigemptyset(&pend);
        //获取未决信号集
        sigpending(&pend);

        for(i=0;i<32;++i)
        {
            if(sigismember(&pend,i)==1)
                printf("1");
            else
                printf("0");
        }
        printf("\n");
        if(j++%10==0)//循环十次，解除阻塞一次，让进程执行信号处理函数
        {
            //从阻塞信号集中解除对SIGINT和SIGQUIT的阻塞
            sigprocmask(SIG_SETMASK,&oldset, NULL);
        }
        else//保持阻塞
        {
            sigprocmask(SIG_BLOCK,&set, NULL);
        }
        sleep(1);
        if(j==21)
            break;
    }
}