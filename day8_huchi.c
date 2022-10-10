//利用互斥锁完成两个线程数数
//需求：定义一个全局变量number，利用互斥锁让每个线程数5000个数，最后全局变量number应该输出10000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

//定义一个互斥锁
pthread_mutex_t mutex;

int number=0;
void * mythread(void * arg)//线程调用函数
{
    int i;
    int j;
    for(i=0;i<5000;++i)
    {
        pthread_mutex_lock(&mutex);//加锁
        j=number;
        j++;
        number=j;
        pthread_mutex_unlock(&mutex);//解锁
    }
    pthread_exit(NULL);
}
int main()
{
    //初始化锁
    pthread_mutex_init(&mutex, NULL);
    //创建子进程
    pthread_t pthread_id[2];
    for (int j=0;j<2;++j)
    {
        int ret = pthread_create(&pthread_id[j], NULL, mythread,NULL);
        if(ret!=0)
        {
            printf("pthread_create error", strerror(ret));
            return -1;
        }
//        pthread_detach(pthread_id[j]);
    }
    pthread_join(pthread_id[0],NULL);
    pthread_join(pthread_id[1],NULL);
    printf("number=%d", number);
    //销毁锁
    pthread_mutex_destroy(&mutex);
    return 1;
}