SiPus - Sistem Peminjaman Perpustakaan Mini
============================================
Tugas Besar Pemrograman dan Struktur Data
Kelompok 7 - Kelas E
Program Studi Teknologi Informasi, Fakultas Teknik, Universitas Udayana, 2026

Anggota kelompok
  Pande Made Dhilla Aprilia Erista     NIM 260530911141   (Mahasiswa A)
  I Gusti Ayu Nandini Amara Santi      NIM 260530911060   (Mahasiswa B)

Repository
  https://github.com/ayunandiniamarasanti-ctrl/Kelompok7


1. DESKRIPSI PROGRAM
--------------------
SiPus adalah aplikasi konsol berbasis menu (C++17) untuk membantu petugas
perpustakaan program studi mengelola data buku, data anggota, transaksi
peminjaman dan pengembalian, serta laporan bulanan. Seluruh data inti
disimpan dalam array of struct dinamis yang dibuat manual (new[] / delete[])
dengan kapasitas awal 4 dan diperbesar dua kali lipat saat penuh.
std::vector, std::list, std::map, dan std::sort TIDAK digunakan.

Aturan bisnis:
  - Kode buku unik berformat BK-xxx, ID anggota unik berformat AG-xxx.
  - Seorang anggota meminjam paling banyak 3 buku bersamaan.
  - Buku hanya dapat dipinjam bila stok > 0.
  - Lama peminjaman 7 hari; denda keterlambatan Rp1.000 per hari.
  - Buku yang masih dipinjam tidak boleh dihapus.


2. FITUR
--------
Menu utama:
  1. Kelola Buku                    (F1)
  2. Kelola Anggota                 (F2)
  3. Daftar Buku dan Pengurutan     (F6)
  4. Pinjam Buku                    (F3)
  5. Kembalikan Buku                (F4)
  6. Cari Buku                      (F5)
  7. Laporan Bulanan                (F7)
  8. Total Denda                    (F8)
  9. Riwayat Transaksi Anggota      (F8)
  0. Keluar

F1  Kelola buku: tambah, ubah, hapus, tampilkan dalam tabel. Hapus ditolak
    bila buku masih dipinjam (ada transaksi dengan selesai == false).
F2  Kelola anggota: tambah, ubah, tampilkan dalam tabel.
F3  Peminjaman: masukan ID anggota, kode buku, tanggal. Diperiksa: anggota
    terdaftar, buku terdaftar, stok > 0, anggota belum meminjam 3 buku,
    tanggal valid. Bila berhasil: stok -1, totalDipinjam +1, sedangPinjam +1,
    transaksi baru disimpan, ditampilkan nomor transaksi dan batas kembali
    (tanggal pinjam + 7 hari). Bila gagal, alasannya ditampilkan dan tidak
    ada data yang berubah.
F4  Pengembalian: masukan nomor transaksi dan tanggal kembali.
    hariTerlambat = maksimum(0, selisihHari - 7); denda = hariTerlambat x 1000.
    Stok +1, sedangPinjam -1, transaksi ditandai selesai. Pengembalian ganda,
    nomor yang tidak ada, dan tanggal sebelum tanggal pinjam ditolak.
    Perhitungan tanggal benar untuk pergantian bulan, tahun, dan kabisat.
F5  Pencarian: (a) potongan judul tanpa membedakan huruf besar/kecil
    (std::string, find()); (b) kode buku dengan binary search REKURSIF.
    Sebelum binary search, data diurutkan berdasarkan kode bila belum urut.
F6  Pengurutan bubble sort manual yang menukar record Buku utuh:
      - kode   : naik (BK-001, BK-002, ...)
      - judul  : naik A-Z, tanpa membedakan huruf besar/kecil
      - jumlah dipinjam : TURUN (terbanyak di atas)
    Bila nilainya sama, urutan ditentukan kode naik.
F7  Laporan rekap peminjaman per kategori x bulan dengan array 2D
    int rekap[5][12], untuk tahun yang dipilih, berdasarkan TANGGAL PINJAM.
    Menampilkan total baris, total kolom, total keseluruhan, dan kategori
    terpopuler.
F8  Total denda seluruh transaksi (totalDendaRekursif) dan riwayat transaksi
    seorang anggota (riwayatAnggotaRekursif). Keduanya fungsi rekursif.

Fitur tambahan (a): penyimpanan berkas dengan <fstream>
    Data disimpan ke data_buku.txt, data_anggota.txt, dan data_transaksi.txt
    di folder tempat program dijalankan. Data dimuat otomatis saat program
    dibuka dan disimpan setiap kali kembali ke menu utama serta saat keluar.
    Format satu atribut per baris, sehingga judul/nama berspasi aman.
    Berkas yang belum ada dianggap data kosong. Record ganda atau rusak
    dilewati dengan peringatan. Nomor transaksi berikutnya = nomor terbesar
    + 1, sehingga tetap unik setelah program dijalankan kembali.
    Untuk memulai dari data kosong, hapus ketiga berkas data_*.txt.

Penanganan masukan: semua masukan dibaca dengan getline() lalu divalidasi.
Huruf pada kolom angka, angka di luar rentang, format kode salah, dan tanggal
yang tidak ada di kalender hanya menampilkan pesan lalu diminta ulang. Pada
kolom kode/ID/tanggal, baris kosong berarti batal. Jika masukan berakhir
(EOF), program menyimpan data dan keluar dengan rapi.


3. STRUKTUR FILE
----------------
  kode/
    sipus.h              Konstanta, struct, enum, dan seluruh prototipe fungsi
    main.cpp             Menu utama, muat/simpan data, pembebasan memori
    buku.cpp             F1, F5, F6 + array dinamis DaftarBuku   (Mahasiswa A)
    anggota.cpp          F2 + array dinamis DaftarAnggota        (Mahasiswa B)
    transaksi.cpp        F3, F4, F8 + array dinamis DaftarTransaksi (Mahasiswa B)
    laporan.cpp          F7 rekap array 2D                       (Mahasiswa B)
    penyimpanan.cpp      Fitur tambahan: simpan/muat berkas      (bersama)
    util.cpp             Pembaca masukan, validasi format, tanggal (bersama)
    uji.cpp              Pengujian otomatis (punya main() sendiri)
    sesi_semua_menu.txt  Masukan contoh untuk sesi pengujian memori seluruh menu
    sesi_eof.txt         Masukan contoh yang berhenti di tengah (uji EOF)
    README.txt           Berkas ini
  dokumen/
    Rancangan-SiPus-Kelompok07.docx
  bukti/
    (tangkapan layar hasil pengujian dan pengujian memori)

main.cpp dan uji.cpp masing-masing memiliki main(), jadi keduanya
dikompilasi TERPISAH. Jangan menyertakan keduanya dalam satu perintah.


4. KOMPILASI DAN MENJALANKAN DI VS CODE (Windows, GCC/MinGW)
------------------------------------------------------------
Buka folder kode/ di VS Code, lalu buka terminal (Terminal > New Terminal).
Pastikan terminal berada di folder kode/ (perintah: cd kode bila perlu).
Periksa compiler: g++ --version

Kompilasi program utama:
  g++ -std=c++17 -Wall -Wextra -pedantic main.cpp buku.cpp anggota.cpp transaksi.cpp laporan.cpp penyimpanan.cpp util.cpp -o sipus

Menjalankan:
  PowerShell / terminal VS Code : .\sipus.exe
  Command Prompt                : sipus.exe
  Linux / macOS / WSL           : ./sipus

Catatan: tampilan memakai karakter ASCII biasa, jadi aman di terminal Windows.


5. PENGUJIAN OTOMATIS (uji.cpp)
-------------------------------
Kompilasi:
  g++ -std=c++17 -Wall -Wextra -pedantic uji.cpp buku.cpp anggota.cpp transaksi.cpp laporan.cpp penyimpanan.cpp util.cpp -o uji

Menjalankan:
  PowerShell : .\uji.exe          Linux/WSL : ./uji

Setiap kasus mencetak [LULUS] atau [GAGAL], diakhiri ringkasan:
  Total pengujian : ...
  Jumlah lulus    : ...
  Jumlah gagal    : ...
Kode keluar 0 bila semua lulus, 1 bila ada yang gagal. Pengujian membuat
berkas sementara uji_tmp_*.txt dan menghapusnya kembali; data_*.txt milik
program utama tidak disentuh.

Cakupan: F1, F2, pembesaran ketiga array dinamis, peminjaman berhasil/gagal
(stok 0, anggota/buku tidak terdaftar, buku keempat, tanggal tidak valid),
hapus buku yang dipinjam, pengembalian tepat waktu/terlambat/ganda,
selisih hari lintas bulan/tahun/kabisat, pencarian judul, binary search
(ada/tidak ada), pengurutan tiga kriteria, rekap array 2D, total denda dan
riwayat rekursif, simpan/muat berkas, serta pembebasan memori.


6. PENGUJIAN MEMORI
-------------------
PDF mewajibkan bukti AddressSanitizer atau Valgrind untuk uji.cpp dan satu
sesi program utama yang mencakup seluruh menu.

PENTING untuk Windows: GCC MinGW-w64 umumnya TIDAK menyediakan
AddressSanitizer (kompilasi dengan -fsanitize=address akan gagal di tahap
linking). Pilih salah satu cara berikut.

Cara A (disarankan) - WSL (Ubuntu di Windows):
  1) Pasang WSL sekali saja (PowerShell sebagai Administrator):
       wsl --install
     lalu restart dan buat user Ubuntu.
  2) Di terminal Ubuntu:
       sudo apt update
       sudo apt install g++ valgrind
  3) Masuk ke folder kode (drive C: ada di /mnt/c), contoh:
       cd "/mnt/c/Users/<nama>/Documents/TB-PSD-Kelompok07/kode"
  4) AddressSanitizer:
       g++ -std=c++17 -Wall -Wextra -pedantic -g -fsanitize=address,undefined uji.cpp buku.cpp anggota.cpp transaksi.cpp laporan.cpp penyimpanan.cpp util.cpp -o uji_asan
       g++ -std=c++17 -Wall -Wextra -pedantic -g -fsanitize=address,undefined main.cpp buku.cpp anggota.cpp transaksi.cpp laporan.cpp penyimpanan.cpp util.cpp -o sipus_asan
       ./uji_asan
       rm -f data_*.txt
       ./sipus_asan < sesi_semua_menu.txt
     Bersih bila tidak ada laporan "ERROR: AddressSanitizer",
     "LeakSanitizer", atau "runtime error".
  5) Valgrind (alternatif/tambahan):
       g++ -std=c++17 -Wall -g uji.cpp buku.cpp anggota.cpp transaksi.cpp laporan.cpp penyimpanan.cpp util.cpp -o uji_g
       g++ -std=c++17 -Wall -g main.cpp buku.cpp anggota.cpp transaksi.cpp laporan.cpp penyimpanan.cpp util.cpp -o sipus_g
       valgrind --leak-check=full ./uji_g
       rm -f data_*.txt
       valgrind --leak-check=full ./sipus_g < sesi_semua_menu.txt
     Bersih bila tertulis "All heap blocks were freed -- no leaks are
     possible" dan "ERROR SUMMARY: 0 errors".

Cara B - MSVC (Visual Studio / Build Tools, "Developer Command Prompt"):
       cl /std:c++17 /EHsc /W4 /Zi /fsanitize=address uji.cpp buku.cpp anggota.cpp transaksi.cpp laporan.cpp penyimpanan.cpp util.cpp /Fe:uji_asan.exe
       uji_asan.exe
  (MSVC AddressSanitizer mendeteksi akses memori ilegal, tetapi deteksi
  kebocoran memorinya terbatas. Untuk bukti kebocoran, gunakan Cara A.)

Sesi seluruh menu: sesi_semua_menu.txt berisi masukan untuk semua menu
1-9 dan 0, termasuk masukan salah (huruf pada kolom angka, kode salah,
tanggal 31/02, pinjam buku keempat, stok 0, pengembalian ganda, dsb.).
Jalankan sesi ini dari folder tanpa data_*.txt agar hasilnya sama.
Untuk demonstrasi langsung, sesi juga dapat dilakukan manual.


7. PEMBAGIAN TUGAS
------------------
  Mahasiswa A - Pande Made Dhilla Aprilia Erista (260530911141)
    F1, F5, F6 (buku.cpp), array dinamis DaftarBuku, binary search rekursif
  Mahasiswa B - I Gusti Ayu Nandini Amara Santi (260530911060)
    F2 (anggota.cpp), F3, F4, F8 (transaksi.cpp), F7 (laporan.cpp),
    array dinamis DaftarAnggota dan DaftarTransaksi, fungsi rekursif denda
    dan riwayat
  Bersama
    Rancangan struct, sipus.h, main.cpp, util.cpp, penyimpanan.cpp,
    integrasi, pengujian (termasuk pengujian silang dan memori), laporan,
    dan video.


8. ASUMSI (tidak tertulis rinci di PDF dosen)
---------------------------------------------
  Milestone 1-2:
  - Tahun terbit valid: 1900 sampai 2026 (TAHUN_MIN, TAHUN_MAKS di sipus.h).
  - Nama kategori 0-4 mengikuti contoh laporan di PDF: Pemrograman,
    Basis Data, Jaringan, Matematika, Umum.
  - Kode/ID harus tepat "BK-" atau "AG-" diikuti 3 digit.
  - Buku/anggota baru selalu memulai totalDipinjam / sedangPinjam dari 0.
  - Stok pada menu dibatasi 0 sampai 1.000.000 sebagai pengaman masukan.
  - Stok yang diubah lewat menu Ubah Buku dianggap koreksi jumlah
    eksemplar tersedia oleh petugas.
  Milestone 3-4:
  - Batas kembali = tanggal pinjam + 7 hari. Pengembalian pada hari ke-7
    belum terlambat; hari ke-8 terlambat 1 hari (Rp1.000).
  - Tanggal transaksi yang diterima: tahun 2000 sampai 2100, dan harus ada
    di kalender (31/04 atau 29/02/2026 ditolak).
  - Tanggal kembali boleh sama dengan tanggal pinjam, tetapi tidak boleh
    lebih awal.
  - Struct Transaksi diberi atribut tambahan "kategori" (salinan kategori
    buku saat dipinjam) agar laporan F7 tetap menghitung transaksi lama
    walaupun bukunya sudah dihapus. Seluruh atribut minimal PDF tetap ada.
  - isiRekap dan tampilRekap versi PDF (tanpa tahun) dipertahankan dan
    merekap semua tahun. Ditambahkan versi dengan parameter tahun yang
    dipakai menu Laporan Bulanan.
  - Bila beberapa kategori memiliki total terbanyak yang sama, semuanya
    ditampilkan sebagai kategori terpopuler.
  - Pencarian kode mengurutkan daftar buku berdasarkan kode bila belum urut
    (syarat binary search), sehingga urutan tampilan ikut berubah.
  - Nomor transaksi tidak pernah dipakai ulang (nomor terbesar + 1).
  - Data disimpan otomatis setiap kembali ke menu utama dan saat keluar.
    Bila jendela konsol ditutup paksa saat berada di dalam submenu,
    perubahan di submenu tersebut belum tersimpan.
