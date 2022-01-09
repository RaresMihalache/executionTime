#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tinycthread.h>

int a[10000000];
mtx_t mutex;
cnd_t condition;

static volatile int counter = 0;

void* thread_func() {
	return NULL;
}

void th_fun1() {
	for (int i = 0; i < 10; i++) {
		if (counter % 2 == 0){
			mtx_lock(&mutex);
			counter++;
			//printf("a\n");
			cnd_signal(&condition);
		}
		cnd_wait(&condition, &mutex);
		mtx_unlock(&mutex);
		if (counter == 20) {
			cnd_signal(&condition);
			return NULL;
		}
	}
}

void th_fun2(void *args) {
	for (int i = 0; i < 10; i++) {
		if (counter % 2 == 1) {
			mtx_lock(&mutex);
			counter++;
			//printf("b\n");
			cnd_signal(&condition);
		}
		cnd_wait(&condition, &mutex);
		mtx_unlock(&mutex);
		if (counter == 20) {
			cnd_signal(&condition);
			return NULL;
		}
	}
}

int main() {

	mtx_init(&mutex, mtx_plain);
	cnd_init(&condition);
	// Memory access
	clock_t begin_t = clock();
	for (int i = 0; i < 10000000; i++){
		a[i] = 5;
	}
	clock_t end_t = clock();
	double total_time = (double)(end_t - begin_t) / CLOCKS_PER_SEC;
	printf("Time elapsed (accessing memory): %lf\n", total_time);

	// Static memory allocation
	begin_t = clock();
	for (int i = 0; i < 100000; i++) {
		int arr1[100000000];
	}
	end_t = clock();
	total_time = (double)(end_t - begin_t) / CLOCKS_PER_SEC;
	printf("Time elapsed (static memory allocation): %lf\n", total_time);

	// Dynamic memory allocaiton
	begin_t = clock();
	int* arr2 = malloc(100000000 * sizeof(int));
	for (int i = 0; i < 100000; i++) {
		arr2 = realloc(arr2, 100000000 * sizeof(int));
	}
	end_t = clock();
	total_time = (double)(end_t - begin_t) / CLOCKS_PER_SEC;
	printf("Time elapsed (dynamic memory allocation): %lf\n", total_time);

	// Create 10000 threads:

	begin_t = clock();
	thrd_t newThread[10000];
	for (int i = 0; i < 10000; i++) {
		thrd_create(&(newThread[i]), thread_func, NULL);
		int x = 5;
	}
	end_t = clock();
	total_time = (double)(end_t - begin_t) / CLOCKS_PER_SEC;
	printf("Time elapsed (thread creation): %lf\n", total_time);

	// Thread context switch
	begin_t = clock();
	thrd_t newThread1;
	thrd_t newThread2;
	thrd_create(&newThread1, th_fun1, NULL);
	thrd_create(&newThread2, th_fun2, NULL);
	thrd_join(newThread1, NULL);
	thrd_join(newThread2, NULL);
	end_t = clock();
	total_time = (double)(end_t - begin_t) / CLOCKS_PER_SEC;
	printf("Time elapsed (thread context switch): %lf\n", total_time);
	
}
