# SiPus: Sistem Peminjaman Perpustakaan Mini

**Tugas Besar Pemrograman dan Struktur Data**  
Kelompok 7 - Kelas E  
Program Studi Teknologi Informasi, Fakultas Teknik, Universitas Udayana, 2026

| Peran | Nama | NIM |
| --- | --- | --- |
| Mahasiswa A | Pande Made Dhilla Aprilia Erista | 260530911141 |
| Mahasiswa B | I Gusti Ayu Nandini Amara Santi | 260530911060 |

Repository: https://github.com/llaamara0809/Kelompok-7

## Deskripsi

SiPus adalah aplikasi konsol berbasis menu (C++17) untuk membantu petugas perpustakaan mengelola data buku, data anggota, peminjaman, pengembalian, dan laporan bulanan. Seluruh data inti disimpan dalam array of struct dinamis (`new[]` / `delete[]`) dengan kapasitas awal 4 yang membesar otomatis. Program tidak memakai `std::vector`, `std::list`, `std::map`, maupun `std::sort`.

## Fitur

| Kode | Fitur | Berkas |
| --- | --- | --- |
| F1 | Kelola buku: tambah, ubah, hapus, tampilkan | `buku.cpp` |
| F2 | Kelola anggota: tambah, ubah, tampilkan | `anggota.cpp` |
| F3 | Peminjaman (stok, batas 3 buku, anggota/buku terdaftar) | `transaksi.cpp` |
| F4 | Pengembalian dan denda Rp1.000/hari setelah 7 hari | `transaksi.cpp` |
| F5 | Cari judul (tidak membedakan huruf) dan binary search rekursif kode | `buku.cpp` |
| F6 | Urutkan berdasarkan kode, judul, jumlah dipinjam (bubble sort) | `buku.cpp` |
| F7 | Laporan rekap kategori x bulan dengan array 2D `rekap[5][12]` | `laporan.cpp` |
| F8 | Total denda dan riwayat transaksi anggota (rekursif) | `transaksi.cpp` |
| Tambahan | Simpan dan muat data ke berkas teks (`fstream`) | `penyimpanan.cpp` |

## Struktur Berkas

```
sipus.h            konstanta, struct, prototipe fungsi
main.cpp           menu utama
buku.cpp           F1, F5, F6
anggota.cpp        F2
transaksi.cpp      F3, F4, F8
laporan.cpp        F7
penyimpanan.cpp    fitur tambahan berkas
util.cpp           pembaca masukan, validasi, tanggal
uji.cpp            pengujian otomatis
README.txt         petunjuk lengkap
```

## Cara Kompilasi dan Menjalankan

Jalankan dari folder utama repo (GCC/MinGW):

```
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp buku.cpp anggota.cpp transaksi.cpp laporan.cpp penyimpanan.cpp util.cpp -o sipus
```

Windows: `sipus.exe` - Linux/WSL: `./sipus`

## Pengujian

```
g++ -std=c++17 -Wall -Wextra -pedantic uji.cpp buku.cpp anggota.cpp transaksi.cpp laporan.cpp penyimpanan.cpp util.cpp -o uji
```

Windows: `uji.exe` - Linux/WSL: `./uji`

`main.cpp` dan `uji.cpp` masing-masing memiliki `main()`, jadi keduanya dikompilasi terpisah. Petunjuk pengujian memori (AddressSanitizer dan Valgrind), daftar asumsi, dan pembagian tugas ada di [README.txt](README.txt).