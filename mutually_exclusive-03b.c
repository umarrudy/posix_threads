#include <pthread.h>
#include <stdio.h>
#include <unistd.h> // how to read mutex line by line

int sharedResource = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *fooAPI(void *param) // cpu 2
{
  pthread_mutex_lock(&mutex); // Nunggu di 10 karena 22, sampai 25 dieksekusi
  sharedResource = 42;
  printf("Changing the shared resource now. sharedResource: %d\n", sharedResource);
  pthread_mutex_unlock(&mutex);
  return 0;
}

int main() // cpu 1
{
  pthread_t thread;

  // Really not lock-ing for any reason other than to make the point.
  pthread_mutex_lock(&mutex);
  pthread_create(&thread, NULL, fooAPI, NULL);
  sleep(1); // busy: sleep 1 second or loop 1 million 
  pthread_mutex_unlock(&mutex);
  sleep(1); // solusi agar lock 28 pasti didahului lock 10
  // Now we need to lock to use the shared resource.
  pthread_mutex_lock(&mutex); // Nunggu di 28 karena 10, sampai 13 dieksekusi
  printf("sharedResource %d\n", sharedResource);
  pthread_mutex_unlock(&mutex);
  pthread_join(thread, NULL);
}