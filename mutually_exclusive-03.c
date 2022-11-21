#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
int a = 1;

pthread_mutex_t w_mutex = PTHREAD_MUTEX_INITIALIZER;

void *w_thread(void *arg) // cpu 2
{
  pthread_mutex_lock(&w_mutex);
  for (int i = 1; i < 10000; i++) // Angka 10k diperlukan untuk sengaja memperlambat komputasi cpu. Sehingga cpu 2 dan 3 tampak tabrakan.
    a++;
  printf("w_thread %d\n", a); // Print ini harus di luar loop karena i/o itu lambat dibanding kecepatan cpu. Kalau di dalam loop cpu 2 ada kemungkinan menang terus atau kalah terus.
  pthread_mutex_unlock(&w_mutex);
  return NULL;
}

void *w_thread2(void *arg) // cpu 3
{
  pthread_mutex_lock(&w_mutex);
  for (int i = 1; i < 10000; i++)
    a--;
  printf("w_thread2 %d\n", a);
  return NULL;
  pthread_mutex_unlock(&w_mutex);
}

int main() // cpu 1
{
  pthread_t thread;
  pthread_t thread2;
  a = 20000;
  
  int status = pthread_create(&thread, NULL, w_thread, NULL);
  sleep(0.1); // memastikan supaya cpu 2 baris 10 menang lawan baris 20.
  pthread_create(&thread2, NULL, w_thread2, NULL);
  printf("status %d\n", status); // Kadang output hanya baris ini, karena cpu 1 terlalu cepat prosesnya. Sedangkan main adalah pemimpin dan cpu 2 child process yang sedang sibuk dan terlalu lama dan gak ada perintah main untuk menunggu child.
  pthread_join(thread2, NULL);   // Dengan perintah ini, cpu 2 akan ditunggu sampai return, baru cpu 1 lanjut ke baris 21 (dalam hal ini a = 7).
  pthread_join(thread, NULL);
  printf("main %d\n", a);
  return 0;
}