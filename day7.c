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

//signal函数测试：测试给没有读端的PIPE的管道写数据，会产生SIGPIPE信号
void handler(int signum)
{
    printf("signum=[%d]", signum);
}
int test()
{
    int fd[2];
    int ret= pipe(fd);
    if (ret<0)
    {
        perror("pipe error");
        return -1;
    }

    //关闭读端，然后写数据到写端，会产生SIGPIPE信号
    close(fd[0]);
    //注册信号处理函数
    signal(SIGPIPE,handler);
    //向写端写入数据
    write(fd[1],"HElLO", sizeof("HElLO"));
    return -1;
}

//利用alarm测试电脑1秒内能输出多少个数字
int test1()
{
    alarm(1);
    int i=1;
    while(1)
    {
        printf("%d\n",i++);
    }
}

int main()
{
    test1();
    return -1;
}