#include <pthread.h>
#include <stdio.h>
#include <unistd.h> 

// how to read mutex line by line

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
  // pthread_mutex_lock(&mutex); // Nunggu di 28 karena 10, sampai 13 dieksekusi
  printf("sharedResource %d\n", sharedResource);
  // pthread_mutex_unlock(&mutex);
  pthread_join(thread, NULL);
}

/*

v.1
Jalan yang akan dilewati oleh mobil-mobil di bawah bukan perempatan. Tapi persimpangan di antara jalan yang bersamping-sampingan (pertigaan lebar).

Ada 2 mobil yang akan melewati pertigaan.

22 = Mobil main menekan tombol mutex dari tempat berangkat sebagai aba-aba untuk bersiap-siap masuk persimpangan.
23 = Tujuan main ternyata sama dengan tujuan mobil foo.
10 = Untuk bersiap-siap masuk persimpangan, foo menekan tombol.
25-26 = Main memberi aba-aba (unlock) telah sampai tujuan dan menekan tombol mutex dari tempat yang dituju, dan berhenti sejenak (sleep).
13 = Foo memberi aba-aba (unlock) telah sampai tujuan dan menekan tombol mutex dari tempat yang dituju.


v.1.1

   | |
  |   |
 / / \ \ 
 | |  | |

Ada dua mobil yang akan melewati pertigaan. Di pertigaan itu ada tombol yang bisa ditekan oleh keduanya supaya tidak tabrakan di ujung pertigaan.
Mobil pertama (main) berangkat lebih awal, sehingga dia sampai di tombol (mutex) itu duluan. Tapi bukan berarti mobil kedua (foo) berhenti, karena dia jalan di jalur lain (pthread_create). Saatnya foo tiba di tombol mutex, dia menekannya sambil memastikan apakah main sudah sampai dengan selamat (unlock). Jika sudah dipastikan, kemudian foo melanjutkan perjalanan ke arah yang sama yang dituju oleh main.

v.1.2

   | |
  |   |
 / / \ \ 
 | |  | |


Ada dua mobil yang akan melewati pertigaan. Awalnya mobil ini berjalan bersamping-sampingan, sampai pada saat mereka harus masuk pertigaan.
Di kedua ujung pertigaan itu ada dua tombol yang bisa ditekan oleh kedua mobil supaya tidak tabrakan.

Mobil pertama (main) berangkat lebih awal, sehingga dia sampai di tombol (mutex) penutup (lock) duluan. Dan foo berhenti. 

Ketika main telah sampai tujuan, dia menekan tombol (mutex) pembuka (unlock)  di ujung satunya dan menekannya, dan penutup yang menghentikan foo terbuka. 

Foo maju kembali dan melakukan hal yang sama seperti main. Kemudian proses ini di tangkap oleh cctv (join)

v.1.3

   | |
  |   |
 / / \ \ 
 | |  | |


Ada dua mobil akan melewati pertigaan. Awalnya mobil ini berjalan bersamping-sampingan, sampai pada saat mereka harus masuk pertigaan.

Di kedua jalur sebelum dan sesudah pertigaan itu ada dua tombol sepasang (buka dan tutup) yang bisa ditekan oleh kedua mobil supaya tidak tabrakan.

Mobil main berangkat lebih awal, sehingga dia sampai di tombol penutup palang duluan. Dan foo berhenti. 

Segera setelah melewati area persimpangan, main harus menekan tombol  pembuka palang, dan palang yang menghentikan foo terbuka. 

Foo maju kembali dan melakukan hal yang sama seperti main, tapi karena main lebih dulu, maka palang yang ditutup foo tidak berpengaruh ke main.

v.1.4

   | |
  |   |
 / / \ \ 
 | |  | |


Ada dua mobil akan melewati pertigaan. Awalnya mobil ini berjalan bersamping-sampingan, sampai pada saat mereka harus masuk pertigaan. [21]

Di kedua jalur sebelum dan sesudah pertigaan itu ada dua tombol (mutex) sepasang (buka / unlock dan tutup / lock) yang bisa ditekan oleh kedua mobil supaya tidak tabrakan. Mobil main berangkat lebih awal, sehingga dia sampai di tombol penutup (lock) palang duluan. Dan foo berhenti. [22-23]

Segera setelah melewati area persimpangan, main harus menekan tombol  pembuka palang, dan palang yang menghentikan foo terbuka. [25]

Foo maju kembali dan melakukan hal yang sama seperti main, tapi karena main lebih dulu, maka palang yang ditutup foo tidak berpengaruh ke main.[10-14]

*/