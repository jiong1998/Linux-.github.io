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

int test4()
{
    int i=10;
    pid_t pid = fork();
    if(pid<0)
    {
        perror("fork error");
        return -1;
    }
    if(pid>0)
    {
        sleep(1);
        printf("父进程中，i的值为%d\n", i);
        wait(NULL);
    }
    if(pid==0)
    {
        i=2;
        printf("子进程中，i的值为%d\n", i);
    }
    return 0;
}


//利用管道实现父子间的进程通信

int test()
{
    int fd[2];
    int result= pipe(fd);
    if(result<0)
    {
        perror("pipe error");
        return -1;
    }
    pid_t pid=fork();
    if(pid<0)
    {
        perror("fork error");
        return -1;
    }
    else if(pid>0)//父进程
    {
        //关闭管道读端
        close(fd[0]);
        //向管道写入数据
        write(fd[1],"Hello world", sizeof("Hello world"));

        wait(NULL);
//        waitpid(-1,NULL,0);
    }
    else//pid==0，子进程
    {
        //关闭管道写端
        close(fd[1]);
        //向管道读取数据
        char buf[100];
        //初始化buf
        memset(buf,0x00,sizeof(buf));
        int n = read(fd[0], buf, sizeof(buf));
        printf("read over，n==[%d], text=[%s]", n, buf);

    }
    return -1;
}

//设置管道为非阻塞
int test1()
{
    int fd[2];
    int result= pipe(fd);
    if(result<0)
    {
        perror("pipe error");
        return -1;
    }

    //设置为非阻塞
    int flags=fcntl(fd[0], F_GETFL,0);
    flags = flags | O_NONBLOCK;
    fcntl(fd[0], F_SETFL, flags);

    char buf[100];
    memset(buf,0x00,sizeof(buf));
    read(fd[0],buf, sizeof(buf));

    return -1;
}

//FIFO通信

int test2()
{
    //创建FIFO
    int result = mkfifo("./myfifo", 0777);
    int fd= open("myfifo", O_RDWR);
    pid_t pid = fork();
    if(pid>0)//父进程
    {
        write(fd, "HELLO world", sizeof("HELLO world"));
        wait(NULL);
        close(fd);
    }
    if(pid==0)//子进程
    {
        char buf[100];
        memset(buf,0x00, sizeof(buf));
        read(fd, buf, sizeof(buf));
        printf("buf=[%s]", buf);
        close(fd);
    }
}

int main() {
    test4();

}
