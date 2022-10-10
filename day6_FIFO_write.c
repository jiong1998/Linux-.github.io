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

//写文件
int main()
{
    //判断FIFO是否存在，如果不存在则创建
    int result= access("./myfifo1", F_OK);
    if(result!=0)//不存在该文件
    {
        result= mkfifo("./myfifo1",0777);
        if(result<0)
        {
            perror("mkfifo error");
            return -1;
        }
    }
    int fd= open("./myfifo1", O_RDWR);
    //写fifo文件
    write(fd,"Hello world", strlen("Hello world"));
    sleep(20);
    close(fd);

}