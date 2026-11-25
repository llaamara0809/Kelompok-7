// uji.cpp - pengujian otomatis SiPus (F1-F8 dan penyimpanan berkas)
// Kelompok 7 - Kelas E
// Pola fungsi cek() sesuai contoh PDF dosen. Dikompilasi TERPISAH dari main.cpp
// (hanya satu main() per executable).
//   Milestone 1-2 : pengujian F1, F2, array dinamis, utilitas.
//   Milestone 3-4 : pengujian F3-F8, tanggal, dan penyimpanan berkas.
#include <iostream>
#include <fstream>
#include <cstdio>
#include "sipus.h"

using namespace std;

int lulus = 0, gagal = 0;

void cek(bool kondisi, const string& nama) {
    cout << (kondisi ? "[LULUS] " : "[GAGAL] ") << nama << endl;
    kondisi ? lulus++ : gagal++;
}

// Membuat buku contoh yang valid.
static Buku buatBuku(const string& kode, int kategori = 0, int stok = 3) {
    Buku b = {kode, "Judul " + kode, "Penulis", kategori, 2024, stok, 0};
    return b;
}

static Anggota buatAnggota(const string& id) {
    Anggota a = {id, "Nama " + id, "Teknologi Informasi", 0};
    return a;
}

static void ujiBukuTambahDanDuplikat() {
    cout << "\n-- F1: tambah buku --\n";
    DaftarBuku d;
    inisialisasi(d, 2);
    cek(d.n == 0 && d.kapasitas == 2 && d.data != nullptr, "inisialisasi buku kosong, kapasitas 2");

    cek(tambahBuku(d, {"BK-002", "Struktur Data", "Wati", 0, 2024, 3, 0}), "tambah buku berhasil");
    cek(d.n == 1 && d.data[0].kode == "BK-002", "data buku tersimpan");
    cek(d.data[0].totalDipinjam == 0, "totalDipinjam diinisialisasi 0");

    // Meskipun pemanggil mengisi totalDipinjam != 0, buku baru tetap mulai dari 0.
    Buku b = {"BK-001", "Algoritma", "Budi", 0, 2023, 1, 9};
    cek(tambahBuku(d, b) && d.data[1].totalDipinjam == 0, "buku baru selalu mulai totalDipinjam 0");

    Buku ganda = {"BK-001", "X", "Y", 0, 2020, 1, 0};
    cek(!tambahBuku(d, ganda), "kode buku ganda ditolak");
    cek(d.n == 2, "jumlah buku tetap setelah duplikat ditolak");

    cek(tambahBuku(d, {"BK-003", "Basis Data", "Sari", 1, 2022, 2, 0}), "tambah buku ke-3 (memicu pembesaran)");
    cek(d.n == 3 && d.kapasitas >= 3, "array buku membesar");
    cek(d.data[0].kode == "BK-002" && d.data[1].kode == "BK-001" && d.data[2].kode == "BK-003",
        "isi lama utuh setelah pembesaran");
    bebaskan(d);
}

static void ujiBukuPembesaranBerulang() {
    cout << "\n-- F1: pembesaran berulang --\n";
    DaftarBuku d;
    inisialisasi(d, 1);
    bool semuaBerhasil = true;
    for (int i = 1; i <= 20; i++) {
        string kode = (i < 10 ? "BK-00" : "BK-0") + to_string(i);
        if (!tambahBuku(d, buatBuku(kode))) semuaBerhasil = false;
    }
    cek(semuaBerhasil && d.n == 20, "20 buku ditambahkan dari kapasitas awal 1");
    cek(d.kapasitas >= 20, "kapasitas >= jumlah data");
    cek(d.data[0].kode == "BK-001" && d.data[19].kode == "BK-020", "urutan data terjaga setelah beberapa kali pembesaran");
    bebaskan(d);
}

static void ujiBukuValidasi() {
    cout << "\n-- F1: validasi masukan --\n";
    DaftarBuku d;
    inisialisasi(d, 4);
    cek(!tambahBuku(d, {"BK-1", "J", "P", 0, 2024, 1, 0}), "kode terlalu pendek ditolak");
    cek(!tambahBuku(d, {"bk-001", "J", "P", 0, 2024, 1, 0}), "kode huruf kecil ditolak");
    cek(!tambahBuku(d, {"BK-0012", "J", "P", 0, 2024, 1, 0}), "kode terlalu panjang ditolak");
    cek(!tambahBuku(d, {"XX-001", "J", "P", 0, 2024, 1, 0}), "awalan kode salah ditolak");
    cek(!tambahBuku(d, {"BK-0A1", "J", "P", 0, 2024, 1, 0}), "kode non-digit ditolak");
    cek(!tambahBuku(d, {"BK-001", "", "P", 0, 2024, 1, 0}), "judul kosong ditolak");
    cek(!tambahBuku(d, {"BK-001", "   ", "P", 0, 2024, 1, 0}), "judul spasi saja ditolak");
    cek(!tambahBuku(d, {"BK-001", "J", "", 0, 2024, 1, 0}), "penulis kosong ditolak");
    cek(!tambahBuku(d, {"BK-001", "J", "P", -1, 2024, 1, 0}), "kategori -1 ditolak");
    cek(!tambahBuku(d, {"BK-001", "J", "P", JML_KATEGORI, 2024, 1, 0}), "kategori 5 ditolak");
    cek(!tambahBuku(d, {"BK-001", "J", "P", 0, 2024, -1, 0}), "stok negatif ditolak");
    cek(!tambahBuku(d, {"BK-001", "J", "P", 0, 0, 1, 0}), "tahun 0 ditolak");
    cek(!tambahBuku(d, {"BK-001", "J", "P", 0, TAHUN_MAKS + 1, 1, 0}), "tahun melebihi batas ditolak");
    cek(d.n == 0, "tidak ada data masuk dari masukan tidak valid");
    cek(tambahBuku(d, {"BK-001", "J", "P", 0, 2024, 0, 0}), "stok 0 diperbolehkan");
    cek(tambahBuku(d, {"BK-002", "J", "P", JML_KATEGORI - 1, TAHUN_MIN, 1, 0}), "nilai batas kategori dan tahun diterima");
    bebaskan(d);
}

static void ujiBukuCariUbahHapus() {
    cout << "\n-- F1: cari, ubah, hapus --\n";
    DaftarBuku d;
    inisialisasi(d, 2);
    tambahBuku(d, buatBuku("BK-001"));
    tambahBuku(d, buatBuku("BK-002"));
    tambahBuku(d, buatBuku("BK-003"));

    Buku* p = cariKode(d, "BK-002");
    cek(p != nullptr && p->kode == "BK-002", "cariKode menemukan buku");
    cek(cariKode(d, "BK-999") == nullptr, "cariKode kode tidak ada mengembalikan nullptr");

    p->totalDipinjam = 7;   // simulasi statistik dari F3 (belum ada)
    cek(ubahBuku(d, "BK-002", "Judul Baru", "Penulis Baru", 3, 2020, 9), "ubah buku berhasil");
    cek(d.data[1].judul == "Judul Baru" && d.data[1].penulis == "Penulis Baru" &&
        d.data[1].kategori == 3 && d.data[1].tahun == 2020 && d.data[1].stok == 9,
        "semua field yang diubah tersimpan");
    cek(d.data[1].kode == "BK-002", "kode tetap setelah diubah");
    cek(d.data[1].totalDipinjam == 7, "totalDipinjam tidak berubah saat ubah buku");
    cek(!ubahBuku(d, "BK-999", "J", "P", 0, 2024, 1), "ubah buku tidak ada ditolak");
    cek(!ubahBuku(d, "BK-002", "", "P", 0, 2024, 1), "ubah dengan judul kosong ditolak");
    cek(!ubahBuku(d, "BK-002", "J", "P", 9, 2024, 1), "ubah dengan kategori tidak valid ditolak");
    cek(!ubahBuku(d, "BK-002", "J", "P", 0, 2024, -5), "ubah dengan stok negatif ditolak");
    cek(d.data[1].judul == "Judul Baru" && d.data[1].stok == 9, "data tidak rusak setelah ubah ditolak");

    // Hapus tanpa transaksi
    DaftarTransaksi t;
    inisialisasi(t, 2);
    cek(hapusBuku(d, t, "BK-001") == HAPUS_OK, "hapus buku (tidak dipinjam) berhasil");
    cek(d.n == 2 && d.data[0].kode == "BK-002" && d.data[1].kode == "BK-003",
        "sisa buku bergeser dengan urutan benar");
    cek(cariKode(d, "BK-001") == nullptr, "buku terhapus tidak ditemukan lagi");
    cek(hapusBuku(d, t, "BK-001") == HAPUS_TIDAK_ADA, "hapus buku tidak ada dilaporkan");

    // Hapus dengan transaksi aktif
    Transaksi aktif = {1, "AG-001", "BK-002", {3, 11, 2026}, {0, 0, 0}, false, 0, 0};
    cek(tambahTransaksi(t, aktif), "tambahTransaksi (fondasi array) berhasil");
    cek(bukuMasihDipinjam(t, "BK-002"), "bukuMasihDipinjam true untuk transaksi aktif");
    cek(!bukuMasihDipinjam(t, "BK-003"), "bukuMasihDipinjam false untuk buku lain");
    cek(hapusBuku(d, t, "BK-002") == HAPUS_MASIH_DIPINJAM, "hapus buku yang masih dipinjam ditolak");
    cek(d.n == 2 && cariKode(d, "BK-002") != nullptr, "buku yang dipinjam tetap ada");

    t.data[0].selesai = true;   // simulasi pengembalian (F4 belum ada)
    cek(!bukuMasihDipinjam(t, "BK-002"), "bukuMasihDipinjam false setelah transaksi selesai");
    cek(hapusBuku(d, t, "BK-002") == HAPUS_OK, "hapus buku berhasil setelah dikembalikan");

    bebaskan(t);
    bebaskan(d);
}

static void ujiAnggota() {
    cout << "\n-- F2: anggota --\n";
    DaftarAnggota d;
    inisialisasi(d, 2);
    cek(d.n == 0 && d.kapasitas == 2 && d.data != nullptr, "inisialisasi anggota kosong, kapasitas 2");

    cek(tambahAnggota(d, {"AG-001", "Made", "Teknologi Informasi", 0}), "tambah anggota berhasil");
    cek(d.n == 1 && d.data[0].id == "AG-001" && d.data[0].nama == "Made", "data anggota tersimpan");
    cek(tambahAnggota(d, {"AG-002", "Ayu", "Sistem Informasi", 2}) && d.data[1].sedangPinjam == 0,
        "sedangPinjam anggota baru selalu 0");
    cek(!tambahAnggota(d, {"AG-001", "Lain", "Prodi Lain", 0}), "ID anggota ganda ditolak");
    cek(d.n == 2, "jumlah anggota tetap setelah duplikat ditolak");

    cek(!tambahAnggota(d, {"AG-1", "N", "P", 0}), "ID terlalu pendek ditolak");
    cek(!tambahAnggota(d, {"ag-003", "N", "P", 0}), "ID huruf kecil ditolak");
    cek(!tambahAnggota(d, {"BK-003", "N", "P", 0}), "awalan ID salah ditolak");
    cek(!tambahAnggota(d, {"AG-003", "", "P", 0}), "nama kosong ditolak");
    cek(!tambahAnggota(d, {"AG-003", "N", "  ", 0}), "prodi kosong ditolak");
    cek(d.n == 2, "tidak ada data masuk dari masukan tidak valid");

    cek(tambahAnggota(d, {"AG-003", "Komang", "Teknologi Informasi", 0}), "tambah anggota ke-3 (memicu pembesaran)");
    cek(d.n == 3 && d.kapasitas >= 3, "array anggota membesar");
    cek(d.data[0].id == "AG-001" && d.data[2].id == "AG-003", "isi lama utuh setelah pembesaran");

    for (int i = 4; i <= 15; i++) {
        string id = (i < 10 ? "AG-00" : "AG-0") + to_string(i);
        tambahAnggota(d, buatAnggota(id));
    }
    cek(d.n == 15 && d.kapasitas >= 15, "anggota bertambah hingga 15 (pembesaran berulang)");

    Anggota* p = cariAnggota(d, "AG-002");
    cek(p != nullptr && p->nama == "Ayu", "cariAnggota menemukan anggota");
    cek(cariAnggota(d, "AG-999") == nullptr, "cariAnggota ID tidak ada mengembalikan nullptr");

    p->sedangPinjam = 2;   // simulasi dari F3 (belum ada)
    cek(ubahAnggota(d, "AG-002", "Ayu Baru", "Prodi Baru"), "ubah anggota berhasil");
    cek(p->nama == "Ayu Baru" && p->prodi == "Prodi Baru", "nama dan prodi berubah");
    cek(p->id == "AG-002", "ID tetap setelah diubah");
    cek(p->sedangPinjam == 2, "sedangPinjam tidak berubah saat ubah anggota");
    cek(!ubahAnggota(d, "AG-999", "N", "P"), "ubah anggota tidak ada ditolak");
    cek(!ubahAnggota(d, "AG-002", "", "P"), "ubah dengan nama kosong ditolak");
    cek(!ubahAnggota(d, "AG-002", "N", ""), "ubah dengan prodi kosong ditolak");
    cek(p->nama == "Ayu Baru" && p->prodi == "Prodi Baru", "data tidak rusak setelah ubah ditolak");

    bebaskan(d);
    cek(d.data == nullptr && d.n == 0 && d.kapasitas == 0, "memori anggota dibebaskan");
}

static void ujiTransaksiDanMemori() {
    cout << "\n-- Array dinamis transaksi dan pembebasan memori --\n";
    DaftarTransaksi t;
    inisialisasi(t, 2);
    bool ok = true;
    for (int i = 1; i <= 9; i++) {
        Transaksi tr = {i, "AG-001", "BK-001", {1, 1, 2026}, {0, 0, 0}, false, 0, 0};
        if (!tambahTransaksi(t, tr)) ok = false;
    }
    cek(ok && t.n == 9 && t.kapasitas >= 9, "array transaksi membesar");
    cek(t.data[0].nomor == 1 && t.data[8].nomor == 9, "isi transaksi utuh setelah pembesaran");
    bebaskan(t);
    cek(t.data == nullptr && t.n == 0 && t.kapasitas == 0, "memori transaksi dibebaskan");

    DaftarBuku d;
    inisialisasi(d, 4);
    tambahBuku(d, buatBuku("BK-001"));
    bebaskan(d);
    cek(d.data == nullptr, "memori dibebaskan");
    cek(d.n == 0 && d.kapasitas == 0, "n dan kapasitas direset setelah dibebaskan");
    bebaskan(d);
    cek(d.data == nullptr, "bebaskan dua kali aman");
    cek(tambahBuku(d, buatBuku("BK-005")) && d.n == 1, "daftar dapat dipakai lagi setelah dibebaskan");
    bebaskan(d);
}

static void ujiValidatorFormat() {
    cout << "\n-- Utilitas validasi format --\n";
    cek(formatKodeBuku("BK-001") && !formatKodeBuku("BK-01") && !formatKodeBuku(""), "formatKodeBuku");
    cek(formatIdAnggota("AG-123") && !formatIdAnggota("AG-12a") && !formatIdAnggota("AG 123"), "formatIdAnggota");
    int x = 0;
    cek(parseInt("42", x) && x == 42, "parseInt bilangan positif");
    cek(parseInt(" -7 ", x) && x == -7, "parseInt bilangan negatif dengan spasi");
    cek(!parseInt("", x) && !parseInt("abc", x) && !parseInt("12a", x) && !parseInt("1234567890", x),
        "parseInt menolak masukan salah");
    cek(trim("  halo  ") == "halo" && hurufBesar("bk-1") == "BK-1", "trim dan hurufBesar");
}

// ======================================================================
// Milestone 3-4: F3-F8 dan fitur tambahan
// ======================================================================

static Tanggal tg(int hari, int bulan, int tahun) {
    Tanggal t = {hari, bulan, tahun};
    return t;
}

static void ujiTanggal() {
    cout << "\n-- F4: perhitungan tanggal --\n";
    cek(selisihHari(tg(3, 11, 2026), tg(10, 11, 2026)) == 7, "selisihHari dalam satu bulan");
    cek(selisihHari(tg(28, 1, 2026), tg(4, 2, 2026)) == 7, "selisihHari melewati bulan");
    cek(selisihHari(tg(28, 12, 2026), tg(4, 1, 2027)) == 7, "selisihHari melewati tahun");
    cek(selisihHari(tg(28, 2, 2028), tg(1, 3, 2028)) == 2, "selisihHari Februari tahun kabisat (2028)");
    cek(selisihHari(tg(28, 2, 2026), tg(1, 3, 2026)) == 1, "selisihHari Februari tahun biasa (2026)");
    cek(selisihHari(tg(1, 1, 2024), tg(1, 1, 2025)) == 366, "satu tahun kabisat = 366 hari");
    cek(selisihHari(tg(10, 11, 2026), tg(3, 11, 2026)) == -7, "selisihHari negatif bila terbalik");
    cek(tahunKabisat(2000) && !tahunKabisat(2100) && tahunKabisat(2028) && !tahunKabisat(2026),
        "aturan tahun kabisat (2000 ya, 2100 tidak)");
    cek(tanggalValid(tg(29, 2, 2028)) && !tanggalValid(tg(29, 2, 2026)), "29 Februari hanya pada tahun kabisat");
    cek(!tanggalValid(tg(31, 4, 2026)) && !tanggalValid(tg(1, 13, 2026)) && !tanggalValid(tg(0, 1, 2026)),
        "tanggal kalender tidak valid ditolak");
    Tanggal hasil = {0, 0, 0};
    tambahHari(tg(28, 12, 2026), 7, hasil);
    cek(hasil.hari == 4 && hasil.bulan == 1 && hasil.tahun == 2027, "batas kembali 7 hari melewati tahun");
    int lama = -1, telat = -1, denda = -1;
    hitungDenda(tg(3, 11, 2026), tg(20, 11, 2026), lama, telat, denda);
    cek(lama == 17 && telat == 10 && denda == 10000, "hitungDenda (parameter keluaran) terlambat 10 hari");
    hitungDenda(tg(3, 11, 2026), tg(10, 11, 2026), lama, telat, denda);
    cek(lama == 7 && telat == 0 && denda == 0, "hitungDenda tepat 7 hari = Rp0");
    Tanggal p = {0, 0, 0};
    cek(parseTanggal("3 11 2026", p) && p.hari == 3 && p.bulan == 11 && p.tahun == 2026, "parseTanggal spasi");
    cek(parseTanggal("3/11/2026", p) && parseTanggal(" 03-11-2026 ", p) && p.bulan == 11, "parseTanggal / dan -");
    cek(!parseTanggal("3 11", p) && !parseTanggal("a b c", p) && !parseTanggal("3 11 2026 5", p),
        "parseTanggal menolak masukan salah");
    cek(formatTanggal(tg(3, 1, 2027)) == "03/01/2027", "formatTanggal dd/mm/yyyy");
}

// Data awal untuk pengujian transaksi.
static void siapkanData(DaftarBuku& b, DaftarAnggota& a, DaftarTransaksi& t) {
    inisialisasi(b, 2);
    inisialisasi(a, 2);
    inisialisasi(t, 2);
    tambahBuku(b, {"BK-001", "Pemrograman C++", "Budi", 0, 2024, 2, 0});
    tambahBuku(b, {"BK-002", "Basis Data", "Sari", 1, 2023, 0, 0});     // stok 0
    tambahBuku(b, {"BK-003", "Jaringan Komputer", "Rina", 2, 2022, 5, 0});
    tambahBuku(b, {"BK-004", "Kalkulus", "Agus", 3, 2021, 5, 0});
    tambahAnggota(a, {"AG-001", "Made Ayu", "Teknologi Informasi", 0});
    tambahAnggota(a, {"AG-002", "Komang", "Teknik Elektro", 0});
}

static void ujiPeminjaman() {
    cout << "\n-- F3: peminjaman --\n";
    DaftarBuku b; DaftarAnggota a; DaftarTransaksi t;
    siapkanData(b, a, t);

    cek(pinjamBuku(t, b, a, "AG-001", "BK-001", tg(3, 11, 2026)), "peminjaman berhasil");
    cek(t.n == 1 && t.data[0].nomor == 1 && !t.data[0].selesai && t.data[0].denda == 0,
        "transaksi baru tersimpan (nomor 1, aktif, denda 0)");
    cek(t.data[0].idAnggota == "AG-001" && t.data[0].kodeBuku == "BK-001" &&
        t.data[0].tglPinjam.hari == 3 && t.data[0].kategori == 0, "isi transaksi benar");
    cek(cariKode(b, "BK-001")->stok == 1, "stok buku berkurang 1");
    cek(cariKode(b, "BK-001")->totalDipinjam == 1, "totalDipinjam buku bertambah 1");
    cek(cariAnggota(a, "AG-001")->sedangPinjam == 1, "sedangPinjam anggota bertambah 1");

    cek(periksaPinjam(b, a, "AG-001", "BK-002", tg(3, 11, 2026)) == PINJAM_STOK_HABIS &&
        !pinjamBuku(t, b, a, "AG-001", "BK-002", tg(3, 11, 2026)), "peminjaman stok kosong ditolak");
    cek(periksaPinjam(b, a, "AG-099", "BK-001", tg(3, 11, 2026)) == PINJAM_ANGGOTA_TIDAK_ADA &&
        !pinjamBuku(t, b, a, "AG-099", "BK-001", tg(3, 11, 2026)), "peminjaman anggota tidak terdaftar ditolak");
    cek(periksaPinjam(b, a, "AG-001", "BK-999", tg(3, 11, 2026)) == PINJAM_BUKU_TIDAK_ADA &&
        !pinjamBuku(t, b, a, "AG-001", "BK-999", tg(3, 11, 2026)), "peminjaman buku tidak terdaftar ditolak");
    cek(periksaPinjam(b, a, "AG-001", "BK-001", tg(30, 2, 2026)) == PINJAM_TANGGAL_TIDAK_VALID &&
        !pinjamBuku(t, b, a, "AG-001", "BK-001", tg(30, 2, 2026)), "peminjaman tanggal tidak valid ditolak");
    cek(t.n == 1 && cariKode(b, "BK-001")->stok == 1 && cariAnggota(a, "AG-001")->sedangPinjam == 1 &&
        cariKode(b, "BK-002")->totalDipinjam == 0, "peminjaman gagal tidak mengubah data");

    cek(pinjamBuku(t, b, a, "AG-001", "BK-003", tg(4, 11, 2026)) &&
        pinjamBuku(t, b, a, "AG-001", "BK-004", tg(5, 11, 2026)), "peminjaman buku ke-2 dan ke-3 berhasil");
    cek(t.n == 3 && t.kapasitas >= 3, "array transaksi membesar (kapasitas awal 2)");
    cek(cariAnggota(a, "AG-001")->sedangPinjam == MAKS_PINJAM, "anggota mencapai batas 3 buku");
    cek(periksaPinjam(b, a, "AG-001", "BK-003", tg(6, 11, 2026)) == PINJAM_BATAS_TERCAPAI &&
        !pinjamBuku(t, b, a, "AG-001", "BK-003", tg(6, 11, 2026)), "peminjaman buku keempat ditolak");
    cek(t.n == 3 && cariKode(b, "BK-003")->stok == 4, "penolakan buku keempat tidak mengubah stok/transaksi");
    cek(t.data[0].nomor == 1 && t.data[1].nomor == 2 && t.data[2].nomor == 3 && nomorBerikutnya(t) == 4,
        "nomor transaksi unik dan berurutan");
    cek(pinjamBuku(t, b, a, "AG-002", "BK-001", tg(6, 11, 2026)) && cariKode(b, "BK-001")->stok == 0,
        "anggota lain meminjam sisa stok terakhir");
    cek(!pinjamBuku(t, b, a, "AG-002", "BK-001", tg(6, 11, 2026)), "stok habis setelah dipinjam ditolak");

    cek(hapusBuku(b, t, "BK-003") == HAPUS_MASIH_DIPINJAM && cariKode(b, "BK-003") != nullptr,
        "penghapusan buku yang sedang dipinjam (via F3) ditolak");
    bebaskan(t); bebaskan(a); bebaskan(b);
}

static void ujiPengembalian() {
    cout << "\n-- F4: pengembalian --\n";
    DaftarBuku b; DaftarAnggota a; DaftarTransaksi t;
    siapkanData(b, a, t);
    pinjamBuku(t, b, a, "AG-001", "BK-001", tg(3, 11, 2026));   // nomor 1
    pinjamBuku(t, b, a, "AG-001", "BK-003", tg(3, 11, 2026));   // nomor 2
    pinjamBuku(t, b, a, "AG-002", "BK-004", tg(28, 12, 2026));  // nomor 3

    cek(kembalikanBuku(t, b, a, 1, tg(10, 11, 2026)), "pengembalian tepat waktu berhasil");
    const Transaksi* tr1 = cariTransaksi(t, 1);
    cek(tr1->selesai && tr1->denda == 0 && tr1->tglKembali.hari == 10, "tepat waktu (7 hari): denda Rp0, status selesai");
    cek(cariKode(b, "BK-001")->stok == 2, "stok buku kembali bertambah");
    cek(cariAnggota(a, "AG-001")->sedangPinjam == 1, "sedangPinjam anggota berkurang");

    cek(kembalikanBuku(t, b, a, 2, tg(13, 11, 2026)) && cariTransaksi(t, 2)->denda == 3000,
        "pengembalian terlambat 3 hari: denda Rp3.000");
    cek(kembalikanBuku(t, b, a, 3, tg(14, 1, 2027)) && cariTransaksi(t, 3)->denda == 10000,
        "terlambat melewati tahun (28/12 -> 14/01): denda Rp10.000");

    cek(periksaKembali(t, 1, tg(11, 11, 2026)) == KEMBALI_SUDAH_SELESAI &&
        !kembalikanBuku(t, b, a, 1, tg(11, 11, 2026)), "pengembalian ganda ditolak");
    cek(cariKode(b, "BK-001")->stok == 2 && cariTransaksi(t, 1)->tglKembali.hari == 10,
        "pengembalian ganda tidak mengubah stok/data");
    cek(periksaKembali(t, 99, tg(11, 11, 2026)) == KEMBALI_TIDAK_ADA &&
        !kembalikanBuku(t, b, a, 99, tg(11, 11, 2026)), "nomor transaksi tidak ada ditolak");

    pinjamBuku(t, b, a, "AG-002", "BK-003", tg(10, 11, 2026));  // nomor 4
    cek(periksaKembali(t, 4, tg(9, 11, 2026)) == KEMBALI_SEBELUM_PINJAM &&
        !kembalikanBuku(t, b, a, 4, tg(9, 11, 2026)), "tanggal kembali sebelum tanggal pinjam ditolak");
    cek(periksaKembali(t, 4, tg(31, 11, 2026)) == KEMBALI_TANGGAL_TIDAK_VALID, "tanggal kembali tidak valid ditolak");
    cek(hapusBuku(b, t, "BK-003") == HAPUS_MASIH_DIPINJAM, "buku masih dipinjam tidak dapat dihapus");
    cek(kembalikanBuku(t, b, a, 4, tg(10, 11, 2026)) && hapusBuku(b, t, "BK-003") == HAPUS_OK,
        "buku dapat dihapus setelah dikembalikan");
    bebaskan(t); bebaskan(a); bebaskan(b);
}

static void ujiPencarian() {
    cout << "\n-- F5: pencarian --\n";
    DaftarBuku d;
    inisialisasi(d, 2);
    tambahBuku(d, {"BK-005", "Struktur Data", "Wati", 0, 2024, 1, 0});
    tambahBuku(d, {"BK-002", "Algoritma Pemrograman", "Budi", 0, 2023, 1, 0});
    tambahBuku(d, {"BK-009", "BASIS DATA Lanjut", "Sari", 1, 2022, 1, 0});
    tambahBuku(d, {"BK-001", "Jaringan Komputer", "Rina", 2, 2021, 1, 0});
    tambahBuku(d, {"BK-007", "Kalkulus", "Agus", 3, 2020, 1, 0});

    cek(hitungCocokJudul(d, "data") == 2, "cari judul 'data' menemukan 2 buku");
    cek(hitungCocokJudul(d, "DATA") == 2 && hitungCocokJudul(d, "DaTa") == 2,
        "pencarian judul tidak membedakan huruf besar/kecil");
    cek(hitungCocokJudul(d, "rogram") == 1 && judulCocok("Algoritma Pemrograman", "GRAM"),
        "pencarian potongan judul");
    cek(hitungCocokJudul(d, "xyz") == 0 && hitungCocokJudul(d, "   ") == 0,
        "kata tidak ada / kosong: 0 hasil");

    cek(!terurutKode(d), "data awal belum terurut berdasarkan kode");
    urutkanKode(d);
    cek(terurutKode(d), "data terurut sebelum binary search");
    int pos = binarySearchKode(d.data, 0, d.n - 1, "BK-007");
    cek(pos == 3 && d.data[pos].kode == "BK-007" && d.data[pos].judul == "Kalkulus",
        "binary search kode yang tersedia (BK-007)");
    bool semuaKetemu = true;
    for (int i = 0; i < d.n; i++) {
        if (binarySearchKode(d.data, 0, d.n - 1, d.data[i].kode) != i) semuaKetemu = false;
    }
    cek(semuaKetemu, "binary search menemukan setiap kode pada indeks yang benar");
    cek(binarySearchKode(d.data, 0, d.n - 1, "BK-003") == -1, "binary search kode tidak tersedia (BK-003)");
    cek(binarySearchKode(d.data, 0, d.n - 1, "BK-000") == -1 &&
        binarySearchKode(d.data, 0, d.n - 1, "BK-999") == -1, "binary search kode di luar rentang");
    cek(binarySearchKode(d.data, 0, -1, "BK-001") == -1, "binary search pada array kosong");
    bebaskan(d);
}

static void ujiPengurutan() {
    cout << "\n-- F6: pengurutan --\n";
    DaftarBuku b; DaftarAnggota a; DaftarTransaksi t;
    inisialisasi(b, 2); inisialisasi(a, 2); inisialisasi(t, 2);
    tambahBuku(b, {"BK-003", "struktur data", "Wati", 0, 2024, 9, 0});
    tambahBuku(b, {"BK-001", "Basis Data", "Sari", 1, 2023, 9, 0});
    tambahBuku(b, {"BK-004", "Algoritma", "Budi", 0, 2022, 9, 0});
    tambahBuku(b, {"BK-002", "Jaringan", "Rina", 2, 2021, 9, 0});
    tambahAnggota(a, {"AG-001", "Made", "TI", 0});
    tambahAnggota(a, {"AG-002", "Ayu", "TI", 0});
    // totalDipinjam: BK-002 = 2, BK-001 = 1, BK-004 = 2, BK-003 = 0
    pinjamBuku(t, b, a, "AG-001", "BK-002", tg(1, 3, 2026));
    pinjamBuku(t, b, a, "AG-001", "BK-001", tg(1, 3, 2026));
    pinjamBuku(t, b, a, "AG-001", "BK-004", tg(1, 3, 2026));
    pinjamBuku(t, b, a, "AG-002", "BK-002", tg(2, 3, 2026));
    pinjamBuku(t, b, a, "AG-002", "BK-004", tg(2, 3, 2026));

    urutkanKode(b);
    cek(b.data[0].kode == "BK-001" && b.data[1].kode == "BK-002" &&
        b.data[2].kode == "BK-003" && b.data[3].kode == "BK-004", "urut kode naik");
    cek(b.data[0].judul == "Basis Data" && b.data[0].penulis == "Sari" && b.data[0].kategori == 1 &&
        b.data[3].judul == "Algoritma" && b.data[3].totalDipinjam == 2,
        "seluruh record ikut ditukar (bukan hanya kode)");

    urutkanJudul(b);
    cek(b.data[0].judul == "Algoritma" && b.data[1].judul == "Basis Data" &&
        b.data[2].judul == "Jaringan" && b.data[3].judul == "struktur data",
        "urut judul A-Z tanpa membedakan huruf besar/kecil");

    urutkanDipinjam(b);
    cek(b.data[0].kode == "BK-002" && b.data[1].kode == "BK-004" &&
        b.data[2].kode == "BK-001" && b.data[3].kode == "BK-003",
        "urut jumlah dipinjam terbanyak (seri -> kode naik)");
    cek(b.data[0].totalDipinjam >= b.data[1].totalDipinjam &&
        b.data[1].totalDipinjam >= b.data[2].totalDipinjam &&
        b.data[2].totalDipinjam >= b.data[3].totalDipinjam, "totalDipinjam menurun");

    // Transaksi menyimpan kode, bukan indeks, jadi hubungannya tetap benar.
    bool relasiBenar = true;
    for (int i = 0; i < t.n; i++) {
        if (cariKode(b, t.data[i].kodeBuku) == nullptr) relasiBenar = false;
    }
    cek(relasiBenar && cariKode(b, t.data[0].kodeBuku)->judul == "Jaringan",
        "hubungan transaksi-buku tetap benar setelah pengurutan");
    cek(kembalikanBuku(t, b, a, 1, tg(5, 3, 2026)) && cariKode(b, "BK-002")->stok == 8,
        "pengembalian setelah pengurutan memperbarui buku yang benar");
    bebaskan(t); bebaskan(a); bebaskan(b);
}

static void ujiRekap() {
    cout << "\n-- F7: rekap peminjaman (array 2D) --\n";
    DaftarBuku b; DaftarAnggota a; DaftarTransaksi t;
    siapkanData(b, a, t);
    int rekap[JML_KATEGORI][JML_BULAN];
    isiRekap(t, b, rekap, 2026);
    cek(totalRekap(rekap) == 0 && kategoriTerpopuler(rekap) == -1, "rekap kosong: total 0, tanpa kategori terpopuler");

    pinjamBuku(t, b, a, "AG-001", "BK-001", tg(3, 11, 2026));   // kategori 0, Nov
    pinjamBuku(t, b, a, "AG-002", "BK-001", tg(4, 11, 2026));   // kategori 0, Nov
    pinjamBuku(t, b, a, "AG-001", "BK-003", tg(1, 1, 2026));    // kategori 2, Jan
    pinjamBuku(t, b, a, "AG-002", "BK-004", tg(9, 3, 2027));    // kategori 3, tahun lain
    kembalikanBuku(t, b, a, 3, tg(5, 1, 2026));
    cek(hapusBuku(b, t, "BK-003") == HAPUS_OK, "buku dengan transaksi selesai dihapus");
    // Transaksi lama tanpa salinan kategori (-1) -> dicari dari daftar buku.
    tambahTransaksi(t, {9, "AG-002", "BK-004", {20, 12, 2026}, {0, 0, 0}, false, 0, -1});
    // Kategori -1 dan bukunya tidak ada -> tidak dapat dihitung, dilewati.
    tambahTransaksi(t, {10, "AG-002", "BK-888", {21, 12, 2026}, {0, 0, 0}, false, 0, -1});

    isiRekap(t, b, rekap, 2026);
    cek(rekap[0][10] == 2, "rekap[Pemrograman][Nov] = 2");
    cek(rekap[2][0] == 1, "transaksi buku yang sudah dihapus tetap terhitung");
    cek(rekap[3][11] == 1, "kategori -1 dicari dari daftar buku");
    cek(rekap[3][2] == 0, "transaksi tahun lain tidak ikut rekap 2026");
    cek(totalBaris(rekap, 0) == 2 && totalBaris(rekap, 2) == 1 && totalBaris(rekap, 1) == 0, "total setiap baris");
    cek(totalKolom(rekap, 10) == 2 && totalKolom(rekap, 0) == 1 && totalKolom(rekap, 11) == 1, "total setiap kolom");
    cek(totalRekap(rekap) == 4, "total keseluruhan rekap 2026");
    cek(kategoriTerpopuler(rekap) == 0, "kategori terpopuler = Pemrograman");
    isiRekap(t, b, rekap);   // versi PDF: semua tahun
    cek(totalRekap(rekap) == 5 && rekap[3][2] == 1, "isiRekap versi PDF menghitung semua tahun");

    string nama[JML_KATEGORI];
    for (int i = 0; i < JML_KATEGORI; i++) nama[i] = labelKategori(i);
    isiRekap(t, b, rekap, 2026);
    tampilRekap(rekap, nama, 2026);   // contoh tampilan pada keluaran uji
    bebaskan(t); bebaskan(a); bebaskan(b);
}

static void ujiDendaRiwayat() {
    cout << "\n-- F8: total denda dan riwayat (rekursif) --\n";
    Transaksi arr[4] = {
        {1, "AG-001", "BK-001", {1, 11, 2026}, {8, 11, 2026}, true, 0, 0},
        {2, "AG-002", "BK-002", {1, 11, 2026}, {11, 11, 2026}, true, 3000, 1},
        {3, "AG-001", "BK-003", {1, 11, 2026}, {18, 11, 2026}, true, 10000, 2},
        {4, "AG-001", "BK-004", {20, 11, 2026}, {0, 0, 0}, false, 0, 3}
    };
    cek(totalDendaRekursif(arr, 4) == 13000, "total denda rekursif = Rp13.000");
    cek(totalDendaRekursif(arr, 0) == 0, "base case n = 0 menghasilkan 0");
    cek(totalDendaRekursif(arr, 2) == 3000, "total denda sebagian array (n = 2)");

    cout << "   (riwayat AG-001:)\n";
    cek(riwayatAnggotaRekursif(arr, 4, 0, "AG-001") == 3, "riwayat AG-001: 3 transaksi");
    cout << "   (riwayat AG-002:)\n";
    cek(riwayatAnggotaRekursif(arr, 4, 0, "AG-002") == 1, "riwayat AG-002: 1 transaksi");
    cek(riwayatAnggotaRekursif(arr, 4, 0, "AG-003") == 0, "anggota tanpa transaksi: 0");
    cek(riwayatAnggotaRekursif(arr, 0, 0, "AG-001") == 0, "riwayat pada array kosong: 0");

    DaftarBuku b; DaftarAnggota a; DaftarTransaksi t;
    siapkanData(b, a, t);
    pinjamBuku(t, b, a, "AG-001", "BK-001", tg(1, 11, 2026));
    pinjamBuku(t, b, a, "AG-002", "BK-003", tg(1, 11, 2026));
    kembalikanBuku(t, b, a, 1, tg(10, 11, 2026));   // terlambat 2 hari
    kembalikanBuku(t, b, a, 2, tg(13, 11, 2026));   // terlambat 5 hari
    cek(totalDendaRekursif(t.data, t.n) == 7000, "total denda dari transaksi nyata = Rp7.000");
    bebaskan(t); bebaskan(a); bebaskan(b);
}

static bool berkasAda(const string& nama) {
    ifstream in(nama.c_str());
    return static_cast<bool>(in);
}

static void hapusBerkasUji(const string& awalan) {
    remove((awalan + "buku.txt").c_str());
    remove((awalan + "anggota.txt").c_str());
    remove((awalan + "transaksi.txt").c_str());
}

static void ujiPenyimpanan() {
    cout << "\n-- Fitur tambahan: penyimpanan berkas --\n";
    const string AWALAN = "uji_tmp_";
    hapusBerkasUji(AWALAN);

    DaftarBuku b; DaftarAnggota a; DaftarTransaksi t;
    siapkanData(b, a, t);
    tambahBuku(b, {"BK-010", "Pengantar   Struktur Data dan Algoritma", "Ni Made Sari Dewi", 4, 2020, 3, 0});
    pinjamBuku(t, b, a, "AG-001", "BK-010", tg(1, 11, 2026));
    pinjamBuku(t, b, a, "AG-002", "BK-001", tg(2, 11, 2026));
    kembalikanBuku(t, b, a, 1, tg(12, 11, 2026));   // 11 hari -> terlambat 4 -> Rp4.000
    cek(simpanData(b, a, t, AWALAN), "simpan data berhasil");
    cek(berkasAda(AWALAN + "buku.txt") && berkasAda(AWALAN + "anggota.txt") &&
        berkasAda(AWALAN + "transaksi.txt"), "tiga berkas data terbentuk");

    DaftarBuku b2; DaftarAnggota a2; DaftarTransaksi t2;
    inisialisasi(b2, 1); inisialisasi(a2, 1); inisialisasi(t2, 1);
    cek(muatData(b2, a2, t2, AWALAN), "muat data berhasil");
    cek(b2.n == b.n && a2.n == a.n && t2.n == t.n, "jumlah buku, anggota, transaksi sama");
    const Buku* pb = cariKode(b2, "BK-010");
    cek(pb != nullptr && pb->judul == "Pengantar   Struktur Data dan Algoritma" &&
        pb->penulis == "Ni Made Sari Dewi", "judul dan penulis berspasi tetap utuh");
    cek(pb != nullptr && pb->stok == 3 && pb->totalDipinjam == 1 && pb->kategori == 4,
        "stok dan totalDipinjam dimuat apa adanya");
    cek(cariAnggota(a2, "AG-002")->sedangPinjam == 1 && cariAnggota(a2, "AG-001")->sedangPinjam == 0,
        "sedangPinjam anggota dimuat apa adanya");
    const Transaksi* tr = cariTransaksi(t2, 1);
    cek(tr != nullptr && tr->selesai && tr->denda == 4000 && tr->tglKembali.hari == 12 &&
        tr->tglPinjam.bulan == 11 && tr->kategori == 4, "isi transaksi selesai dimuat lengkap");
    cek(cariTransaksi(t2, 2) != nullptr && !cariTransaksi(t2, 2)->selesai, "transaksi aktif tetap aktif");

    cek(muatData(b2, a2, t2, AWALAN) && b2.n == b.n && t2.n == t.n, "memuat dua kali tidak menggandakan data");
    cek(nomorBerikutnya(t2) == 3 && pinjamBuku(t2, b2, a2, "AG-001", "BK-004", tg(1, 12, 2026)) &&
        t2.data[t2.n - 1].nomor == 3, "nomor transaksi tetap unik setelah dimuat ulang");
    cek(kembalikanBuku(t2, b2, a2, 2, tg(5, 11, 2026)), "transaksi hasil muat dapat dikembalikan");

    DaftarBuku b3; DaftarAnggota a3; DaftarTransaksi t3;
    inisialisasi(b3, 1); inisialisasi(a3, 1); inisialisasi(t3, 1);
    cek(muatData(b3, a3, t3, "uji_tidak_ada_") && b3.n == 0 && a3.n == 0 && t3.n == 0,
        "berkas belum ada: data kosong tanpa error");

    {   // berkas buku dengan kode ganda: hanya satu yang dimuat
        ofstream f((AWALAN + "buku.txt").c_str());
        f << "SIPUS-BUKU\n2\nBK-001\nA\nB\n0\n2020\n1\n0\nBK-001\nC\nD\n0\n2020\n1\n0\n";
    }
    cek(!muatData(b3, a3, t3, AWALAN) && b3.n == 1, "record ganda dalam berkas ditolak saat memuat");
    {   // berkas rusak (penanda salah)
        ofstream f((AWALAN + "buku.txt").c_str());
        f << "BUKAN-SIPUS\n1\n";
    }
    cek(!muatData(b3, a3, t3, AWALAN) && b3.n == 0, "berkas rusak dideteksi tanpa crash");

    hapusBerkasUji(AWALAN);
    bebaskan(b); bebaskan(a); bebaskan(t);
    bebaskan(b2); bebaskan(a2); bebaskan(t2);
    bebaskan(b3); bebaskan(a3); bebaskan(t3);
    cek(b.data == nullptr && a.data == nullptr && t.data == nullptr &&
        b2.data == nullptr && t3.data == nullptr, "seluruh memori dinamis pengujian dibebaskan");
}

int main() {
    // Milestone 1-2
    ujiBukuTambahDanDuplikat();
    ujiBukuPembesaranBerulang();
    ujiBukuValidasi();
    ujiBukuCariUbahHapus();
    ujiAnggota();
    ujiTransaksiDanMemori();
    ujiValidatorFormat();
    // Milestone 3-4
    ujiTanggal();
    ujiPeminjaman();
    ujiPengembalian();
    ujiPencarian();
    ujiPengurutan();
    ujiRekap();
    ujiDendaRiwayat();
    ujiPenyimpanan();

    cout << "\n========================================\n"
         << "Total pengujian : " << (lulus + gagal) << '\n'
         << "Jumlah lulus    : " << lulus << '\n'
         << "Jumlah gagal    : " << gagal << '\n'
         << "========================================\n";
    return gagal == 0 ? 0 : 1;
}
