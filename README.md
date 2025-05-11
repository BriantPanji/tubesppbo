# <center> *TUGAS BESAR* PRAKTIKUM PEMROGRAMAN BERBASIS OBJEK</center>


## *Note*
*Program ini tidak bisa berjalan di terminal yang bukan berasal dari **windows**, seperti **`Command Prompt`** dan **`PowerShell`***

## Anggota
* Panji Briant Depari - 241402034
* Sutanto - 241402037
* M Rizky Fadhillah - 241402001


## Penjelasan
1. Inventory (Inventaris): dimana *user (kasir)* dapat mengatur barang yang ada di *inventaris* melalui fitur *inventaris* yang kemudian akan disimpan ke dalam sebuah file
2. Invoice (Struk/Bon): dimana *user (kasir)* dapat membuat sebuah *invoice* perbelanjaan, dimana setiap *invoice* akan di simpan kedalam file unik.
3. Kasir: terdapat *kelas (class)* khusus untuk kasir sehingga mempermudah user melakukan aktivitas kasir sederhana

## Penggunaan
### Generate database inventaris
Ada dua cara untuk melakukan ini:
1. Jika ingin database kosong: Buat file dengan nama `barang.txt` di direktory `barang/`: `barang/barang.txt`
2. Jika ingin database di-isi otomatis:
    * Di CMD
    ```sh
    $ copy barang\brg_init.txt barang\barang.txt
    ```
    * Di PowerShell dan Unix-like terminal lainnya
    ```sh
    $ cp barang/brg_init.txt barang/barang.txt
    ```
    * Copy manual file `barang/brg_init.txt` dari *file explorer* menjadi `barang/barang.txt`

### Mengkompilasi file:
Lakukan perintah:
```sh
$ g++ -okasir PanKasir.cpp Inventory.cpp Invoice.cpp Kasir.cpp main.cpp 
```

### Menjalankan file:
```sh
$ kasir
```
atau
```sh
$ ./kasir
```

### Download Source Code
***[Klik Disini](https://github.com/BriantPanji/tubesppbo/raw/refs/heads/release/tubesppbo.rar)** untuk mendownload source code*

atau

***[Klik Disini](https://github.com/BriantPanji/tubesppbo/raw/refs/heads/release/tubesppbo.rar)** Jika link di atas tidak dapat dibuka*
