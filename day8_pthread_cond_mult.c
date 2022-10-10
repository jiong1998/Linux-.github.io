//案例：利用条件变量解决多生产者和多消费者问题
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

void * producer(void * arg)
{
    NODE * pnode= NULL;
    int i=*(int *)arg;
    while(1)
    {
        //生产一个链表
        pnode = (NODE *) malloc(sizeof(NODE));
        pnode->data=rand()%1000;
        //加锁
        pthread_mutex_lock(&mutex);
        pnode->next=head;
        head=pnode;
        printf("[%d]Producing---[%d]\n", i, head->data);
        //解锁
        pthread_mutex_unlock(&mutex);

        //通知消费者线程解除阻塞
        pthread_cond_signal(&cond);
        sleep(rand()%2);

    }
}

void * consumer(void * arg)
{
    NODE * pnode = NULL;
    int i=*(int *)arg;
    while(1)
    {
        //加锁
        pthread_mutex_lock(&mutex);
        while(head==NULL)//通常只需要一部分线程去做执行任务，所以其它的线程需要继续wait。所以这里改成while不要用if
        {
            //如果头节点为空，则阻塞在此并解锁，直到生产者给出信号(被生成者线程调用pthread_cond_signal函数通知)，才唤醒并重新加锁
            pthread_cond_wait(&cond, &mutex);
        }
        pnode=head;
        head=head->next;
        printf("[%d]Consuming---[%d]\n", i, pnode->data);
        //解锁
        pthread_mutex_unlock(&mutex);
        free(pnode);
        pnode=NULL;
        sleep(rand()%3);
    }
}

int main()
{
    int ret;
    int i;
    int P_arr[5];
    int C_arr[5];
    //初始化锁
    pthread_mutex_init(&mutex, NULL);
    //初始化条件变量
    pthread_cond_init(&cond, NULL);

    pthread_t P_pthread[3];
    pthread_t C_pthread[5];
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
    for(i=0;i<5;++i)
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
    //销毁条件变量
    pthread_cond_destroy(&cond);
    return 1;
}