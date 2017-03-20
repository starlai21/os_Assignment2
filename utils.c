#include "utils.h"
//#include <semaphore.h>



Queue *make_queue(int length){
	Queue *queue = (Queue*)check_malloc(sizeof(Queue));
	queue->length = length;
	queue->num = 0;
	queue->array = (int*)check_malloc(length*sizeof(int));
	queue->next_in = 0;
	queue->next_out = 0;
	queue->mutex = make_semaphore(1);
	queue->students = make_semaphore(0);
	queue->TA = make_semaphore(1);
	return queue;
}
void queue_student(Queue *queue,int item){
	semaphore_wait(queue->mutex);
	if(queue_empty(queue)){
		semaphore_wait(queue->TA);	
		//printf("Student %d wakes TA\n", item);
	}
	queue_push(queue,item);
	semaphore_signal(queue->mutex);
}
void printHelping(int num){
	int time = rand() % 3 + 1; 
 	sleep(time); 
 	printf("Helping a student for %d seconds, waiting students = %d\n",time,num-1);
}
void queue_TA(Queue *queue){
	semaphore_wait(queue->students);
	semaphore_wait(queue->mutex);
	printHelping(queue->num);
	queue_pop(queue);
	if (queue_empty(queue)){
		semaphore_signal(queue->TA);
		printf("TA is sleeping\n");
	}
	else{
		printf("TA checking for the next student\n");
	}
	semaphore_signal(queue->mutex);
}
bool queue_full(Queue *queue){
	return increment(queue,queue->next_in) == queue->next_out;
}
bool queue_empty(Queue *queue){
	return queue->next_in == queue->next_out;
}
bool queue_push(Queue *queue, int item){

	//semaphore_wait(queue->mutex);
	if (queue_full(queue)){
		printf("\t\tStudent %d will try later\n", item);
		//semaphore_signal(queue->mutex);
		return false;
	}
	queue->array[queue->next_in] = item;
	queue->next_in = increment(queue,queue->next_in);
	queue->num++;
	printf("\tStudent %d takes a seat waiting = %d\n",item, queue->num);
	semaphore_signal(queue->students);
	//semaphore_signal(queue->mutex);
	return true;
}
int queue_pop(Queue *queue){

	//semaphore_wait(queue->mutex);
	if (queue_empty(queue)){
		//semaphore_signal(queue->mutex);
		return -1;
	}
	int value = queue->array[queue->next_out];
	queue->next_out = increment(queue,queue->next_out);
	queue->num--;
	printf("Student %d receiving help\n",value);
	//semaphore_signal(queue->mutex);
	return value;
}
int increment(Queue *queue, int value){
	return (value+1)%queue->length;
}

void perror_exit (char *s)
{
  perror (s);  exit (-1);
}

void * check_malloc(int size){
	void * p = malloc(size);
	if (p == 0){
		perror("malloc failed.");
		exit(-1);
	}
	return p;
}
//Wrap the sem_init
Semaphore *make_semaphore(int value){
	Semaphore *sem = check_malloc(sizeof(Semaphore));
	int n = sem_init(sem,0,value);
	if (n != 0){
		perror_exit("sem_init failed.");
	}
	return sem;
}

//Wrap the sem_wait
void semaphore_wait(Semaphore *sem){
	int n = sem_wait(sem);
	if (n != 0){
		perror_exit("sem_wait failed.");
	}
}
//Wrap the sem_post
void semaphore_signal(Semaphore *sem){
	int n = sem_post(sem);
	if (n !=0 ){
		perror_exit("sem_post failed.");
	}
}
