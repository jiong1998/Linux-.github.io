//案例：利用信号量解决多线程间的生产者和消费者问题
//需求：假设有5个生产者，4个消费者，生产者每次生产一个苹果到桌子上，消费者每次从桌子上消费一个苹果，假设桌子最多放20个苹果
//利用信号量解决该问题
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>

sem_t empty;//剩余空间信号量
sem_t full;//占用空间信号量
pthread_mutex_t mutex;//互斥锁
int apple_number=0;//苹果的数量

void * producer(void * arg)
{
    int i = *(int *)arg;
    while(1)//生产
    {
        sem_wait(&empty);//生产苹果，剩余空间-1
        pthread_mutex_lock(&mutex);
        sleep(1);
        apple_number++;
        printf("序号[%d]生产了一个苹果,目前桌子上有[%d]个水果\n", i, apple_number);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);//生产完，占用空间+1
        sleep(rand()%2);
    }
}

void * consumer(void * arg)
{
    int i = *(int *)arg;
    while(1)//消费
    {
        sem_wait(&full);//消费苹果，占用空间-1
        pthread_mutex_lock(&mutex);
        sleep(1);
        apple_number--;
        printf("序号[%d]消费了一个苹果,目前桌子上有[%d]个水果\n", i, apple_number);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);//消费完，剩余空间+1
        sleep(rand()%3);
    }
}

int main()
{
    int i,ret;
    int P_arr[5];
    int C_arr[4];
    pthread_mutex_init(&mutex,NULL);
    sem_init(&empty,0,3);//空间多大就设多少
    sem_init(&full,0,0);

    pthread_t P_pthread[5];
    pthread_t C_pthread[4];
    //创造生产者
    for(i=0;i<5;++i)
    {
        P_arr[i]=i;
        ret=pthread_create(&P_pthread[i], NULL, producer,&P_arr[i]);
        if(ret!=0)
        {
            printf("pthread_create error, [%s]\n", strerror(ret));
            return -1;
        }
    }
    //创造消费者
    for(i=0;i<4;++i)
    {
        C_arr[i]=i;
        ret=pthread_create(&C_pthread[i], NULL, consumer,&C_arr[i]);
        if(ret!=0)
        {
            printf("pthread_create error, [%s]\n", strerror(ret));
            return -1;
        }
    }
    //等待线程结束
    for(i=0;i<5;++i)
    {
        pthread_join(P_pthread[i],NULL);
    }
    for(i=0;i<5;++i)
    {
        pthread_join(C_pthread[i],NULL);
    }
    //销毁锁
    pthread_mutex_destroy(&mutex);
    //销毁信号量
    sem_destroy(&empty);
    sem_destroy(&full);
    return 1;
}