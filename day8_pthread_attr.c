//在创建线程的时候设置分离属性
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

void * mythread(void * arg)
{
    printf("chlid pthread[%ld]\n", pthread_self());
}

int main()
{
    pthread_t pthread_id;
    //1 定义线程属性类型的变量
    pthread_attr_t attr;
    //2 对线程属性变量进行初始化
    pthread_attr_init(&attr);
    //3 设置线程为分离属性
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int ret = pthread_create(&pthread_id, &attr, mythread, NULL);
    if(ret!=0)
    {
        printf("pthread_create error", strerror(ret));
        return -1;
    }
    //释放线程属性
    pthread_attr_destroy(&attr);
    printf("father pthread[%ld]\n", pthread_self());
    return 1;
}
