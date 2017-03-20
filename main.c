#include "utils.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>


#define STUDENT_NUM 5
#define MAX_SLEEP 5
#define SEAT_NUM 3

Queue *queue;

void* student_entry(void *studentID);
void* TA_entry();
void printProgramming(int ID);
int main(int argc, char ** argv){
	queue = make_queue(SEAT_NUM+1);
	pthread_t *students;
	pthread_t TA;

	int *studentIDs;
	srand(time(NULL)); 
	int num = STUDENT_NUM;
	printf("Total number of students is %d\n",num);

	students = (pthread_t*)check_malloc(sizeof(pthread_t)*num);
	studentIDs = (int*)check_malloc(sizeof(int)*num);

	pthread_create(&TA,NULL,TA_entry,NULL);
	int i;
	for (i=0;i<num;i++){
		studentIDs[i]=i;
		pthread_create(&students[i],NULL,student_entry,(void*)&studentIDs[i]);
	}
	pthread_join(TA, NULL); 
 	 
 	for(i=0; i<num;i++) 
 	{ 
 		pthread_join(students[i],NULL); 
 	}	 
	return 0;
}
void* student_entry(void *studentID){
	int ID = *(int*)studentID;
	while(1){
		printProgramming(ID);
		queue_student(queue,ID);
	}
}
void printProgramming(int ID){
	int time = rand() % MAX_SLEEP + 1; 
 	sleep(time); 
 	printf("\tStudent %d programming for %d seconds\n",ID,time);
}
void* TA_entry(){
	while(1){
		queue_TA(queue);
	}
}