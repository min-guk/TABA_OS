#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <semaphore.h>
#include <stdbool.h>

#define SEM_COUNT 3

int g_nthd = 0;
int g_worker_loop_cnt = 0;

int g_count_arr[SEM_COUNT];
int g_is_working[SEM_COUNT];

sem_t b_semaphore;
sem_t c_semaphore;

static void *work(void* tno);

int main(int argc, char* argv[]){
	pthread_t *thd_arr;
	int thd_cnt;

	if (argc != 3){
		fprintf(stderr, "%s parameter: nthread, worker_loop_count", argv[0]);
		exit(-1);
	}

	g_nthd = atoi(argv[1]);
	g_worker_loop_cnt = atoi(argv[2]);

	thd_arr = malloc(sizeof(pthread_t) * g_nthd);

	sem_init(&b_semaphore, 0, 1);
	sem_init(&c_semaphore, 0, 3);

	for(thd_cnt=0; thd_cnt < g_nthd; thd_cnt++){
		assert(pthread_create(&thd_arr[thd_cnt], NULL, work, (void*) (intptr_t) thd_cnt) == 0);
	}

	for(thd_cnt=0; thd_cnt < g_nthd; thd_cnt++){
		 assert(pthread_join(thd_arr[thd_cnt], NULL) == 0);
	}

	printf("Counter arry: ");
	for (int i = 0; i < SEM_COUNT; i++){
		printf("%d\t", g_count_arr[i]);
	}

	printf("\n");
}

static void *work(void* cnt){
	int thd_cnt = (int)(intptr_t) cnt;
	int count_index;
	int i;

	sem_wait(&c_semaphore);

	sem_wait(&b_semaphore);
	for (i=0; i < SEM_COUNT; i++){
		if (g_is_working[i] == false){
			g_is_working[i] = true;
			count_index = i;
			break;
		}
	}

	sem_post(&b_semaphore);

	for (i=0; i < g_worker_loop_cnt; i++){
		g_count_arr[count_index]++;
	}

	sem_wait(&b_semaphore);
	g_is_working[count_index] = false;
	sem_post(&b_semaphore);

	sem_post(&c_semaphore);

	return NULL;
}
