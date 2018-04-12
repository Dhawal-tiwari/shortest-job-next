#include<stdio.h>

#include<unistd.h>

#include<stdlib.h>

#include<malloc.h>

#include<time.h>

#include<pthread.h>

#include<semaphore.h>

struct Process {

	int time,Arrtime,Burtime,id;

	clock_t arrival;

	int flag,completed,p;

	sem_t se;

	struct Process *next;

	int priority;

	int WT;

};

int i=0,k=0;

typedef struct Process node;

clock_t start;

float A_TAT=0,A_WT=0;

node *Y=NULL,*X=NULL,*tem;

void *processor(node *S) {

	clock_t count;

	while(1) {

		sem_wait(&S->se);

		if((S->Arrtime<=(clock()-start)/CLOCKS_PER_SEC && S->p==1)) {

			S->p=0;

			count=clock();

		}

		if(S->flag==1) {

			printf("\nProcess-%d Running \nTimer :%d",S->id,(clock()-start)/CLOCKS_PER_SEC);

			S->flag=0;

			S->arrival=clock();

		}

		if((clock()-count)/CLOCKS_PER_SEC==1) {

			count=clock();

			printf("\nTimer :%d",(clock()-start)/CLOCKS_PER_SEC);

			S->time-=1;

			if(S->time==0) {

				S->WT=((clock()-start)/CLOCKS_PER_SEC)-S->Burtime-S->Arrtime;

				A_TAT+=(clock()-start)/CLOCKS_PER_SEC-S->Arrtime;

				A_WT+=((clock()-start)/CLOCKS_PER_SEC)-S->Burtime-S->Arrtime;

				sleep(2);

				node *start=X;

				while(start!=NULL) {

					if(start->next==S) {

						start->next=S->next;

						break;

					}

					if(X==S) {

						X=X->next;

						break;

					}

					start=start->next;

				}

				printf("\nProcess-%d Completed",S->id);

				if(X!=NULL){

					printf("next Process-%d",X->id);

				}

				S->completed=7;

				if(X!=NULL){

					sem_post(&X->se);

				}

			}

			}

					if(S->completed==7) {

			break;

		}

	sem_post(&S->se);

	}

} 

void spush(node *tem) {

	int k;

	node *start=X;

	k=tem->priority;

	k=1+(tem->WT/tem->time);

	if(X==NULL) {

		X=tem;

		X->next=NULL;

	}

	else{

	int t=tem->time;

	if(start->priority<k){

		tem->next = X;

        X=tem;

	}

	else if (start->time > t) {

        	tem->next = X;

        	X=tem;

    	}

    else {

        	while (start->next != NULL && start->next->time< t) {

            		start = start->next;

        	}

        tem->next = start->next;

        start->next = tem;

    }

	}

}

void push() {

	tem=(node *)malloc(sizeof(node));

	printf("\nEnter Arrival Time of %d Process :",(i+1));

	scanf("%d",&tem->Arrtime);

	printf("Enter Burst Time :");

	scanf("%d",&tem->time);

	tem->id=i+1;

	tem->p=1;

	tem->flag=1;

	tem->Burtime=tem->time;

	tem->priority=1+(tem->WT/tem->time);

	sem_init(&tem->se,0,0);

	int t=tem->Arrtime;

	node *start=Y;

	if (start->Arrtime > t) {

        	tem->next = Y;

        	Y=tem;

    	}

    	else {

        	while (start->next != NULL && start->next->Arrtime < t) {

            		start = start->next;

        	}

        tem->next = start->next;

        start->next = tem;

    }

}

void main() {

	printf("\t\t\t\t\t\t\t\t\t\t*********Shortest Job Next-Priority**********\n");

	int n,l=1;

	pthread_t p[10];

	printf("\nEnter No.of Processes :");

	scanf("%d",&n);

	while(i<n) {

		if(Y==NULL) {

			Y=(node *)malloc(sizeof(node));

			printf("Enter Arrival Time of %d Process :",(i+1));

			scanf("%d",&Y->Arrtime);

			printf("Enter Burst Time :");

			scanf("%d",&Y->time);

			Y->id=i+1;

			Y->flag=1;

			Y->p=1;

			Y->WT=0;

			Y->Burtime=Y->time;

			Y->priority=1+(Y->WT/Y->time);

			sem_init(&Y->se,0,0);

			Y->next=NULL;

		}

		else {

			push();

		}

		i++;

	}

	i=0;

	start=clock();

	while(i<n) {

		tem=Y;	

		if(tem->Arrtime<=0) {

			printf("Process-%d is Dicarded Due to Incorrect Arrival Time\n",tem->id);

			Y=tem->next;

			tem=Y;

			i++;

		}

		if(l==1) {

				l=0;

				sem_post(&tem->se);

			}

		if((clock()-start)/CLOCKS_PER_SEC==tem->Arrtime) {

		printf("Process-%d is created\n",tem->id);

			pthread_create(&p[i],NULL,processor,tem);

			Y=Y->next;

			spush(tem);

			i++;

		}}

	for(i=0;i<n;i++) {

		pthread_join(p[i],NULL);

	}

	printf("\nAverage Waiting Time :%f\nAverage Turn Around Time :%f",(float)A_WT/n,(float)A_TAT/n);

}
