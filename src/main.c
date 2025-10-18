#include <stdio.h>
#include <mutex.h>
#include <spink_lock.h>
#include <pthread.h>


int counter = 0;

spin_lock_t spin;

void *thread_func(void *args) {
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

	pthread_t tid1;
	pthread_create(&tid1, NULL, thread_func, NULL);

	pthread_t tid2;
	pthread_create(&tid2, NULL, thread_func, NULL);

	pthread_join(tid1, NULL);

	pthread_join(tid2, NULL);


	printf("%s, %d\n", "counter:", counter);

	sleep(10000);

	return 0;
}
