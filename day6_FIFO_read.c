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

//读
int main()
{
    int fd= open("./myfifo1", O_RDWR);
    if(fd<0)
    {
        perror("open error");
        return -1;
    }
    //读fifo文件
    char buf[100];
    memset(buf,0x00,sizeof(buf));
    read(fd,buf, sizeof (buf));
    printf("buf=[%s]", buf);
    close(fd);

}