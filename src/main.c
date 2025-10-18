#include <stdio.h>
#include <mutex.h>
#include <spink_lock.h>

int main() {
	spin_lock_t spin;
	spin_lock_init(&spin);
	
	spin_lock_lock(&spin);
	spin_lock_lock(&spin);
	return 0;
}
