// anggota.cpp - F2: kelola anggota (tambah, ubah, tampilkan)
// Penanggung jawab : I Gusti Ayu Nandini Amara Santi (NIM 260530911060)
// Kelompok 7 - Kelas E
//
// Struktur data: array of struct dinamis (Anggota*) dengan new[]/delete[].
// Fungsi yang hanya membaca data menerima parameter const (referensi konstan).
#include <iostream>
#include <iomanip>
#include <new>
#include "sipus.h"

using namespace std;

// ----------------------------------------------------------------------
// Array dinamis
// ----------------------------------------------------------------------

void inisialisasi(DaftarAnggota& d, int kapasitasAwal) {
    if (kapasitasAwal < 1) kapasitasAwal = 1;
    d.data = new (nothrow) Anggota[kapasitasAwal];
    d.n = 0;
    d.kapasitas = (d.data != nullptr) ? kapasitasAwal : 0;
}

void bebaskan(DaftarAnggota& d) {
    delete[] d.data;
    d.data = nullptr;
    d.n = 0;
    d.kapasitas = 0;
}

static bool perbesar(DaftarAnggota& d) {
    int kapasitasBaru = (d.kapasitas < 1) ? 1 : d.kapasitas * 2;
    Anggota* baru = new (nothrow) Anggota[kapasitasBaru];
    if (baru == nullptr) return false;
    for (int i = 0; i < d.n; i++) {
        baru[i] = d.data[i];
    }
    delete[] d.data;
    d.data = baru;
    d.kapasitas = kapasitasBaru;
    return true;
}

// ----------------------------------------------------------------------
// Pencarian
// ----------------------------------------------------------------------

static int cariIndeks(const DaftarAnggota& d, const string& id) {
    for (int i = 0; i < d.n; i++) {
        if (d.data[i].id == id) return i;
    }
    return -1;
}

Anggota* cariAnggota(DaftarAnggota& d, const string& id) {
    int i = cariIndeks(d, id);
    return (i >= 0) ? &d.data[i] : nullptr;
}

const Anggota* cariAnggota(const DaftarAnggota& d, const string& id) {
    int i = cariIndeks(d, id);
    return (i >= 0) ? &d.data[i] : nullptr;
}

// ----------------------------------------------------------------------
// Validasi
// ----------------------------------------------------------------------

string validasiNamaProdi(const string& nama, const string& prodi) {
    if (trim(nama).empty()) return "Nama tidak boleh kosong.";
    if (trim(prodi).empty()) return "Program studi tidak boleh kosong.";
    return "";
}

string validasiAnggota(const DaftarAnggota& d, const Anggota& a) {
    if (!formatIdAnggota(a.id)) return "Format ID harus AG-xxx (xxx = 3 digit).";
    if (cariIndeks(d, a.id) >= 0) return "ID " + a.id + " sudah digunakan.";
    return validasiNamaProdi(a.nama, a.prodi);
}

// ----------------------------------------------------------------------
// Tambah dan ubah
// ----------------------------------------------------------------------

bool tambahAnggota(DaftarAnggota& d, const Anggota& a) {
    if (!validasiAnggota(d, a).empty()) return false;
    if (d.n >= d.kapasitas) {
        if (!perbesar(d)) return false;
    }
    Anggota baru = a;
    baru.nama = trim(a.nama);
    baru.prodi = trim(a.prodi);
    baru.sedangPinjam = 0;           // anggota baru belum meminjam apa pun
    d.data[d.n] = baru;
    d.n++;
    return true;
}

bool ubahAnggota(DaftarAnggota& d, const string& id, const string& nama,
                 const string& prodi) {
    Anggota* p = cariAnggota(d, id);
    if (p == nullptr) return false;
    if (!validasiNamaProdi(nama, prodi).empty()) return false;
    // Hanya nama dan prodi yang berubah. id dan sedangPinjam sengaja dibiarkan.
    p->nama = trim(nama);
    p->prodi = trim(prodi);
    return true;
}

// ----------------------------------------------------------------------
// Tampilan tabel
// ----------------------------------------------------------------------

void tampilAnggota(const DaftarAnggota& d) {
    cout << "\n=== DAFTAR ANGGOTA ===\n";
    if (d.n == 0) {
        cout << "(Belum ada data anggota.)\n";
        return;
    }
    const int W_NO = 3, W_ID = 6, W_NAMA = 26, W_PRODI = 26, W_PINJAM = 13;
    const int LEBAR = W_NO + W_ID + W_NAMA + W_PRODI + W_PINJAM + 4;

    cout << left;
    cout << garis(LEBAR) << '\n'
         << setw(W_NO) << "No" << ' ' << setw(W_ID) << "ID" << ' '
         << setw(W_NAMA) << "Nama" << ' ' << setw(W_PRODI) << "Prodi" << ' '
         << setw(W_PINJAM) << "Sedang Pinjam" << '\n'
         << garis(LEBAR) << '\n';
    for (int i = 0; i < d.n; i++) {
        const Anggota& a = d.data[i];
        cout << setw(W_NO) << (i + 1) << ' ' << setw(W_ID) << a.id << ' '
             << setw(W_NAMA) << potong(a.nama, W_NAMA) << ' '
             << setw(W_PRODI) << potong(a.prodi, W_PRODI) << ' '
             << setw(W_PINJAM) << a.sedangPinjam << '\n';
    }
    cout << garis(LEBAR) << '\n';
    cout << right;
    cout << "Total: " << d.n << " anggota.\n";
}

// ----------------------------------------------------------------------
// Antarmuka interaktif F2
// ----------------------------------------------------------------------

void prosesTambahAnggota(DaftarAnggota& d) {
    cout << "\n--- Tambah Anggota ---\n";
    Anggota a = {"", "", "", 0};
    string id;
    while (true) {
        if (bacaTeks("ID anggota (AG-xxx, kosongkan untuk batal): ", id, true) != BACA_OK) {
            cout << ">> Dibatalkan.\n";
            return;
        }
        id = hurufBesar(id);
        if (!formatIdAnggota(id)) {
            cout << ">> Format ID harus AG-xxx (xxx = 3 digit), contoh AG-001.\n";
        } else if (cariAnggota(d, id) != nullptr) {
            cout << ">> ID " << id << " sudah digunakan.\n";
        } else {
            break;
        }
    }
    a.id = id;
    if (bacaTeks("Nama anggota   : ", a.nama, false) != BACA_OK) return;
    if (bacaTeks("Program studi  : ", a.prodi, false) != BACA_OK) return;
    a.sedangPinjam = 0;

    if (tambahAnggota(d, a)) {
        cout << ">> Anggota " << a.id << " berhasil ditambahkan.\n";
    } else {
        string pesan = validasiAnggota(d, a);
        cout << ">> Gagal menambah anggota: "
             << (pesan.empty() ? "memori tidak cukup." : pesan) << '\n';
    }
}

void prosesUbahAnggota(DaftarAnggota& d) {
    cout << "\n--- Ubah Anggota ---\n";
    if (d.n == 0) {
        cout << ">> Belum ada data anggota.\n";
        return;
    }
    string id;
    if (bacaTeks("ID anggota yang diubah (kosongkan untuk batal): ", id, true) != BACA_OK) {
        cout << ">> Dibatalkan.\n";
        return;
    }
    id = hurufBesar(id);
    const Anggota* lama = cariAnggota(d, id);
    if (lama == nullptr) {
        cout << ">> Anggota dengan ID " << id << " tidak ditemukan.\n";
        return;
    }
    cout << "Data saat ini:\n"
         << "  ID            : " << lama->id << '\n'
         << "  Nama          : " << lama->nama << '\n'
         << "  Prodi         : " << lama->prodi << '\n'
         << "  Sedang pinjam : " << lama->sedangPinjam << '\n'
         << "(Tekan Enter tanpa mengetik untuk mempertahankan nilai lama)\n";

    string nama = lama->nama;
    string prodi = lama->prodi;
    string teks;
    HasilBaca r = bacaTeks("Nama baru  : ", teks, true);
    if (r == BACA_EOF) return;
    if (r == BACA_OK) nama = teks;
    r = bacaTeks("Prodi baru : ", teks, true);
    if (r == BACA_EOF) return;
    if (r == BACA_OK) prodi = teks;

    if (ubahAnggota(d, id, nama, prodi)) {
        cout << ">> Data anggota " << id << " berhasil diubah.\n";
    } else {
        cout << ">> Gagal mengubah anggota: " << validasiNamaProdi(nama, prodi) << '\n';
    }
}

void menuAnggota(DaftarAnggota& d) {
    int pilihan = 0;
    do {
        cout << "\n-------- KELOLA ANGGOTA --------\n"
             << "1. Tambah Anggota\n"
             << "2. Ubah Anggota\n"
             << "3. Tampilkan Anggota\n"
             << "0. Kembali\n"
             << "--------------------------------\n";
        if (bacaInt("Masukkan pilihan: ", 0, 3, pilihan, false) != BACA_OK) return;
        switch (pilihan) {
            case 1: prosesTambahAnggota(d); break;
            case 2: prosesUbahAnggota(d); break;
            case 3: tampilAnggota(d); break;
            default: break;
        }
    } while (pilihan != 0);
}
