//读写锁测试程序
//3个线程不定时写同一全局资源，5个线程不定时读同一全局资源。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

//定义一把锁
pthread_rwlock_t rwlock;

//定义全局变量number
int number=0;

void* write_function(void * arg)
{
    int temp;
    while(1)
    {
        //加写锁
        pthread_rwlock_wrlock(&rwlock);

        temp=number;
        temp++;
        number=temp;

        //解锁
        pthread_rwlock_unlock(&rwlock);
        sleep(rand()%3);
    }
}
void* read_function(void * arg)
{
    int i=*(int *) arg;
    int temp;
    while(1)
    {
        //加写锁
        pthread_rwlock_rdlock(&rwlock);

        temp=number;
        printf("[%ld]---read：[%d]\n", i,temp);

        //解锁
        pthread_rwlock_unlock(&rwlock);
        sleep(rand()%3);
    }
}
int main()
{
    int i;
    int n=8;
    int arr[8];
    pthread_t thread[8];
    //初始化读写锁
    pthread_rwlock_init(&rwlock,NULL);

    //创建3个写线程
    for(i=0;i<3;++i)
    {
        arr[i]=i;
        pthread_create(&thread[i],NULL,write_function, &arr[i]);
    }
    //创建5个读线程
    for(i=3;i<n;++i)
    {
        arr[i]=i;
        pthread_create(&thread[i],NULL,read_function, &arr[i]);
    }
    for(int j=0;j<n;++j)
    {
        pthread_join(thread[j], NULL);
    }
    //销毁读写锁
    pthread_rwlock_destroy(&rwlock);
    return 0;
}