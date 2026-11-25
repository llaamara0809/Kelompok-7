// transaksi.cpp - array dinamis DaftarTransaksi, F3, F4, dan F8
// Penanggung jawab : I Gusti Ayu Nandini Amara Santi (NIM 260530911060)
// Kelompok 7 - Kelas E
//
// Isi berkas:
//   Bagian 1 (Milestone 1-2) : inisialisasi/bebaskan/tambahTransaksi dan
//                              bukuMasihDipinjam() untuk aturan hapus buku.
//   Bagian 2 (Milestone 3)   : F3 peminjaman, F4 pengembalian dan denda,
//                              F8 total denda dan riwayat anggota (rekursif).
#include <iostream>
#include <iomanip>
#include <new>
#include "sipus.h"

using namespace std;

void inisialisasi(DaftarTransaksi& d, int kapasitasAwal) {
    if (kapasitasAwal < 1) kapasitasAwal = 1;
    d.data = new (nothrow) Transaksi[kapasitasAwal];
    d.n = 0;
    d.kapasitas = (d.data != nullptr) ? kapasitasAwal : 0;
}

void bebaskan(DaftarTransaksi& d) {
    delete[] d.data;
    d.data = nullptr;
    d.n = 0;
    d.kapasitas = 0;
}

static bool perbesar(DaftarTransaksi& d) {
    int kapasitasBaru = (d.kapasitas < 1) ? 1 : d.kapasitas * 2;
    Transaksi* baru = new (nothrow) Transaksi[kapasitasBaru];
    if (baru == nullptr) return false;
    for (int i = 0; i < d.n; i++) {
        baru[i] = d.data[i];
    }
    delete[] d.data;
    d.data = baru;
    d.kapasitas = kapasitasBaru;
    return true;
}

// Menambahkan record apa adanya. Aturan bisnis (stok, batas 3 buku, dsb.)
// akan diterapkan oleh pinjamBuku() pada Milestone 3, bukan di sini.
bool tambahTransaksi(DaftarTransaksi& d, const Transaksi& t) {
    if (d.n >= d.kapasitas) {
        if (!perbesar(d)) return false;
    }
    d.data[d.n] = t;
    d.n++;
    return true;
}

// Sebuah buku dianggap "masih dipinjam" bila ada transaksi untuk kode itu
// yang belum selesai. F3 kelak membuat transaksi dengan selesai == false dan
// F4 menggantinya menjadi true, sehingga fungsi ini tidak perlu diubah.
bool bukuMasihDipinjam(const DaftarTransaksi& t, const string& kode) {
    for (int i = 0; i < t.n; i++) {
        if (t.data[i].kodeBuku == kode && !t.data[i].selesai) return true;
    }
    return false;
}

// ======================================================================
// Bagian 2 - F3, F4, F8 (Milestone 3)
// ======================================================================

// ----------------------------------------------------------------------
// Pencarian transaksi dan nomor unik
// ----------------------------------------------------------------------

static int cariIndeksTransaksi(const DaftarTransaksi& t, int nomor) {
    for (int i = 0; i < t.n; i++) {
        if (t.data[i].nomor == nomor) return i;
    }
    return -1;
}

Transaksi* cariTransaksi(DaftarTransaksi& t, int nomor) {
    int i = cariIndeksTransaksi(t, nomor);
    return (i >= 0) ? &t.data[i] : nullptr;
}

const Transaksi* cariTransaksi(const DaftarTransaksi& t, int nomor) {
    int i = cariIndeksTransaksi(t, nomor);
    return (i >= 0) ? &t.data[i] : nullptr;
}

// Nomor baru = nomor terbesar yang pernah dipakai + 1. Karena dihitung dari
// data yang ada (termasuk data yang dimuat dari berkas), nomor tetap unik
// walaupun program ditutup lalu dijalankan kembali.
int nomorBerikutnya(const DaftarTransaksi& t) {
    int maks = 0;
    for (int i = 0; i < t.n; i++) {
        if (t.data[i].nomor > maks) maks = t.data[i].nomor;
    }
    return maks + 1;
}

// ----------------------------------------------------------------------
// Perhitungan tanggal (F4)
// ----------------------------------------------------------------------

// Mengubah tanggal menjadi "nomor hari" yang dihitung sejak 1 Januari tahun 1.
// Dua tanggal cukup dikurangkan untuk mendapat selisih hari, sehingga
// pergantian bulan, tahun, dan tahun kabisat otomatis ikut terhitung.
static int nomorHari(const Tanggal& tgl) {
    int y = tgl.tahun - 1;
    int total = y * 365 + y / 4 - y / 100 + y / 400;  // hari pada tahun-tahun penuh
    for (int b = 1; b < tgl.bulan; b++) {
        total += jumlahHariBulan(b, tgl.tahun);       // hari pada bulan-bulan penuh
    }
    return total + tgl.hari;
}

// Selisih hari (akhir - awal). Bernilai negatif bila akhir lebih awal.
int selisihHari(const Tanggal& awal, const Tanggal& akhir) {
    return nomorHari(akhir) - nomorHari(awal);
}

// Parameter keluaran: hasil = awal + jumlahHari (maju hari demi hari).
void tambahHari(const Tanggal& awal, int jumlahHari, Tanggal& hasil) {
    hasil = awal;
    for (int i = 0; i < jumlahHari; i++) {
        hasil.hari++;
        if (hasil.hari > jumlahHariBulan(hasil.bulan, hasil.tahun)) {
            hasil.hari = 1;
            hasil.bulan++;
            if (hasil.bulan > 12) {
                hasil.bulan = 1;
                hasil.tahun++;
            }
        }
    }
}

// Parameter keluaran: tiga hasil sekaligus dikirim lewat reference.
//   hariTerlambat = maksimum(0, selisihHari - 7)
//   denda         = hariTerlambat x 1000
void hitungDenda(const Tanggal& tglPinjam, const Tanggal& tglKembali,
                 int& lamaHari, int& hariTerlambat, int& denda) {
    lamaHari = selisihHari(tglPinjam, tglKembali);
    hariTerlambat = lamaHari - LAMA_PINJAM;
    if (hariTerlambat < 0) hariTerlambat = 0;
    denda = hariTerlambat * DENDA_HARIAN;
}

// ----------------------------------------------------------------------
// F3 - Peminjaman
// ----------------------------------------------------------------------

// Hanya MEMERIKSA (semua parameter const), tidak mengubah data apa pun.
StatusPinjam periksaPinjam(const DaftarBuku& b, const DaftarAnggota& a,
                           const string& idAg, const string& kodeBk,
                           const Tanggal& tgl) {
    const Anggota* pa = cariAnggota(a, idAg);
    if (pa == nullptr) return PINJAM_ANGGOTA_TIDAK_ADA;
    const Buku* pb = cariKode(b, kodeBk);
    if (pb == nullptr) return PINJAM_BUKU_TIDAK_ADA;
    if (pb->stok <= 0) return PINJAM_STOK_HABIS;
    if (pa->sedangPinjam >= MAKS_PINJAM) return PINJAM_BATAS_TERCAPAI;
    if (!tanggalValid(tgl)) return PINJAM_TANGGAL_TIDAK_VALID;
    return PINJAM_OK;
}

string pesanPinjam(StatusPinjam s) {
    switch (s) {
        case PINJAM_OK: return "Peminjaman berhasil.";
        case PINJAM_ANGGOTA_TIDAK_ADA: return "ID anggota tidak terdaftar.";
        case PINJAM_BUKU_TIDAK_ADA: return "Kode buku tidak terdaftar.";
        case PINJAM_STOK_HABIS: return "Stok buku habis (stok = 0).";
        case PINJAM_BATAS_TERCAPAI:
            return "Anggota sudah meminjam " + to_string(MAKS_PINJAM) +
                   " buku (batas maksimal).";
        case PINJAM_TANGGAL_TIDAK_VALID: return "Tanggal peminjaman tidak valid.";
        case PINJAM_GAGAL_MEMORI: return "Memori tidak cukup untuk menyimpan transaksi.";
    }
    return "Alasan tidak diketahui.";
}

// Ketiga daftar diterima sebagai reference karena stok buku, jumlah pinjaman
// anggota, dan daftar transaksi diubah sekaligus. Urutan langkah dibuat agar
// kegagalan tidak meninggalkan perubahan setengah jalan:
//   1) periksa semua aturan,  2) simpan transaksi (satu-satunya langkah yang
//   bisa gagal),  3) baru ubah buku dan anggota melalui pointer.
bool pinjamBuku(DaftarTransaksi& t, DaftarBuku& b, DaftarAnggota& a,
                const string& idAg, const string& kodeBk, const Tanggal& tgl) {
    if (periksaPinjam(b, a, idAg, kodeBk, tgl) != PINJAM_OK) return false;

    Buku* pb = cariKode(b, kodeBk);        // pointer ke record asli di array
    Anggota* pa = cariAnggota(a, idAg);

    Transaksi baru;
    baru.nomor = nomorBerikutnya(t);
    baru.idAnggota = idAg;
    baru.kodeBuku = kodeBk;
    baru.tglPinjam = tgl;
    baru.tglKembali = {0, 0, 0};           // belum kembali
    baru.selesai = false;
    baru.denda = 0;
    baru.kategori = pb->kategori;          // salinan untuk laporan F7
    // tambahTransaksi hanya memindahkan array transaksi, sehingga pointer
    // pb dan pa (ke array buku/anggota) tetap sah setelah pemanggilan ini.
    if (!tambahTransaksi(t, baru)) return false;

    pb->stok--;              // operator panah: mengubah data buku asli
    pb->totalDipinjam++;
    pa->sedangPinjam++;
    return true;
}

// ----------------------------------------------------------------------
// F4 - Pengembalian
// ----------------------------------------------------------------------

StatusKembali periksaKembali(const DaftarTransaksi& t, int nomor,
                             const Tanggal& tgl) {
    const Transaksi* tr = cariTransaksi(t, nomor);
    if (tr == nullptr) return KEMBALI_TIDAK_ADA;
    if (tr->selesai) return KEMBALI_SUDAH_SELESAI;
    if (!tanggalValid(tgl)) return KEMBALI_TANGGAL_TIDAK_VALID;
    if (selisihHari(tr->tglPinjam, tgl) < 0) return KEMBALI_SEBELUM_PINJAM;
    return KEMBALI_OK;
}

string pesanKembali(StatusKembali s) {
    switch (s) {
        case KEMBALI_OK: return "Pengembalian berhasil.";
        case KEMBALI_TIDAK_ADA: return "Nomor transaksi tidak ditemukan.";
        case KEMBALI_SUDAH_SELESAI: return "Transaksi ini sudah dikembalikan sebelumnya.";
        case KEMBALI_TANGGAL_TIDAK_VALID: return "Tanggal pengembalian tidak valid.";
        case KEMBALI_SEBELUM_PINJAM:
            return "Tanggal pengembalian lebih awal dari tanggal peminjaman.";
    }
    return "Alasan tidak diketahui.";
}

bool kembalikanBuku(DaftarTransaksi& t, DaftarBuku& b, DaftarAnggota& a,
                    int nomor, const Tanggal& tgl) {
    if (periksaKembali(t, nomor, tgl) != KEMBALI_OK) return false;

    Transaksi* tr = cariTransaksi(t, nomor);
    int lamaHari = 0, hariTerlambat = 0, denda = 0;
    hitungDenda(tr->tglPinjam, tgl, lamaHari, hariTerlambat, denda);

    // Buku yang masih dipinjam tidak dapat dihapus (F1), dan anggota tidak
    // dapat dihapus (F2), sehingga keduanya seharusnya selalu ditemukan.
    // Pemeriksaan nullptr tetap dilakukan sebagai pengaman.
    Buku* pb = cariKode(b, tr->kodeBuku);
    if (pb != nullptr) pb->stok++;
    Anggota* pa = cariAnggota(a, tr->idAnggota);
    if (pa != nullptr && pa->sedangPinjam > 0) pa->sedangPinjam--;

    tr->tglKembali = tgl;
    tr->denda = denda;
    tr->selesai = true;      // mencegah pengembalian ganda
    return true;
}

// ----------------------------------------------------------------------
// F8 - Fungsi rekursif
// ----------------------------------------------------------------------

// Total denda = denda elemen terakhir + total denda (n-1) elemen sebelumnya.
//   Base case      : n <= 0, tidak ada elemen, total 0.
//   Recursive case : arr[n-1].denda + totalDendaRekursif(arr, n-1).
int totalDendaRekursif(const Transaksi arr[], int n) {
    if (n <= 0) return 0;
    return arr[n - 1].denda + totalDendaRekursif(arr, n - 1);
}

static string formatRupiah(int nilai) {
    string angka = to_string(nilai < 0 ? -nilai : nilai);
    string hasil;
    int hitung = 0;
    for (int i = static_cast<int>(angka.size()) - 1; i >= 0; i--) {
        hasil = angka[static_cast<size_t>(i)] + hasil;
        hitung++;
        if (hitung % 3 == 0 && i > 0) hasil = "." + hasil;   // pemisah ribuan
    }
    return (nilai < 0 ? "-Rp" : "Rp") + hasil;
}

static const int W_NOMOR = 6, W_IDAG = 6, W_KODE = 6, W_TGL = 10, W_STATUS = 8, W_DENDA = 10;

static void cetakBarisTransaksi(const Transaksi& tr) {
    cout << left << setw(W_NOMOR) << tr.nomor << ' '
         << setw(W_KODE) << tr.kodeBuku << ' '
         << setw(W_TGL) << formatTanggal(tr.tglPinjam) << ' '
         << setw(W_TGL) << (tr.selesai ? formatTanggal(tr.tglKembali) : "-") << ' '
         << setw(W_STATUS) << (tr.selesai ? "Selesai" : "Dipinjam") << ' '
         << right << setw(W_DENDA) << formatRupiah(tr.denda) << '\n';
}

// Menelusuri array mulai indeks i. Baris dicetak bila milik anggota idAg.
//   Base case      : i >= n, seluruh elemen sudah diperiksa, hasil 0.
//   Recursive case : (1 jika arr[i] cocok, selain itu 0)
//                    + riwayatAnggotaRekursif(arr, n, i + 1, idAg).
int riwayatAnggotaRekursif(const Transaksi arr[], int n, int i,
                           const string& idAg) {
    if (i >= n) return 0;
    int cocok = 0;
    if (arr[i].idAnggota == idAg) {
        cetakBarisTransaksi(arr[i]);
        cocok = 1;
    }
    return cocok + riwayatAnggotaRekursif(arr, n, i + 1, idAg);
}

// ----------------------------------------------------------------------
// Antarmuka interaktif F3, F4, F8
// ----------------------------------------------------------------------

// Membaca ID anggota (untukAnggota = true) atau kode buku (false) sampai
// formatnya benar. Baris kosong berarti batal (mengembalikan false).
static bool bacaKodeFormat(const string& prompt, bool untukAnggota, string& hasil) {
    string teks;
    while (true) {
        if (bacaTeks(prompt, teks, true) != BACA_OK) return false;
        teks = hurufBesar(teks);
        bool formatBenar = untukAnggota ? formatIdAnggota(teks) : formatKodeBuku(teks);
        if (formatBenar) {
            hasil = teks;
            return true;
        }
        cout << ">> Format harus " << (untukAnggota ? "AG-xxx, contoh AG-001"
                                                     : "BK-xxx, contoh BK-001") << ".\n";
    }
}

void prosesPinjamBuku(DaftarTransaksi& t, DaftarBuku& b, DaftarAnggota& a) {
    cout << "\n--- Peminjaman Buku ---\n";
    if (a.n == 0 || b.n == 0) {
        cout << ">> Data anggota atau buku masih kosong. Tambahkan terlebih dahulu.\n";
        return;
    }
    string idAg, kodeBk;
    Tanggal tgl = {0, 0, 0};
    if (!bacaKodeFormat("ID anggota (AG-xxx, kosongkan untuk batal): ", true, idAg)) {
        cout << ">> Dibatalkan.\n";
        return;
    }
    if (!bacaKodeFormat("Kode buku  (BK-xxx, kosongkan untuk batal): ", false, kodeBk)) {
        cout << ">> Dibatalkan.\n";
        return;
    }
    if (bacaTanggal("Tanggal pinjam (hari bulan tahun): ", tgl, true) != BACA_OK) {
        cout << ">> Dibatalkan.\n";
        return;
    }

    StatusPinjam status = periksaPinjam(b, a, idAg, kodeBk, tgl);
    if (status == PINJAM_OK && !pinjamBuku(t, b, a, idAg, kodeBk, tgl)) {
        status = PINJAM_GAGAL_MEMORI;   // satu-satunya kegagalan setelah lolos periksa
    }
    if (status != PINJAM_OK) {
        cout << ">> Peminjaman ditolak: " << pesanPinjam(status) << '\n';
        return;
    }
    const Transaksi& baru = t.data[t.n - 1];
    const Buku* pb = cariKode(b, kodeBk);
    const Anggota* pa = cariAnggota(a, idAg);
    Tanggal batas = {0, 0, 0};
    tambahHari(baru.tglPinjam, LAMA_PINJAM, batas);
    cout << ">> Berhasil. Nomor transaksi: " << baru.nomor << '\n'
         << ">> Batas kembali: " << formatTanggal(batas)
         << " (" << LAMA_PINJAM << " hari)\n"
         << "   Buku    : " << pb->kode << " - " << pb->judul
         << " (sisa stok " << pb->stok << ")\n"
         << "   Anggota : " << pa->id << " - " << pa->nama
         << " (sedang pinjam " << pa->sedangPinjam << "/" << MAKS_PINJAM << ")\n";
}

// Daftar transaksi aktif agar petugas mudah melihat nomor transaksi.
static int tampilTransaksiAktif(const DaftarTransaksi& t) {
    int jumlah = 0;
    for (int i = 0; i < t.n; i++) {
        if (t.data[i].selesai) continue;
        if (jumlah == 0) {
            cout << "Transaksi yang masih aktif:\n"
                 << left << setw(W_NOMOR) << "Nomor" << ' ' << setw(W_IDAG) << "ID" << ' '
                 << setw(W_KODE) << "Kode" << ' ' << setw(W_TGL) << "Pinjam" << ' '
                 << setw(W_TGL) << "Batas" << '\n'
                 << garis(W_NOMOR + W_IDAG + W_KODE + 2 * W_TGL + 4) << '\n';
        }
        Tanggal batas = {0, 0, 0};
        tambahHari(t.data[i].tglPinjam, LAMA_PINJAM, batas);
        cout << left << setw(W_NOMOR) << t.data[i].nomor << ' '
             << setw(W_IDAG) << t.data[i].idAnggota << ' '
             << setw(W_KODE) << t.data[i].kodeBuku << ' '
             << setw(W_TGL) << formatTanggal(t.data[i].tglPinjam) << ' '
             << setw(W_TGL) << formatTanggal(batas) << '\n';
        jumlah++;
    }
    cout << right;
    return jumlah;
}

void prosesKembalikanBuku(DaftarTransaksi& t, DaftarBuku& b, DaftarAnggota& a) {
    cout << "\n--- Pengembalian Buku ---\n";
    if (tampilTransaksiAktif(t) == 0) {
        cout << ">> Tidak ada buku yang sedang dipinjam.\n";
        return;
    }
    int nomor = 0;
    Tanggal tgl = {0, 0, 0};
    if (bacaInt("Nomor transaksi (kosongkan untuk batal): ", 1, 999999999,
                nomor, true) != BACA_OK) {
        cout << ">> Dibatalkan.\n";
        return;
    }
    if (bacaTanggal("Tanggal kembali (hari bulan tahun): ", tgl, true) != BACA_OK) {
        cout << ">> Dibatalkan.\n";
        return;
    }
    StatusKembali status = periksaKembali(t, nomor, tgl);
    if (status != KEMBALI_OK || !kembalikanBuku(t, b, a, nomor, tgl)) {
        cout << ">> Pengembalian ditolak: " << pesanKembali(status) << '\n';
        return;
    }
    const Transaksi* tr = cariTransaksi(t, nomor);
    int lamaHari = 0, hariTerlambat = 0, denda = 0;
    hitungDenda(tr->tglPinjam, tr->tglKembali, lamaHari, hariTerlambat, denda);
    Tanggal batas = {0, 0, 0};
    tambahHari(tr->tglPinjam, LAMA_PINJAM, batas);
    cout << ">> Pengembalian berhasil.\n"
         << "   Nomor transaksi : " << tr->nomor << '\n'
         << "   Anggota / Buku  : " << tr->idAnggota << " / " << tr->kodeBuku << '\n'
         << "   Tanggal pinjam  : " << formatTanggal(tr->tglPinjam) << '\n'
         << "   Batas kembali   : " << formatTanggal(batas) << '\n'
         << "   Tanggal kembali : " << formatTanggal(tr->tglKembali) << '\n'
         << "   Lama pinjam     : " << lamaHari << " hari\n"
         << "   Terlambat       : " << hariTerlambat << " hari\n"
         << "   Denda           : " << formatRupiah(tr->denda) << '\n';
}

void prosesTotalDenda(const DaftarTransaksi& t) {
    cout << "\n--- Total Denda Seluruh Transaksi ---\n";
    int selesai = 0, terlambat = 0;
    for (int i = 0; i < t.n; i++) {
        if (t.data[i].selesai) selesai++;
        if (t.data[i].denda > 0) terlambat++;
    }
    cout << "Jumlah transaksi          : " << t.n << '\n'
         << "Sudah dikembalikan        : " << selesai << '\n'
         << "Dikembalikan terlambat    : " << terlambat << '\n'
         << ">> Total denda (rekursif) : "
         << formatRupiah(totalDendaRekursif(t.data, t.n)) << '\n';
}

void prosesRiwayatAnggota(const DaftarTransaksi& t, const DaftarAnggota& a) {
    cout << "\n--- Riwayat Transaksi Anggota ---\n";
    string idAg;
    if (!bacaKodeFormat("ID anggota (AG-xxx, kosongkan untuk batal): ", true, idAg)) {
        cout << ">> Dibatalkan.\n";
        return;
    }
    const Anggota* pa = cariAnggota(a, idAg);
    if (pa == nullptr) {
        cout << ">> Anggota dengan ID " << idAg << " tidak terdaftar.\n";
        return;
    }
    cout << "Anggota : " << pa->id << " - " << pa->nama << " (" << pa->prodi << ")\n"
         << left << setw(W_NOMOR) << "Nomor" << ' ' << setw(W_KODE) << "Kode" << ' '
         << setw(W_TGL) << "Pinjam" << ' ' << setw(W_TGL) << "Kembali" << ' '
         << setw(W_STATUS) << "Status" << ' ' << right << setw(W_DENDA) << "Denda" << '\n'
         << garis(W_NOMOR + W_KODE + 2 * W_TGL + W_STATUS + W_DENDA + 5) << '\n';
    int jumlah = riwayatAnggotaRekursif(t.data, t.n, 0, idAg);
    cout << right;
    if (jumlah == 0) {
        cout << ">> Anggota ini belum memiliki transaksi.\n";
    } else {
        cout << garis(W_NOMOR + W_KODE + 2 * W_TGL + W_STATUS + W_DENDA + 5) << '\n'
             << "Total: " << jumlah << " transaksi, sedang meminjam "
             << pa->sedangPinjam << " buku.\n";
    }
}
