#include <pthread.h>
#include <stdio.h>

int a = 1;

void *w_thread(void *arg) // cpu 2
{
  a = 2;
  printf("w_thread %d\n", a);
  return NULL;
}

int main() // cpu 1
{
  pthread_t thread;
  a = 3;
  int status = pthread_create(&thread, NULL, w_thread, NULL);
  a = 4;
  printf("status %d\n", status); // Kadang output hanya baris ini, karena cpu 1 terlalu cepat prosesnya. Sedangkan main adalah pemimpin dan cpu 2 child process yang sedang sibuk dan terlalu lama dan gak ada perintah main untuk menunggu child.
  pthread_join(thread, NULL); // Dengan perintah ini, cpu 2 akan ditunggu sampai return, baru cpu 1 lanjut ke baris 21 (dalam hal ini a = 7).
  a = 7;
  printf("main %d\n", a);
  return 0;
}