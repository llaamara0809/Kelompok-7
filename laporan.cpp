// laporan.cpp - F7: laporan rekap peminjaman kategori x bulan
// Penanggung jawab : I Gusti Ayu Nandini Amara Santi (NIM 260530911060)
// Kelompok 7 - Kelas E
//
// Struktur data: array 2D int rekap[JML_KATEGORI][JML_BULAN].
//   baris -> kategori buku (0 Pemrograman ... 4 Umum)
//   kolom -> bulan peminjaman (0 = Januari ... 11 = Desember)
// Rekap dihitung dari TANGGAL PINJAM, bukan tanggal kembali.
#include <iostream>
#include <iomanip>
#include "sipus.h"

using namespace std;

static const string NAMA_BULAN[JML_BULAN] = {
    "Jan", "Feb", "Mar", "Apr", "Mei", "Jun",
    "Jul", "Agu", "Sep", "Okt", "Nov", "Des"
};

// ----------------------------------------------------------------------
// Pengisian rekap
// ----------------------------------------------------------------------

// Versi PDF: rekap seluruh transaksi tanpa memilah tahun.
void isiRekap(const DaftarTransaksi& t, const DaftarBuku& b,
              int rekap[][JML_BULAN]) {
    isiRekap(t, b, rekap, 0);
}

// tahun = 0 berarti semua tahun; selain itu hanya transaksi pada tahun itu.
void isiRekap(const DaftarTransaksi& t, const DaftarBuku& b,
              int rekap[][JML_BULAN], int tahun) {
    // Perulangan bersarang: kosongkan seluruh sel terlebih dahulu.
    for (int i = 0; i < JML_KATEGORI; i++) {
        for (int j = 0; j < JML_BULAN; j++) {
            rekap[i][j] = 0;
        }
    }
    for (int k = 0; k < t.n; k++) {
        const Transaksi& tr = t.data[k];
        if (tahun != 0 && tr.tglPinjam.tahun != tahun) continue;

        // Kategori diambil dari salinan pada transaksi, sehingga transaksi
        // lama tetap terhitung walaupun bukunya sudah dihapus. Bila salinan
        // belum ada (-1), kategori dicari dari daftar buku.
        int kategori = tr.kategori;
        if (kategori < 0 || kategori >= JML_KATEGORI) {
            const Buku* pb = cariKode(b, tr.kodeBuku);
            if (pb == nullptr) continue;          // tidak dapat ditentukan
            kategori = pb->kategori;
        }
        int bulan = tr.tglPinjam.bulan;
        if (bulan < 1 || bulan > JML_BULAN) continue;
        rekap[kategori][bulan - 1]++;             // bulan 1..12 -> kolom 0..11
    }
}

// ----------------------------------------------------------------------
// Perhitungan total (parameter array 2D, hanya dibaca -> const)
// ----------------------------------------------------------------------

int totalBaris(const int rekap[][JML_BULAN], int baris) {
    int total = 0;
    for (int j = 0; j < JML_BULAN; j++) total += rekap[baris][j];
    return total;
}

int totalKolom(const int rekap[][JML_BULAN], int kolom) {
    int total = 0;
    for (int i = 0; i < JML_KATEGORI; i++) total += rekap[i][kolom];
    return total;
}

int totalRekap(const int rekap[][JML_BULAN]) {
    int total = 0;
    for (int i = 0; i < JML_KATEGORI; i++) {
        for (int j = 0; j < JML_BULAN; j++) {
            total += rekap[i][j];
        }
    }
    return total;
}

// Indeks kategori dengan total terbesar. Bila seri, indeks terkecil yang
// dipilih. Mengembalikan -1 bila belum ada peminjaman sama sekali.
int kategoriTerpopuler(const int rekap[][JML_BULAN]) {
    int terbaik = -1;
    int maks = 0;
    for (int i = 0; i < JML_KATEGORI; i++) {
        int total = totalBaris(rekap, i);
        if (total > maks) {
            maks = total;
            terbaik = i;
        }
    }
    return terbaik;
}

// ----------------------------------------------------------------------
// Tampilan tabel
// ----------------------------------------------------------------------

void tampilRekap(const int rekap[][JML_BULAN], const string namaKategori[]) {
    tampilRekap(rekap, namaKategori, 0);
}

void tampilRekap(const int rekap[][JML_BULAN], const string namaKategori[],
                 int tahun) {
    const int W_NAMA = 12, W_SEL = 4, W_TOTAL = 6;
    const int LEBAR = W_NAMA + JML_BULAN * W_SEL + W_TOTAL;

    string judul = "REKAP PEMINJAMAN " +
                   (tahun == 0 ? string("SEMUA TAHUN") : to_string(tahun));
    cout << '\n' << garis(LEBAR) << '\n' << judul << '\n'
         << garis(LEBAR) << '\n'
         << left << setw(W_NAMA) << "Kategori" << right;
    for (int j = 0; j < JML_BULAN; j++) cout << setw(W_SEL) << NAMA_BULAN[j];
    cout << setw(W_TOTAL) << "Total" << '\n' << garis(LEBAR) << '\n';

    // Isi tabel: perulangan bersarang baris (kategori) x kolom (bulan).
    for (int i = 0; i < JML_KATEGORI; i++) {
        cout << left << setw(W_NAMA) << namaKategori[i] << right;
        for (int j = 0; j < JML_BULAN; j++) {
            cout << setw(W_SEL) << rekap[i][j];
        }
        cout << setw(W_TOTAL) << totalBaris(rekap, i) << '\n';
    }

    cout << garis(LEBAR) << '\n' << left << setw(W_NAMA) << "Total" << right;
    for (int j = 0; j < JML_BULAN; j++) cout << setw(W_SEL) << totalKolom(rekap, j);
    cout << setw(W_TOTAL) << totalRekap(rekap) << '\n' << garis(LEBAR) << '\n';

    int populer = kategoriTerpopuler(rekap);
    if (populer < 0) {
        cout << ">> Belum ada peminjaman pada periode ini.\n";
        return;
    }
    // Bila beberapa kategori memiliki total yang sama besar, semuanya disebut.
    int maks = totalBaris(rekap, populer);
    cout << ">> Kategori terpopuler: ";
    bool pertama = true;
    for (int i = 0; i < JML_KATEGORI; i++) {
        if (totalBaris(rekap, i) == maks) {
            cout << (pertama ? "" : ", ") << namaKategori[i];
            pertama = false;
        }
    }
    cout << " (" << maks << " peminjaman)\n";
}

// ----------------------------------------------------------------------
// Antarmuka interaktif F7
// ----------------------------------------------------------------------

void prosesLaporan(const DaftarTransaksi& t, const DaftarBuku& b) {
    cout << "\n--- Laporan Bulanan ---\n";
    if (t.n == 0) {
        cout << ">> Belum ada transaksi peminjaman.\n";
        return;
    }
    int tahunMin = t.data[0].tglPinjam.tahun, tahunMaks = tahunMin;
    for (int i = 1; i < t.n; i++) {
        int th = t.data[i].tglPinjam.tahun;
        if (th < tahunMin) tahunMin = th;
        if (th > tahunMaks) tahunMaks = th;
    }
    cout << "Transaksi tercatat pada tahun " << tahunMin;
    if (tahunMaks != tahunMin) cout << " sampai " << tahunMaks;
    cout << ".\n";

    int tahun = 0;
    if (bacaInt("Tahun laporan (kosongkan untuk batal): ", TAHUN_TRX_MIN,
                TAHUN_TRX_MAKS, tahun, true) != BACA_OK) {
        cout << ">> Dibatalkan.\n";
        return;
    }
    string namaKategori[JML_KATEGORI];
    for (int i = 0; i < JML_KATEGORI; i++) namaKategori[i] = labelKategori(i);

    int rekap[JML_KATEGORI][JML_BULAN];
    isiRekap(t, b, rekap, tahun);
    tampilRekap(rekap, namaKategori, tahun);
}
