#include <stdio.h>
#include <mutex.h>
#include <spink_lock.h>
#include <pthread.h>

int counter = 0;

spin_lock_t spin;

mutex_t mutex;

void *thread_func_mutex(void *args) {
	int err;
	
	err = mutex_lock(&mutex);
	if (err != 0) {
		printf("%s\n", "mutex lock failed");
		return NULL;
	}

	int i = 0;
	while (i < 100000000) {
		++counter;
		++i;
	}
	
	err = mutex_unlock(&mutex);
	if (err != 0) {
		printf("%s\n", "mutex unlock failed");
		return NULL;
	}
}

void *thread_func_spin(void *args) {
	int err;

	err = spin_lock_lock(&spin);
	if (err != 0) {
		printf("%s\n", "spin lock failed");
		return NULL;
	}

	int i = 0;
	while (i < 100000000) {
		++counter;
		++i;
	}
	
	err = spin_lock_unlock(&spin);
	if (err != 0) {
		printf("%s\n", "spin unlock failed");
		return NULL;
	}
}



int main() {
	int err;

	err = spin_lock_init(&spin);
	if (err != 0) {
		printf("%s\n", "spin init failed");
		return -1;
	}


	err = mutex_init(&mutex);
	if (err != 0) {
		printf("%s\n", "mutex init failed");
		return -1;
	}

	pthread_t tid1;

	err = pthread_create(&tid1, NULL, thread_func_mutex, NULL);
	if (err != 0) {
		printf("%s\n", "pthread craete failed");
		return -1;
	}

	pthread_t tid2;

	err = pthread_create(&tid2, NULL, thread_func_mutex, NULL);
	if (err != 0) {
		printf("%s\n", "pthread craete failed");
		return -1;
	}

	err = pthread_join(tid1, NULL);
	if (err != 0) {
		printf("%s\n", "pthread join failed");
		return -1;
	}

	err = pthread_join(tid2, NULL);
	if (err != 0) {
		printf("%s\n", "pthread join failed");
		return -1;
	}

	printf("%s, %d\n", "counter:", counter);

	return 0;
}
