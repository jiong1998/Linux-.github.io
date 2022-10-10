#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
//主线程循环创建5个子线程，并让子线程判断自己是第几个子线程。

//子线程执行函数
void *mythread(void * arg)
{
    int i= *(int *)arg;
    printf("i=[%d],thread_id=[%ld]\n", i, pthread_self());
}

int main()
{
    pthread_t thread[5];
    int i,ret;
    int arr[5];
    for(i=0;i<5;++i)
    {
        arr[i]=i;
        ret = pthread_create(&thread[i],NULL,mythread,&arr[i]);
        if(ret!=0)
        {
            printf("pthread_create error", strerror(ret));
            return -1;
        }
    }
    sleep(1);
    return 0;
}