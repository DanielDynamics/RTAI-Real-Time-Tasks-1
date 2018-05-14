/*
 * RT_Task.c
 *
 *  Created on: Sep 15, 2016
 *      Author: zxdhf
 */

//
//  RT_Tasks.c
//
//
//  Created by Zhentao Xie on 9/14/16.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <rtai.h>
#include <rtai_lxrt.h>

#define N 100
#define M 20
#define Num_of_item 16

char store[M][N];
char buffer[N];
RTIME BaseP;
void *ThreadFunc_Read1(void *ptr);
void *ThreadFunc_Read2(void *ptr);
void *ThreadFunc_Write(void *ptr);

int main()
{
    pthread_t thrd1, thrd2, thrd3;

    BaseP = start_rt_timer(nano2count(10000000)); //base periodic

    pthread_create(&thrd1, NULL, ThreadFunc_Read1, NULL); //create thread for read task 1
    pthread_create(&thrd2, NULL, ThreadFunc_Read2, NULL); //create thread for read task 2
    pthread_create(&thrd3, NULL, ThreadFunc_Write, NULL); //create thread for write task

    pthread_join(thrd1,NULL);
    pthread_join(thrd2,NULL);
    pthread_join(thrd3,NULL);

    stop_rt_timer();
    printf("store matrix is: \n");
    int i;
    for (i=0; i < Num_of_item; i++)
    {
       puts(store[i]);
    }

    return 0;
}

void *ThreadFunc_Read1(void *ptr)
{
    int i=0;
	RT_TASK *readtask1 = rt_task_init(nam2num("thrd1"),0,512,256); //initial task read1
    rt_task_make_periodic(readtask1,rt_get_time()+0*BaseP,400*BaseP);
    FILE *fp1;
    fp1 = fopen("first.txt","r");
    if (fp1==NULL)
    {
        printf("Can't open first.txt\n");
    }
    /*
    while (!feof(fp1))
    {
    	printf("Item of first (%d):\n",i);
    	fgets(buffer,N,fp1); //read 1 line from first.txt, save to buffer
    	puts(buffer);
    	rt_task_wait_period(); //then stop
        i++;
    }
*/
    for (i=0;i<8;i++)
    {
    	printf("Item of first (%d):\n",i);
    	fgets(buffer,N,fp1);
    	puts(buffer);
    	rt_task_wait_period();
    }

}

void *ThreadFunc_Read2(void *ptr)
{
    int i=0;
	RT_TASK *readtask2 = rt_task_init(nam2num("thrd2"),0,512,256); //initial task read2
    rt_task_make_periodic(readtask2,rt_get_time()+200*BaseP,400*BaseP);
    FILE *fp2;
    fp2 = fopen("second.txt","r");
    if (fp2==NULL)
    {
        printf("Can't open second.txt\n");
    }
    /*
    while (!feof(fp2))
    {
    	printf("Item of second(%d):\n",i);
        fgets(buffer,N,fp2); //read 1 line from second.txt, save to buffer
        puts(buffer);
        rt_task_wait_period(); //then stop
        i++;
    }
*/
    for (i=0;i<8;i++)
        {
        	printf("Item of second (%d):\n",i);
        	fgets(buffer,N,fp2);
        	puts(buffer);
        	rt_task_wait_period();
        }

}

void *ThreadFunc_Write(void *ptr)
{
    RT_TASK *writetask = rt_task_init(nam2num("thrd3"),0,512,256); //initial task write
    rt_task_make_periodic(writetask,rt_get_time()+100*BaseP,200*BaseP);
    int i;
    for (i=0; i<Num_of_item; i++)
    {
        strcpy(store[i],buffer);
        rt_task_wait_period();
    }
}






