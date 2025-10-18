#include <stdio.h>
#include <mutex.h>
#include <spink_lock.h>
#include <pthread.h>


int counter = 0;

spin_lock_t spin;

mutex_t mutex;

void *thread_func_mutex(void *args) {
	mutex_lock(&mutex);
	int i = 0;
	while (i < 100000000) {
		counter++;
		i++;
	}
	mutex_unlock(&mutex);
}

void *thread_func_spin(void *args) {
	spin_lock_lock(&spin);
	int i = 0;
	while (i < 100000000) {
		counter++;
		i++;
	}
	spin_lock_unlock(&spin);
}



int main() {
	spin_lock_init(&spin);
	mutex_init(&mutex);

	pthread_t tid1;
	pthread_create(&tid1, NULL, thread_func_mutex, NULL);

	pthread_t tid2;
	pthread_create(&tid2, NULL, thread_func_spin, NULL);

	pthread_join(tid1, NULL);

	pthread_join(tid2, NULL);


	printf("%s, %d\n", "counter:", counter);

	return 0;
}
