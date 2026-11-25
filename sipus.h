// sipus.h - definisi tipe data dan prototipe fungsi
// SiPus: Sistem Peminjaman Perpustakaan Mini
// Kelompok 7 - Kelas E - Pemrograman dan Struktur Data
// Program Studi Teknologi Informasi, Fakultas Teknik, Universitas Udayana, 2026
//
// Mahasiswa A : Pande Made Dhilla Aprilia Erista    (NIM 260530911141)
// Mahasiswa B : I Gusti Ayu Nandini Amara Santi     (NIM 260530911060)
// Repository  : https://github.com/ayunandiniamarasanti-ctrl/Kelompok7
//
// Riwayat versi:
//   Milestone 1-2 : F1 dan F2 (beserta array dinamis ketiga daftar).
//   Milestone 3-4 : F3-F8 dan fitur tambahan penyimpanan berkas (fstream).
// Seluruh struct, atribut, konstanta, dan prototipe dari PDF dosen
// dipertahankan. Tambahan kelompok selalu diberi keterangan "tambahan".
#ifndef SIPUS_H
#define SIPUS_H

#include <string>
using namespace std;

// ======================================================================
// Konstanta (sesuai PDF dosen)
// ======================================================================
const int JML_KATEGORI = 5;    // Ukuran baris rekap
const int JML_BULAN = 12;      // Ukuran kolom rekap
const int MAKS_PINJAM = 3;     // Batas buku/anggota
const int DENDA_HARIAN = 1000; // Rupiah per hari

// ----------------------------------------------------------------------
// Konstanta tambahan kelompok (ASUMSI, tidak ada di PDF dosen)
// ----------------------------------------------------------------------
const int KAPASITAS_AWAL = 4;  // PDF: kapasitas awal array dinamis maksimal 4
const int TAHUN_MIN = 1900;    // Batas bawah tahun terbit yang dianggap valid
const int TAHUN_MAKS = 2026;   // Batas atas tahun terbit (tahun proyek)
const int LAMA_PINJAM = 7;     // Lama peminjaman (hari), aturan bisnis PDF
const int TAHUN_TRX_MIN = 2000; // Rentang tahun tanggal transaksi yang diterima
const int TAHUN_TRX_MAKS = 2100;

// ======================================================================
// Definisi struct (sesuai PDF dosen, atribut tidak diubah)
// ======================================================================
struct Tanggal {
    int hari, bulan, tahun;
};

struct Buku {
    string kode;        // Format "BK-001"
    string judul;
    string penulis;
    int kategori;       // 0..JML_KATEGORI-1
    int tahun;
    int stok;           // Eksemplar tersedia
    int totalDipinjam;  // Untuk statistik
};

struct Anggota {
    string id;          // Format "AG-001"
    string nama;
    string prodi;
    int sedangPinjam;   // Maksimum MAKS_PINJAM
};

struct Transaksi {
    int nomor;
    string idAnggota;
    string kodeBuku;
    Tanggal tglPinjam;
    Tanggal tglKembali; // Diisi saat buku kembali
    bool selesai;
    int denda;
    // Tambahan kelompok: salinan kategori buku saat dipinjam (diisi F3).
    // Dipakai F7 agar transaksi lama tetap terhitung walaupun bukunya
    // kemudian dihapus. Nilai -1 berarti "belum diketahui" (F7 lalu
    // mencari kategori dari daftar buku).
    int kategori;
};

// Pembungkus array dinamis: alamat, isi, kapasitas
struct DaftarBuku {
    Buku* data; int n, kapasitas;
};
struct DaftarAnggota {
    Anggota* data; int n, kapasitas;
};
struct DaftarTransaksi {
    Transaksi* data; int n, kapasitas;
};

// ======================================================================
// Tipe bantu (tambahan kelompok)
// ======================================================================

// Hasil penghapusan buku dengan pemeriksaan transaksi aktif.
enum StatusHapus {
    HAPUS_OK = 0,
    HAPUS_TIDAK_ADA,       // kode buku tidak ditemukan
    HAPUS_MASIH_DIPINJAM   // masih ada transaksi aktif (selesai == false)
};

// Hasil pembacaan masukan dari papan ketik.
enum HasilBaca {
    BACA_OK = 0,           // masukan valid terbaca
    BACA_KOSONG,           // baris kosong (hanya jika diizinkan)
    BACA_EOF               // masukan berakhir (Ctrl+Z / Ctrl+D / pipa habis)
};

// Alasan hasil pemeriksaan peminjaman (F3). Dipakai untuk pesan yang jelas.
enum StatusPinjam {
    PINJAM_OK = 0,
    PINJAM_ANGGOTA_TIDAK_ADA,   // ID anggota tidak terdaftar
    PINJAM_BUKU_TIDAK_ADA,      // kode buku tidak terdaftar
    PINJAM_STOK_HABIS,          // stok buku 0
    PINJAM_BATAS_TERCAPAI,      // anggota sudah meminjam MAKS_PINJAM buku
    PINJAM_TANGGAL_TIDAK_VALID, // tanggal kalender tidak valid
    PINJAM_GAGAL_MEMORI         // array transaksi tidak dapat diperbesar
};

// Alasan hasil pemeriksaan pengembalian (F4).
enum StatusKembali {
    KEMBALI_OK = 0,
    KEMBALI_TIDAK_ADA,          // nomor transaksi tidak ditemukan
    KEMBALI_SUDAH_SELESAI,      // transaksi sudah dikembalikan (cegah ganda)
    KEMBALI_TANGGAL_TIDAK_VALID,// tanggal kalender tidak valid
    KEMBALI_SEBELUM_PINJAM      // tanggal kembali lebih awal dari tanggal pinjam
};

// Kriteria pengurutan buku (F6).
enum KriteriaUrut {
    URUT_KODE = 1,              // kode naik (A-Z)
    URUT_JUDUL,                 // judul naik (A-Z), tanpa membedakan huruf
    URUT_DIPINJAM               // totalDipinjam turun (paling sering dipinjam di atas)
};

// ======================================================================
// Modul Buku (Mahasiswa A) - F1 : IMPLEMENTASI di buku.cpp
// ======================================================================

// ---- Prototipe wajib PDF dosen yang termasuk F1 ----
void inisialisasi(DaftarBuku& d, int kapasitasAwal);
void bebaskan(DaftarBuku& d);
bool tambahBuku(DaftarBuku& d, const Buku& b);
bool hapusBuku(DaftarBuku& d, const string& kode);
Buku* cariKode(DaftarBuku& d, const string& kode);

// ---- Prototipe tambahan kelompok untuk melengkapi F1 ----
const Buku* cariKode(const DaftarBuku& d, const string& kode);
// Mengembalikan pesan kesalahan, atau string kosong jika data valid.
string validasiBuku(const DaftarBuku& d, const Buku& b);
string validasiIsiBuku(const string& judul, const string& penulis,
                       int kategori, int tahun, int stok);
// Mengubah judul, penulis, kategori, tahun, stok. Kode dan totalDipinjam tetap.
bool ubahBuku(DaftarBuku& d, const string& kode, const string& judul,
              const string& penulis, int kategori, int tahun, int stok);
// Hapus dengan pemeriksaan transaksi aktif (aturan bisnis).
StatusHapus hapusBuku(DaftarBuku& d, const DaftarTransaksi& t,
                      const string& kode);
void tampilBuku(const DaftarBuku& d);

// Antarmuka interaktif F1
void prosesTambahBuku(DaftarBuku& d);
void prosesUbahBuku(DaftarBuku& d);
void prosesHapusBuku(DaftarBuku& d, const DaftarTransaksi& t);
void menuBuku(DaftarBuku& d, const DaftarTransaksi& t);

// ======================================================================
// Modul Anggota (Mahasiswa B) - F2 : IMPLEMENTASI di anggota.cpp
// ======================================================================
void inisialisasi(DaftarAnggota& d, int kapasitasAwal);
void bebaskan(DaftarAnggota& d);
bool tambahAnggota(DaftarAnggota& d, const Anggota& a);
Anggota* cariAnggota(DaftarAnggota& d, const string& id);
const Anggota* cariAnggota(const DaftarAnggota& d, const string& id);
string validasiAnggota(const DaftarAnggota& d, const Anggota& a);
string validasiNamaProdi(const string& nama, const string& prodi);
// Mengubah nama dan prodi. ID dan sedangPinjam tetap.
bool ubahAnggota(DaftarAnggota& d, const string& id, const string& nama,
                 const string& prodi);
void tampilAnggota(const DaftarAnggota& d);

// Antarmuka interaktif F2
void prosesTambahAnggota(DaftarAnggota& d);
void prosesUbahAnggota(DaftarAnggota& d);
void menuAnggota(DaftarAnggota& d);

// ======================================================================
// Pendukung array dinamis transaksi (Mahasiswa B) : transaksi.cpp
// Fondasi memori dan pemeriksaan transaksi aktif (F3/F4 ada di bawah).
// ======================================================================
void inisialisasi(DaftarTransaksi& d, int kapasitasAwal);
void bebaskan(DaftarTransaksi& d);
// Menambah record apa adanya (tanpa aturan bisnis peminjaman).
bool tambahTransaksi(DaftarTransaksi& d, const Transaksi& t);
// true bila ada transaksi dengan kodeBuku == kode dan selesai == false.
bool bukuMasihDipinjam(const DaftarTransaksi& t, const string& kode);

// ======================================================================
// Utilitas masukan dan tampilan : util.cpp (kerja bersama)
// ======================================================================
string trim(const string& s);
string hurufBesar(const string& s);
bool formatKodeBuku(const string& kode);   // BK-xxx
bool formatIdAnggota(const string& id);    // AG-xxx
bool parseInt(const string& s, int& hasil);
HasilBaca bacaBaris(const string& prompt, string& hasil);
HasilBaca bacaTeks(const string& prompt, string& hasil, bool bolehKosong);
HasilBaca bacaInt(const string& prompt, int minimum, int maksimum,
                  int& hasil, bool bolehKosong);
string labelKategori(int kategori);
string potong(const string& s, int lebar);
string garis(int panjang);

// ---- Utilitas tanggal (tambahan, dipakai F3, F4, F7) ----
bool tahunKabisat(int tahun);
int jumlahHariBulan(int bulan, int tahun);       // 0 jika bulan tidak valid
bool tanggalValid(const Tanggal& tgl);           // juga cek rentang TAHUN_TRX
bool parseTanggal(const string& s, Tanggal& hasil); // "3 11 2026", "3/11/2026", "3-11-2026"
string formatTanggal(const Tanggal& tgl);        // "03/11/2026"
HasilBaca bacaTanggal(const string& prompt, Tanggal& hasil, bool bolehKosong);
string hurufKecil(const string& s);

// ======================================================================
// Modul Buku (Mahasiswa A) - F5 dan F6 : IMPLEMENTASI di buku.cpp
// ======================================================================
// ---- Prototipe wajib PDF dosen ----
void cariJudul(const DaftarBuku& d, const string& kata);
void urutkanKode(DaftarBuku& d);
int binarySearchKode(const Buku arr[], int kiri, int kanan, const string& kode);

// ---- Tambahan kelompok untuk melengkapi F5 dan F6 ----
// true bila kata ditemukan pada judul (tidak membedakan huruf besar/kecil).
bool judulCocok(const string& judul, const string& kata);
// Jumlah buku yang judulnya cocok (dipakai cariJudul dan uji.cpp).
int hitungCocokJudul(const DaftarBuku& d, const string& kata);
bool terurutKode(const DaftarBuku& d);
void urutkanJudul(DaftarBuku& d);
void urutkanDipinjam(DaftarBuku& d);
void urutkanBuku(DaftarBuku& d, KriteriaUrut kriteria);
void prosesCariJudul(const DaftarBuku& d);
void prosesCariKode(DaftarBuku& d);
void menuCariBuku(DaftarBuku& d);
void menuDaftarBuku(DaftarBuku& d);

// ======================================================================
// Modul Transaksi (Mahasiswa B) - F3, F4, F8 : IMPLEMENTASI di transaksi.cpp
// ======================================================================
// ---- Prototipe wajib PDF dosen ----
bool pinjamBuku(DaftarTransaksi& t, DaftarBuku& b, DaftarAnggota& a,
                const string& idAg, const string& kodeBk, const Tanggal& tgl);
bool kembalikanBuku(DaftarTransaksi& t, DaftarBuku& b, DaftarAnggota& a,
                    int nomor, const Tanggal& tgl);
int selisihHari(const Tanggal& awal, const Tanggal& akhir);
int totalDendaRekursif(const Transaksi arr[], int n);

// ---- Tambahan kelompok untuk melengkapi F3, F4, F8 ----
// Pemeriksaan tanpa mengubah data. pinjamBuku/kembalikanBuku memakai
// fungsi ini lebih dulu, sehingga transaksi gagal tidak mengubah data.
StatusPinjam periksaPinjam(const DaftarBuku& b, const DaftarAnggota& a,
                           const string& idAg, const string& kodeBk,
                           const Tanggal& tgl);
StatusKembali periksaKembali(const DaftarTransaksi& t, int nomor,
                             const Tanggal& tgl);
string pesanPinjam(StatusPinjam s);
string pesanKembali(StatusKembali s);
int nomorBerikutnya(const DaftarTransaksi& t);   // nomor terbesar + 1
Transaksi* cariTransaksi(DaftarTransaksi& t, int nomor);
const Transaksi* cariTransaksi(const DaftarTransaksi& t, int nomor);
// Parameter keluaran: hasil dikirim lewat reference.
void tambahHari(const Tanggal& awal, int jumlahHari, Tanggal& hasil);
void hitungDenda(const Tanggal& tglPinjam, const Tanggal& tglKembali,
                 int& lamaHari, int& hariTerlambat, int& denda);
// Rekursif: mencetak riwayat satu anggota mulai indeks i, mengembalikan
// jumlah transaksi milik anggota tersebut.
int riwayatAnggotaRekursif(const Transaksi arr[], int n, int i,
                           const string& idAg);
void prosesPinjamBuku(DaftarTransaksi& t, DaftarBuku& b, DaftarAnggota& a);
void prosesKembalikanBuku(DaftarTransaksi& t, DaftarBuku& b, DaftarAnggota& a);
void prosesTotalDenda(const DaftarTransaksi& t);
void prosesRiwayatAnggota(const DaftarTransaksi& t, const DaftarAnggota& a);

// ======================================================================
// Modul Laporan (Mahasiswa B) - F7 : IMPLEMENTASI di laporan.cpp
// ======================================================================
// ---- Prototipe wajib PDF dosen (rekap seluruh tahun) ----
void isiRekap(const DaftarTransaksi& t, const DaftarBuku& b,
              int rekap[][JML_BULAN]);
void tampilRekap(const int rekap[][JML_BULAN], const string namaKategori[]);

// ---- Tambahan kelompok: versi dengan tahun laporan ----
// tahun = 0 berarti seluruh tahun (sama dengan versi PDF di atas).
void isiRekap(const DaftarTransaksi& t, const DaftarBuku& b,
              int rekap[][JML_BULAN], int tahun);
void tampilRekap(const int rekap[][JML_BULAN], const string namaKategori[],
                 int tahun);
int totalBaris(const int rekap[][JML_BULAN], int baris);
int totalKolom(const int rekap[][JML_BULAN], int kolom);
int totalRekap(const int rekap[][JML_BULAN]);
int kategoriTerpopuler(const int rekap[][JML_BULAN]); // -1 jika rekap kosong
void prosesLaporan(const DaftarTransaksi& t, const DaftarBuku& b);

// ======================================================================
// Fitur tambahan (a) - penyimpanan berkas : IMPLEMENTASI di penyimpanan.cpp
// ======================================================================
// awalan = awalan nama berkas, mis. "data_" -> data_buku.txt,
// data_anggota.txt, data_transaksi.txt (di folder tempat program dijalankan).
bool simpanData(const DaftarBuku& b, const DaftarAnggota& a,
                const DaftarTransaksi& t, const string& awalan);
// Mengosongkan ketiga daftar lalu memuat ulang dari berkas. Berkas yang
// belum ada dianggap data kosong. Mengembalikan false bila ada baris rusak
// yang dilewati.
bool muatData(DaftarBuku& b, DaftarAnggota& a, DaftarTransaksi& t,
              const string& awalan);

#endif
