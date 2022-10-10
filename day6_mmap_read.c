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
int main()
{
    //打开要映射到内存的文件
    int fd= open("./printf.log", O_RDWR);
    int len= lseek(fd, 0 , SEEK_END);
    //建立共享映射区
    void * addr= mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr==MAP_FAILED)//建立失败函数返回MAP_FAILED宏
    {
        perror("mmap error");
        return -1;
    }

    //读
    char buf[1024];
    memset(buf,0x00, sizeof(buf));
    memcpy(buf,addr, 1024);
    printf("buf=[%s]", buf);
}