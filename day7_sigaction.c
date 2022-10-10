//sigaction函数测试：完成信号的注册

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void sighandle(int signum)
{
    printf("signum=[%d]", signum);
}
int main()
{
    struct sigaction act;
    act.sa_handler=sighandle;//信号处理函数
    //设置sa_flags：在执行信号处理函数时需要阻塞的信号
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,SIGQUIT);//在信号处理函数执行期间，阻塞SIGQUIT信号
    act.sa_flags=0;
    //注册信号捕捉函数
    sigaction(SIGINT,&act,NULL);

    return 0;
}