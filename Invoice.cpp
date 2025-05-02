// Invoice.cpp
#include "Invoice.hpp"
using namespace std;

string Invoice::randomId() {
    string id = "INV";
    for (int i = 0; i < 6; i++) {
        id += to_string(rand() % 10);
    }
    return id;
}

Invoice::Invoice() {
    time_t timeNow = time(0);
    srand(timeNow);
    ltm = localtime(&timeNow);
    string namaBulan[] = {"Januari", "Februari", "Maret", "April", "Mei", "Juni", "Juli", "Agustus", "September", "Oktober", "November", "Desember"};
    bulanNow = namaBulan[ltm->tm_mon];
    namaFile = randomId();

    if (!filesystem::exists(dirCetakan)) {
        filesystem::create_directories(dirCetakan);
    }
    fileCetakBon.open(dirCetakan + namaFile + "_bon.txt");

    Inventory inv;
    listBarang = inv.getListBarang();
    namaToko = "Toko Mipo";
    totalBayar = 0;
    pajak = 11;
    strUpper(namaToko);
    alamatToko = "Jl. Dr. T. Mansur No. 9, Kampus Padang Bulan, Medan, 20155, Sumatera Utara";
    noTelpToko = "082168889060";
    namaKasir = "Panji Depari";
}

Invoice::~Invoice() {
    fileCetakBon.close();
    if (listError.empty()) return;
    cerr << endl << "Error & Warn: " << endl;
    for (auto &err : listError) {
        cerr << " - " << err << endl;
    }
}

string Invoice::getWaktu() {
    ostringstream oss;
    oss << setfill('0')
        << setw(2) << ltm->tm_mday << " "
        << bulanNow << " "
        << (ltm->tm_year + 1900) << " "
        << setw(2) << ltm->tm_hour << ":"
        << setw(2) << ltm->tm_min << ":"
        << setw(2) << ltm->tm_sec;
    return oss.str();
}

void Invoice::printTeks(string teks) {
    int textLength = teks.length();
    teks = strTrunc(teks, totalWidth);
    int padding = ((totalWidth - textLength) / 2) + 1;
    int extra = (totalWidth - textLength) % 2;

    fileCetakBon << "║"
                 << string(padding, ' ')
                 << teks
                 << string(padding + extra, ' ')
                 << "║" << endl;
}

void Invoice::printTeks(string teks1, string teks2) {
    teks1 = strTrunc(teks1, (totalWidth / 2) + 1);
    teks2 = strTrunc(teks2, totalWidth / 2);
    fileCetakBon << "║ " << left << setw((totalWidth / 2) + 1) << teks1
                 << right << setw(totalWidth / 2) << teks2
                 << " ║" << endl;
}

void Invoice::printTeksHarga(string teks1, string teks2, string teks3) {
    teks1 = strTrunc(teks1, (totalWidth / 3) + 1);
    teks2 = strTrunc(teks2, (totalWidth / 3) - 1);
    teks3 = strTrunc(teks3, (totalWidth / 3));
    fileCetakBon << "║ " << left << setw((totalWidth / 3) + 1) << setfill('.') << teks1
                 << right << setw((totalWidth / 3)) << teks2
                 << right << setw(totalWidth / 3) << teks3 << setfill(' ')
                 << " ║" << endl;
}

void Invoice::printTeksSisi(string teks, char side) {
    if (teks.length() > totalWidth) {
        listError.push_back("WARN (Invoice::printTeksSisi): Text is too long.");
        teks = strTrunc(teks, totalWidth);
    }
    if (side != 'l' && side != 'r') {
        listError.push_back("ERR (Invoice::printTeksSisi): Invalid side param.");
        side = 'l';
    }
    fileCetakBon << "║ " << (side == 'l' ? left : right) << setw(totalWidth) << teks << " ║" << endl;
}

void Invoice::printHeader() {
    fileCetakBon << "╔═══════════════════════════════════════════════════╗" << endl;
    printTeks(namaToko);
    fileCetakBon << "╠═══════════════════════════════════════════════════╣" << endl;
    fileCetakBon << "╠═══════════════════════════════════════════════════╣" << endl;
    printTeks(namaFile, getWaktu());
    printTeks("  ");
    printTeksSisi("Alamat:    " + alamatToko);
    printTeksSisi("No. Telp:  " + noTelpToko);
    printTeksSisi("Kasir:     " + namaKasir);
    fileCetakBon << "╠═══════════════════════════════════════════════════╣" << endl;
    fileCetakBon << "╠═══════════════════════════════════════════════════╣" << endl;
}

void Invoice::printFooter() {
    fileCetakBon << "╠═══════════════════════════════════════════════════╣" << endl;
    fileCetakBon << "╠═══════════════════════════════════════════════════╣" << endl;
    printTeks("TERIMA KASIH ATAS KUNJUNGAN ANDA");
    printTeks("SILAHKAN DATANG KEMBALI");
    fileCetakBon << "╚═══════════════════════════════════════════════════╝" << endl;
}

void Invoice::tambahBarang(ItemNoId brg) {
    listBelian.push_back({brg.nama, brg.jumlah, brg.harga, brg.diskon});
}
void Invoice::tambahBarang(string namaBrg, unsigned int jlhBrg, unsigned int hargaBrg, float diskon) {
    tambahBarang({namaBrg, jlhBrg, hargaBrg, diskon});
}

void Invoice::printIsi() {
    int totalHarga = 0;

    for (auto &i : listBelian) {
        totalHarga += (i.jumlah * (i.harga - (float(i.harga) * i.diskon)));
        printTeks(i.nama, formatRupiah(i.harga));
        if (i.diskon > 0) {
            string diskon = to_string(i.diskon);
            printTeksHarga(to_string(i.jumlah) + " x", " " + diskon.substr(0, diskon.find(".") + 3) + "% OFF ", formatRupiah(i.jumlah * (i.harga - (i.harga * i.diskon))));
        } else {
            printTeksHarga(to_string(i.jumlah) + " x", ".", formatRupiah(i.jumlah * i.harga));
        }
    }

    fileCetakBon << "╠═══════════════════════════════════════════════════╣" << endl;
    printTeks("TOTAL HARGA", formatRupiah(totalHarga));
    printTeks("PAJAK", to_string(pajak) + "%");
    fileCetakBon << "╠═══════════════════════════════════════════════════╣" << endl;
    totalBayar = totalHarga + (totalHarga * pajak / 100);
    printTeks("TOTAL BAYAR", formatRupiah(totalBayar));
}

void Invoice::cetakBon() {
    if (!fileCetakBon.is_open()) {
        listError.push_back("ERR (Invoice::cetakBon): Failed to open file.");
        return;
    }

    printHeader();
    printIsi();
    printFooter();
}

void Invoice::setListBarang(vector<Item> barangs) { listBarang = barangs; }
void Invoice::setNamaToko(string str) { namaToko = str; strUpper(namaToko); }
void Invoice::setPajak(int pajak) { this->pajak = pajak; }
void Invoice::setAlamatToko(string str) { alamatToko = str; }
void Invoice::setNoTelpToko(string str) { noTelpToko = str; }
void Invoice::setNamaKasir(string str) { namaKasir = str; }
int Invoice::getTotalBayar() { return totalBayar; }
void Invoice::debug() { /* implementasi debug jika perlu */ }
