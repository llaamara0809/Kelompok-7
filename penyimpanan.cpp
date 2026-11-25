// penyimpanan.cpp - Fitur tambahan (a): penyimpanan berkas dengan <fstream>
// Kelompok 7 - Kelas E (kerja bersama Mahasiswa A dan B)
//
// Tiga berkas teks dibuat di folder tempat program dijalankan:
//   <awalan>buku.txt, <awalan>anggota.txt, <awalan>transaksi.txt
// Program utama memakai awalan "data_", sedangkan uji.cpp memakai awalan
// lain agar data asli tidak tertimpa.
//
// Format: SATU ATRIBUT PER BARIS. Baris pertama penanda jenis berkas, baris
// kedua jumlah record, lalu atribut tiap record berurutan. Karena satu atribut
// menempati satu baris penuh, judul dan nama yang mengandung spasi tetap utuh
// saat dibaca kembali dengan getline().
//
// Contoh data_buku.txt:
//   SIPUS-BUKU
//   1
//   BK-001          <- kode
//   Struktur Data   <- judul
//   Wati            <- penulis
//   0               <- kategori
//   2024            <- tahun
//   3               <- stok
//   5               <- totalDipinjam
#include <fstream>
#include "sipus.h"

using namespace std;

static const string PENANDA_BUKU = "SIPUS-BUKU";
static const string PENANDA_ANGGOTA = "SIPUS-ANGGOTA";
static const string PENANDA_TRANSAKSI = "SIPUS-TRANSAKSI";

// ----------------------------------------------------------------------
// Pembantu baca berkas
// ----------------------------------------------------------------------

// Membaca satu baris; membuang '\r' agar berkas dari Windows (CRLF) aman.
static bool bacaBarisBerkas(ifstream& in, string& hasil) {
    if (!getline(in, hasil)) return false;
    if (!hasil.empty() && hasil[hasil.size() - 1] == '\r') {
        hasil.erase(hasil.size() - 1);
    }
    return true;
}

static bool bacaIntBerkas(ifstream& in, int& hasil) {
    string baris;
    return bacaBarisBerkas(in, baris) && parseInt(baris, hasil);
}

static bool bacaTanggalBerkas(ifstream& in, Tanggal& hasil) {
    string baris;
    return bacaBarisBerkas(in, baris) && parseTanggal(baris, hasil);
}

// Membuka berkas dan memeriksa penanda serta jumlah record.
//   return  1 : berkas terbuka dan kepala berkas benar
//   return  0 : berkas belum ada (bukan kesalahan; data dianggap kosong)
//   return -1 : berkas ada tetapi rusak
static int bukaBerkas(ifstream& in, const string& nama, const string& penanda,
                      int& jumlah) {
    in.open(nama.c_str());
    if (!in) return 0;
    string kepala;
    if (!bacaBarisBerkas(in, kepala) || kepala != penanda) return -1;
    if (!bacaIntBerkas(in, jumlah) || jumlah < 0) return -1;
    return 1;
}

static string tanggalKeTeks(const Tanggal& tgl) {
    return to_string(tgl.hari) + " " + to_string(tgl.bulan) + " " + to_string(tgl.tahun);
}

// ----------------------------------------------------------------------
// Simpan
// ----------------------------------------------------------------------

bool simpanData(const DaftarBuku& b, const DaftarAnggota& a,
                const DaftarTransaksi& t, const string& awalan) {
    ofstream fb((awalan + "buku.txt").c_str());
    ofstream fa((awalan + "anggota.txt").c_str());
    ofstream ft((awalan + "transaksi.txt").c_str());
    if (!fb || !fa || !ft) return false;

    fb << PENANDA_BUKU << '\n' << b.n << '\n';
    for (int i = 0; i < b.n; i++) {
        const Buku& x = b.data[i];
        fb << x.kode << '\n' << x.judul << '\n' << x.penulis << '\n'
           << x.kategori << '\n' << x.tahun << '\n' << x.stok << '\n'
           << x.totalDipinjam << '\n';
    }

    fa << PENANDA_ANGGOTA << '\n' << a.n << '\n';
    for (int i = 0; i < a.n; i++) {
        const Anggota& x = a.data[i];
        fa << x.id << '\n' << x.nama << '\n' << x.prodi << '\n'
           << x.sedangPinjam << '\n';
    }

    ft << PENANDA_TRANSAKSI << '\n' << t.n << '\n';
    for (int i = 0; i < t.n; i++) {
        const Transaksi& x = t.data[i];
        ft << x.nomor << '\n' << x.idAnggota << '\n' << x.kodeBuku << '\n'
           << tanggalKeTeks(x.tglPinjam) << '\n' << tanggalKeTeks(x.tglKembali) << '\n'
           << (x.selesai ? 1 : 0) << '\n' << x.denda << '\n' << x.kategori << '\n';
    }
    return fb.good() && fa.good() && ft.good();
}

// ----------------------------------------------------------------------
// Muat
// ----------------------------------------------------------------------

static bool muatBuku(DaftarBuku& b, const string& nama) {
    ifstream in;
    int jumlah = 0;
    int status = bukaBerkas(in, nama, PENANDA_BUKU, jumlah);
    if (status <= 0) return status == 0;
    bool bersih = true;
    for (int i = 0; i < jumlah; i++) {
        Buku x;
        if (!bacaBarisBerkas(in, x.kode) || !bacaBarisBerkas(in, x.judul) ||
            !bacaBarisBerkas(in, x.penulis) || !bacaIntBerkas(in, x.kategori) ||
            !bacaIntBerkas(in, x.tahun) || !bacaIntBerkas(in, x.stok) ||
            !bacaIntBerkas(in, x.totalDipinjam)) {
            return false;                       // berkas terpotong/rusak
        }
        // tambahBuku memvalidasi data dan MENOLAK kode ganda, sehingga
        // tidak mungkin terjadi duplikasi saat pemuatan.
        if (x.totalDipinjam >= 0 && tambahBuku(b, x)) {
            b.data[b.n - 1].totalDipinjam = x.totalDipinjam;  // tambahBuku mengisi 0
        } else {
            bersih = false;                     // record tidak valid dilewati
        }
    }
    return bersih;
}

static bool muatAnggota(DaftarAnggota& a, const string& nama) {
    ifstream in;
    int jumlah = 0;
    int status = bukaBerkas(in, nama, PENANDA_ANGGOTA, jumlah);
    if (status <= 0) return status == 0;
    bool bersih = true;
    for (int i = 0; i < jumlah; i++) {
        Anggota x;
        if (!bacaBarisBerkas(in, x.id) || !bacaBarisBerkas(in, x.nama) ||
            !bacaBarisBerkas(in, x.prodi) || !bacaIntBerkas(in, x.sedangPinjam)) {
            return false;
        }
        bool sahPinjam = x.sedangPinjam >= 0 && x.sedangPinjam <= MAKS_PINJAM;
        if (sahPinjam && tambahAnggota(a, x)) {
            a.data[a.n - 1].sedangPinjam = x.sedangPinjam;    // tambahAnggota mengisi 0
        } else {
            bersih = false;
        }
    }
    return bersih;
}

static bool transaksiSah(const DaftarTransaksi& t, const Transaksi& x) {
    if (x.nomor < 1 || cariTransaksi(t, x.nomor) != nullptr) return false;  // nomor unik
    if (!formatIdAnggota(x.idAnggota) || !formatKodeBuku(x.kodeBuku)) return false;
    if (!tanggalValid(x.tglPinjam)) return false;
    if (x.selesai && !tanggalValid(x.tglKembali)) return false;
    if (x.denda < 0) return false;
    return x.kategori >= -1 && x.kategori < JML_KATEGORI;
}

static bool muatTransaksi(DaftarTransaksi& t, const string& nama) {
    ifstream in;
    int jumlah = 0;
    int status = bukaBerkas(in, nama, PENANDA_TRANSAKSI, jumlah);
    if (status <= 0) return status == 0;
    bool bersih = true;
    for (int i = 0; i < jumlah; i++) {
        Transaksi x;
        int selesai = 0;
        if (!bacaIntBerkas(in, x.nomor) || !bacaBarisBerkas(in, x.idAnggota) ||
            !bacaBarisBerkas(in, x.kodeBuku) || !bacaTanggalBerkas(in, x.tglPinjam) ||
            !bacaTanggalBerkas(in, x.tglKembali) || !bacaIntBerkas(in, selesai) ||
            !bacaIntBerkas(in, x.denda) || !bacaIntBerkas(in, x.kategori)) {
            return false;
        }
        x.selesai = (selesai == 1);
        if (transaksiSah(t, x) && tambahTransaksi(t, x)) continue;
        bersih = false;
    }
    return bersih;
}

bool muatData(DaftarBuku& b, DaftarAnggota& a, DaftarTransaksi& t,
              const string& awalan) {
    // Daftar dikosongkan lebih dulu (kapasitas tetap), sehingga memuat dua
    // kali tidak menggandakan data.
    b.n = 0;
    a.n = 0;
    t.n = 0;
    bool okBuku = muatBuku(b, awalan + "buku.txt");
    bool okAnggota = muatAnggota(a, awalan + "anggota.txt");
    bool okTransaksi = muatTransaksi(t, awalan + "transaksi.txt");
    return okBuku && okAnggota && okTransaksi;
}
