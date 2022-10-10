//编写一个守护进程，每隔2S钟获取一次系统时间，并将这个时间写入磁盘文件。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>

int flags=0;

void handler()
{
    //文件只需要打开一次，设置flag来判断是否第一次打开
    int fd;
    if(flags==0)
    {
        fd= open("./time.log",O_RDWR | O_APPEND | O_CREAT, 0755);
        if(fd<0)
            return;
    }
    //获取时间
    time_t t;
    time(&t);
    char * p = ctime(&t);
    //写入文件
    write(fd, p, strlen(p));
    return;
}

int main()
{
    pid_t pid= fork();
    if(pid < 0 || pid>0)//父进程退出
    {
        exit(1);
    }
    //子进程创建会话
    setsid();
    //改变当前工作目录
    chdir("./");//这里不该
    //重设文件掩码
    umask(0000);
    //核心操作:

    //注册信号
    struct sigaction act;
    act.sa_handler= handler;
    act.sa_flags=0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM, &act, NULL);
//    signal(SIGALRM, handler);

    //设置时钟信号
    struct itimerval tm;
    //触发周期赋值
    tm.it_interval.tv_sec=2;//每隔两秒触发一次
    tm.it_interval.tv_usec=0;//微秒，不管他
    //第一次触发事件赋值
    tm.it_value.tv_sec=3;//三秒后第一次触发
    tm.it_value.tv_usec=0;//微秒，不管他
    setitimer(ITIMER_REAL, &tm, NULL);//该函数每隔2s发送一次SIGALRM信号
    while (1)//让守护进程持续运行
    {
        sleep(1);
        //注意：结束程序要先用命令：ps ajx | grep day8_Daemo查找进程号，然后kill -9 进程号 来结束进程
    }
}
