#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

void *mythread(void *arg)
{
    printf("child thread, pid=%d, id=%ld\n", getpid(),pthread_self());
    pthread_exit(NULL);
}
int main()
{
    //创建子进程
    pthread_t pthread_id;
    int ret = pthread_create(&pthread_id, NULL, mythread,NULL);
    if(ret!=0)
    {
        printf("pthread_create error", strerror(ret));
        return -1;
    }
    printf("father thread, pid=%d, id=%ld\n", getpid(),pthread_self());
    pthread_join(pthread_id, NULL);
    return 1;
}

