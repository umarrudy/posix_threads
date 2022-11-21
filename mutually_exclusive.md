| File      | Description |
| --------- | ----------- |
| `mutually_exclusive.c` | Kadang output hanya baris ini, karena cpu 1 terlalu cepat prosesnya. Sedangkan main adalah pemimpin dan cpu 2 child process yang sedang sibuk dan terlalu lama dan gak ada perintah main untuk menunggu child.|
| `mutually_exclusive-01.c` | Dengan perintah ini, cpu 2 akan ditunggu sampai return, baru cpu 1 lanjut ke baris 21 (dalam hal ini a = 7).|
| `mutually_exclusive-02.c` | Sampai akhir program ini, a (shared memory) tidak bisa ditebak nilai akhirnya. Inilah problem thread tanpa sinkronisasi.|
| `mutually_exclusive-03b.c` | How to read mutex line by line. |
| `mutually_exclusive-03.c` | Meniru `03b` dan membuat protokol sederhana menggunakan mutex dan sleep untuk menyolusikan `02`. Sehingga nilai shared memory bisa diprediksi karena thread1 selesai dulu baru thread2 dijalankan. |
