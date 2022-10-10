//案例：利用条件变量解决单生产者和单消费者问题
//需求：生产者每次生产一个链表，消费者每次消费一个链表。（链表不带头节点）
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

//定义一把锁
pthread_mutex_t mutex;
//定义条件变量
pthread_cond_t cond;
//定义链表
typedef struct node
{
    int data;
    struct node * next;

}NODE;

NODE *head = NULL;

void * producer(void*arg)
{
    NODE * pnode= NULL;
    while(1)
    {
        //生产一个链表
        pnode = (NODE *) malloc(sizeof(NODE));
        pnode->data=rand()%1000;
        //加锁
        pthread_mutex_lock(&mutex);
        pnode->next=head;
        head=pnode;
        printf("Producing---[%d]\n", head->data);
        //解锁
        pthread_mutex_unlock(&mutex);

        //通知消费者线程解除阻塞
        pthread_cond_signal(&cond);
        sleep(rand()%2);

    }
}

void * consumer(void*arg)
{
    NODE * pnode = NULL;
    while(1)
    {
        //加锁
        pthread_mutex_lock(&mutex);
        if(head==NULL)
        {
            //如果头节点为空，则阻塞在此并解锁，直到生产者给出信号(被生成者线程调用pthread_cond_signal函数通知)，才唤醒并重新加锁
            pthread_cond_wait(&cond, &mutex);
        }
        pnode=head;
        head=head->next;
        printf("Consuming---[%d]\n", pnode->data);
        //解锁
        pthread_mutex_unlock(&mutex);
        free(pnode);
        pnode=NULL;
        sleep(rand()%2);
    }
}

int main()
{
    int ret;
    //初始化锁
    pthread_mutex_init(&mutex, NULL);
    //初始化条件变量
    pthread_cond_init(&cond, NULL);

    pthread_t pthread[2];

    ret=pthread_create(&pthread[0], NULL, producer,NULL);
    if(ret!=0)
    {
        printf("pthread_create error, [%s]\n", strerror(ret));
        return -1;
    }
    ret=pthread_create(&pthread[1], NULL, consumer,NULL);
    if(ret!=0)
    {
        printf("pthread_create error, [%s]\n", strerror(ret));
        return -1;
    }

    //等待线程结束
    pthread_join(pthread[0],NULL);
    pthread_join(pthread[1],NULL);

    //销毁锁
    pthread_mutex_destroy(&mutex);
    //销毁条件变量
    pthread_cond_destroy(&cond);
    return 1;
}