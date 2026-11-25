// main.cpp - menu utama SiPus (F1-F8 dan penyimpanan berkas)
// Kelompok 7 - Kelas E
// Mahasiswa A : Pande Made Dhilla Aprilia Erista (260530911141)
// Mahasiswa B : I Gusti Ayu Nandini Amara Santi  (260530911060)
//
// Alur program:
//   1) inisialisasi tiga array dinamis (kapasitas awal 4)
//   2) muat data dari berkas (fitur tambahan)
//   3) perulangan menu utama; data disimpan setiap kembali ke menu utama
//   4) simpan data, lalu bebaskan seluruh memori dinamis
#include <iostream>
#include "sipus.h"

using namespace std;

// Awalan nama berkas data: data_buku.txt, data_anggota.txt, data_transaksi.txt
static const string AWALAN_BERKAS = "data_";

static void tampilMenuUtama() {
    cout << "\n========================================\n"
         << "      SiPus - PERPUSTAKAAN MINI\n"
         << "========================================\n"
         << "        KELOMPOK 7 - KELAS E\n"
         << "----------------------------------------\n"
         << "1. Kelola Buku\n"
         << "2. Kelola Anggota\n"
         << "3. Daftar Buku dan Pengurutan\n"
         << "4. Pinjam Buku\n"
         << "5. Kembalikan Buku\n"
         << "6. Cari Buku\n"
         << "7. Laporan Bulanan\n"
         << "8. Total Denda\n"
         << "9. Riwayat Transaksi Anggota\n"
         << "0. Keluar\n"
         << "----------------------------------------\n";
}

int main() {
    DaftarBuku buku;
    DaftarAnggota anggota;
    DaftarTransaksi transaksi;

    // Kapasitas awal kecil (4) agar mekanisme pembesaran mudah teruji.
    inisialisasi(buku, KAPASITAS_AWAL);
    inisialisasi(anggota, KAPASITAS_AWAL);
    inisialisasi(transaksi, KAPASITAS_AWAL);

    // Fitur tambahan: memuat data yang tersimpan dari sesi sebelumnya.
    bool muatBersih = muatData(buku, anggota, transaksi, AWALAN_BERKAS);
    cout << ">> Data dimuat: " << buku.n << " buku, " << anggota.n
         << " anggota, " << transaksi.n << " transaksi.\n";
    if (!muatBersih) {
        cout << ">> Peringatan: sebagian isi berkas data rusak atau tidak valid "
             << "dan dilewati.\n";
    }

    int pilihan = 0;
    do {
        tampilMenuUtama();
        if (bacaInt("Masukkan pilihan: ", 0, 9, pilihan, false) != BACA_OK) {
            cout << "\n>> Masukan berakhir. Program ditutup.\n";
            pilihan = 0;
        }
        switch (pilihan) {
            case 1: menuBuku(buku, transaksi); break;
            case 2: menuAnggota(anggota); break;
            case 3: menuDaftarBuku(buku); break;
            case 4: prosesPinjamBuku(transaksi, buku, anggota); break;
            case 5: prosesKembalikanBuku(transaksi, buku, anggota); break;
            case 6: menuCariBuku(buku); break;
            case 7: prosesLaporan(transaksi, buku); break;
            case 8: prosesTotalDenda(transaksi); break;
            case 9: prosesRiwayatAnggota(transaksi, anggota); break;
            case 0: break;
            default: break;
        }
        // Simpan setiap kembali ke menu utama agar data tidak hilang.
        if (!simpanData(buku, anggota, transaksi, AWALAN_BERKAS)) {
            cout << ">> Peringatan: data gagal disimpan ke berkas.\n";
        }
    } while (pilihan != 0);

    cout << "\n>> Data tersimpan di " << AWALAN_BERKAS << "buku.txt, "
         << AWALAN_BERKAS << "anggota.txt, dan " << AWALAN_BERKAS << "transaksi.txt.\n"
         << "Terima kasih telah menggunakan SiPus.\n";

    // Wajib: seluruh memori dinamis dibebaskan sebelum keluar.
    bebaskan(buku);
    bebaskan(anggota);
    bebaskan(transaksi);
    return 0;
}
