

#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>


void perror_exit (char *s);
void * check_malloc(int size);
typedef sem_t Semaphore;
Semaphore *make_semaphore(int value);
void semaphore_wait(Semaphore *sem);
void semaphore_signal(Semaphore *sem);

typedef struct{
	int *array;
	int length;
	int num;
	int next_in;
	int next_out;
	Semaphore *mutex;
	Semaphore *TA;
	Semaphore *students;
	
}Queue;

Queue *make_queue(int length);
bool queue_push(Queue *queue, int item);
int queue_pop(Queue *queue);
int increment(Queue *queue, int value);
bool queue_full(Queue *queue);
bool queue_empty(Queue *queue);
void queue_student(Queue *queue,int item);
void queue_TA(Queue *queue);
void printHelping(int num);