// util.cpp - utilitas masukan, validasi format, dan tampilan
// Kelompok 7 - Kelas E (kerja bersama Mahasiswa A dan B)
//
// Strategi masukan yang konsisten: SEMUA masukan dibaca dengan getline()
// lalu diubah/divalidasi sendiri. Dengan begitu tidak ada sisa newline di
// buffer (masalah klasik cin >> lalu getline) dan masukan salah tidak
// membuat cin masuk ke keadaan gagal.
#include <iostream>
#include <cctype>
#include "sipus.h"

using namespace std;

string trim(const string& s) {
    size_t awal = 0;
    size_t akhir = s.size();
    while (awal < akhir && isspace(static_cast<unsigned char>(s[awal]))) awal++;
    while (akhir > awal && isspace(static_cast<unsigned char>(s[akhir - 1]))) akhir--;
    return s.substr(awal, akhir - awal);
}

string hurufBesar(const string& s) {
    string hasil = s;
    for (size_t i = 0; i < hasil.size(); i++) {
        hasil[i] = static_cast<char>(toupper(static_cast<unsigned char>(hasil[i])));
    }
    return hasil;
}

// Format: awalan 2 huruf + '-' + tepat 3 digit, mis. "BK-001".
static bool formatKode(const string& s, const string& awalan) {
    if (s.size() != 6) return false;
    if (s.compare(0, 3, awalan) != 0) return false;
    for (size_t i = 3; i < 6; i++) {
        if (!isdigit(static_cast<unsigned char>(s[i]))) return false;
    }
    return true;
}

bool formatKodeBuku(const string& kode) { return formatKode(kode, "BK-"); }
bool formatIdAnggota(const string& id) { return formatKode(id, "AG-"); }

// Bilangan bulat dengan tanda '-' opsional, 1..9 digit (aman dari overflow).
bool parseInt(const string& s, int& hasil) {
    string t = trim(s);
    size_t mulai = 0;
    bool negatif = false;
    if (!t.empty() && t[0] == '-') {
        negatif = true;
        mulai = 1;
    }
    size_t banyakDigit = t.size() - mulai;
    if (banyakDigit < 1 || banyakDigit > 9) return false;
    int nilai = 0;
    for (size_t i = mulai; i < t.size(); i++) {
        if (!isdigit(static_cast<unsigned char>(t[i]))) return false;
        nilai = nilai * 10 + (t[i] - '0');
    }
    hasil = negatif ? -nilai : nilai;
    return true;
}

HasilBaca bacaBaris(const string& prompt, string& hasil) {
    cout << prompt;
    if (!getline(cin, hasil)) return BACA_EOF;
    if (!hasil.empty() && hasil[hasil.size() - 1] == '\r') {
        hasil.erase(hasil.size() - 1);   // aman untuk berkas berakhiran CRLF
    }
    return BACA_OK;
}

// Membaca teks (sudah di-trim). Jika tidak boleh kosong, diulang sampai terisi.
HasilBaca bacaTeks(const string& prompt, string& hasil, bool bolehKosong) {
    string baris;
    while (true) {
        if (bacaBaris(prompt, baris) == BACA_EOF) return BACA_EOF;
        baris = trim(baris);
        if (!baris.empty()) {
            hasil = baris;
            return BACA_OK;
        }
        if (bolehKosong) return BACA_KOSONG;
        cout << ">> Masukan tidak boleh kosong.\n";
    }
}

// Membaca bilangan bulat dalam rentang [minimum, maksimum]. Diulang sampai valid.
HasilBaca bacaInt(const string& prompt, int minimum, int maksimum,
                  int& hasil, bool bolehKosong) {
    string baris;
    while (true) {
        if (bacaBaris(prompt, baris) == BACA_EOF) return BACA_EOF;
        baris = trim(baris);
        if (baris.empty() && bolehKosong) return BACA_KOSONG;
        int nilai = 0;
        if (parseInt(baris, nilai) && nilai >= minimum && nilai <= maksimum) {
            hasil = nilai;
            return BACA_OK;
        }
        cout << ">> Masukan tidak valid. Masukkan bilangan bulat "
             << minimum << " sampai " << maksimum << ".\n";
    }
}

// Nama kategori 0..4. ASUMSI: nama diambil dari contoh laporan pada PDF dosen.
string labelKategori(int kategori) {
    static const char* const NAMA[JML_KATEGORI] = {
        "Pemrograman", "Basis Data", "Jaringan", "Matematika", "Umum"
    };
    if (kategori < 0 || kategori >= JML_KATEGORI) return "?";
    return NAMA[kategori];
}

// Memotong teks agar muat di kolom tabel; tambahkan "..." jika terpotong.
string potong(const string& s, int lebar) {
    if (lebar < 4) return s.substr(0, static_cast<size_t>(lebar < 0 ? 0 : lebar));
    if (static_cast<int>(s.size()) <= lebar) return s;
    return s.substr(0, static_cast<size_t>(lebar - 3)) + "...";
}

string garis(int panjang) {
    return string(static_cast<size_t>(panjang < 0 ? 0 : panjang), '-');
}

string hurufKecil(const string& s) {
    string hasil = s;
    for (size_t i = 0; i < hasil.size(); i++) {
        hasil[i] = static_cast<char>(tolower(static_cast<unsigned char>(hasil[i])));
    }
    return hasil;
}

// ----------------------------------------------------------------------
// Utilitas tanggal (tambahan Milestone 3; dipakai F3, F4, F7)
// ----------------------------------------------------------------------

// Tahun kabisat: habis dibagi 4, kecuali tahun abad yang tidak habis dibagi 400.
bool tahunKabisat(int tahun) {
    return (tahun % 4 == 0 && tahun % 100 != 0) || (tahun % 400 == 0);
}

int jumlahHariBulan(int bulan, int tahun) {
    static const int HARI[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (bulan < 1 || bulan > 12) return 0;
    if (bulan == 2 && tahunKabisat(tahun)) return 29;
    return HARI[bulan - 1];
}

// Tanggal valid bila kalendernya benar (mis. 30/02 ditolak, 29/02 hanya
// pada tahun kabisat) dan tahunnya dalam rentang transaksi.
bool tanggalValid(const Tanggal& tgl) {
    if (tgl.tahun < TAHUN_TRX_MIN || tgl.tahun > TAHUN_TRX_MAKS) return false;
    if (tgl.bulan < 1 || tgl.bulan > 12) return false;
    return tgl.hari >= 1 && tgl.hari <= jumlahHariBulan(tgl.bulan, tgl.tahun);
}

// Menerima "3 11 2026", "3/11/2026", atau "3-11-2026". Hanya mengurai
// angka; kebenaran kalender diperiksa oleh tanggalValid().
bool parseTanggal(const string& s, Tanggal& hasil) {
    string teks = s;
    for (size_t i = 0; i < teks.size(); i++) {
        if (teks[i] == '/' || teks[i] == '-') teks[i] = ' ';
    }
    int angka[3] = {0, 0, 0};
    int banyak = 0;
    size_t i = 0;
    while (i < teks.size()) {
        while (i < teks.size() && teks[i] == ' ') i++;      // lewati spasi
        if (i >= teks.size()) break;
        size_t awal = i;
        while (i < teks.size() && teks[i] != ' ') i++;      // ambil satu kata
        if (banyak >= 3) return false;                      // lebih dari 3 bagian
        if (!parseInt(teks.substr(awal, i - awal), angka[banyak])) return false;
        banyak++;
    }
    if (banyak != 3) return false;
    hasil.hari = angka[0];
    hasil.bulan = angka[1];
    hasil.tahun = angka[2];
    return true;
}

static string duaDigit(int x) {
    return (x < 10 ? "0" : "") + to_string(x);
}

string formatTanggal(const Tanggal& tgl) {
    return duaDigit(tgl.hari) + "/" + duaDigit(tgl.bulan) + "/" + to_string(tgl.tahun);
}

// Membaca tanggal sampai valid. Jika bolehKosong, baris kosong berarti batal.
HasilBaca bacaTanggal(const string& prompt, Tanggal& hasil, bool bolehKosong) {
    string baris;
    while (true) {
        if (bacaBaris(prompt, baris) == BACA_EOF) return BACA_EOF;
        baris = trim(baris);
        if (baris.empty() && bolehKosong) return BACA_KOSONG;
        Tanggal tgl = {0, 0, 0};
        if (parseTanggal(baris, tgl) && tanggalValid(tgl)) {
            hasil = tgl;
            return BACA_OK;
        }
        cout << ">> Tanggal tidak valid. Gunakan format hari bulan tahun, "
             << "contoh 3 11 2026 (tahun " << TAHUN_TRX_MIN << "-"
             << TAHUN_TRX_MAKS << ").\n";
    }
}
