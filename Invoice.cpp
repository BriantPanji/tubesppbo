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

void Invoice::printBorder(char type, bool cetakFile = true) {
    switch (type) {
        case 'h':
            (cetakFile ? fileCetakBon : cout) << "╔═══════════════════════════════════════════════════╗\n";
            break;
        case 'b':
            (cetakFile ? fileCetakBon : cout) << "╠═══════════════════════════════════════════════════╣\n";
            break;
        case 'f':
            (cetakFile ? fileCetakBon : cout) << "╚═══════════════════════════════════════════════════╝\n";
            break;
        default:
            listError.push_back("ERR (Invoice::printBorder): Invalid border type.");
            break;
    }
}

void Invoice::printTeksTanya(string teks) {
    cout << "║ " << teks;
}

void Invoice::printTeks(string teks, bool cetakFile = true) {
    teks = strTrunc(teks, totalWidth);
    int textLength = teks.length();
    int padding = ((totalWidth - textLength) / 2) + 1;
    int extra = (totalWidth - textLength) % 2;

    (cetakFile ? fileCetakBon : cout) << "║"
                 << string(padding, ' ')
                 << teks
                 << string(padding + extra, ' ')
                 << "║\n";
}

void Invoice::printTeks(string teks1, string teks2, bool cetakFile = true) {
    bool isOneOnly = teks2 == " ";
    teks1 = strTrunc(teks1, (isOneOnly ? totalWidth - 2 : (totalWidth / 2) + 1));
    teks2 = strTrunc(teks2, (isOneOnly ? 2 : totalWidth / 2));
    (cetakFile ? fileCetakBon : cout) << "║ " << left << setw(isOneOnly ? totalWidth-2 : (totalWidth / 2) + 1) << teks1
                 << right << setw(isOneOnly ? 2 : totalWidth / 2) << teks2
                 << " ║\n";
}

void Invoice::printTeksHarga(string teks1, string teks2, string teks3, bool cetakFile = true) {
    teks1 = strTrunc(teks1, (totalWidth / 3) + 1);
    teks2 = strTrunc(teks2, (totalWidth / 3) - 1);
    teks3 = strTrunc(teks3, (totalWidth / 3));
    (cetakFile ? fileCetakBon : cout) << "║ " << left << setw((totalWidth / 3) + 1) << setfill('.') << teks1
                 << right << setw((totalWidth / 3)) << teks2
                 << right << setw(totalWidth / 3) << teks3 << setfill(' ')
                 << " ║\n";
}

void Invoice::printTeksSisi(string teks, char side = 'l', bool cetakFile = true) {
    if (teks.length() > totalWidth) {
        listError.push_back("WARN (Invoice::printTeksSisi): Text is too long.");
        teks = strTrunc(teks, totalWidth);
    }
    if (side != 'l' && side != 'r') {
        listError.push_back("ERR (Invoice::printTeksSisi): Invalid side param.");
        side = 'l';
    }
    (cetakFile ? fileCetakBon : cout) << "║ " << (side == 'l' ? left : right) << setw(totalWidth) << teks << " ║\n";
}

void Invoice::printHeader(bool cetakFile = true) {
    printBorder('h', cetakFile);
    printTeks(namaToko);
    printBorder('b', cetakFile);
    printBorder('b', cetakFile);
    printTeks(namaFile, getWaktu());
    printTeks("  ");
    printTeksSisi("Alamat   : " + alamatToko);
    printTeksSisi("No. Telp : " + noTelpToko);
    printTeksSisi("Kasir    : " + namaKasir);
    printBorder('b', cetakFile);
    printBorder('b', cetakFile);
}

void Invoice::printFooter(bool cetakFile = true) {
    printBorder('b', cetakFile);
    printBorder('b', cetakFile);
    printTeks("TERIMA KASIH ATAS KUNJUNGAN ANDA");
    printTeks("SILAHKAN DATANG KEMBALI");
    printBorder('f', cetakFile);
}

void Invoice::tambahBelian(int id, unsigned int jumlah) {
    auto it = find_if(listBarang.begin(), listBarang.end(), [id](Item &item) { return item.id == id; });
    if (it != listBarang.end()) {
        auto ite = find_if(listBelian.begin(), listBelian.end(), [it](Belian &item) { return item.id == it->id; });
        if (ite != listBelian.end()) {
            if (ite->jumlah + jumlah > it->jumlah)
                ite->jumlah = it->jumlah;
            else
                ite->jumlah += jumlah;
        }
        else
            listBelian.push_back({it->id, jumlah == 0 ? 1 : jumlah  });
    } else {
        listError.push_back("ERR (Invoice::tambahBelian): Item not found.");
    }
}
void Invoice::tambahBelian(Belian belian) {
    tambahBelian(belian.id, belian.jumlah);
}
void Invoice::editBelian(int id, unsigned int jumlah) {
    auto it = find_if(listBelian.begin(), listBelian.end(), [id](const Belian &belian) { return belian.id == id; });
    if (it != listBelian.end()) {
        it->jumlah = jumlah;
    } else {
        listError.push_back("ERR (Invoice::editBelian): Item not found.");
    }
}
void Invoice::hapusBelian(int id) {
    auto it = find_if(listBelian.begin(), listBelian.end(), [id](const Belian &belian) { return belian.id == id; });
    if (it != listBelian.end()) {
        listBelian.erase(it);
    } else {
        listError.push_back("ERR (Invoice::hapusBelian): Item not found.");
    }
}

void Invoice::printIsi(bool cetakFile = true) {
    int totalHarga = 0;

    for (int i = 0; i < listBelian.size(); i++) {
        auto it = find_if(listBarang.begin(), listBarang.end(), [this, i](const Item &item) { return item.id == listBelian.at(i).id; });
        unsigned int jlhBrg = listBelian.at(i).jumlah;
        if (it != listBarang.end()) {
            totalHarga += (jlhBrg * (it->harga - (float(it->harga) * it->diskon)));
            printTeks(it->nama, formatRupiah(it->harga), cetakFile);
            if (it->diskon > 0) {
                printTeksHarga(to_string(jlhBrg) + " x", " " + to_string((int)(it->diskon*100)) + "% OFF ", formatRupiah(jlhBrg * (it->harga - (it->harga * it->diskon))), cetakFile);
            } else {
                printTeksHarga(to_string(jlhBrg) + " x", ".", formatRupiah(jlhBrg * it->harga), cetakFile);
            }
        } else {
            listError.push_back("ERR (Invoice::printIsi): Item not found in inventory.");
        }
    }

    printBorder('b', cetakFile);
    printTeks("TOTAL HARGA", formatRupiah(totalHarga), cetakFile);
    printTeks("PAJAK", to_string(pajak) + "%", cetakFile);
    printBorder('b', cetakFile);
    totalBayar = totalHarga + (totalHarga * pajak / 100);
    printTeks("TOTAL BAYAR", formatRupiah(totalBayar), cetakFile);
}

void Invoice::cetakBon(bool cetakFile = true) {
    fileCetakBon.open(dirCetakan + namaFile + "_bon.txt");
    if (!fileCetakBon.is_open()) {
        listError.push_back("ERR (Invoice::cetakBon): Failed to open file.");
        return;
    }

    printBorder('h', false);
    printTeks(string(INFO "Cetak bon ke file: ") + dirCetakan + namaFile + "_bon.txt", false);
    printBorder('f', false);

    printHeader(cetakFile);
    printIsi(cetakFile);
    printFooter(cetakFile);
    fileCetakBon.close();
    namaFile = randomId();
}

void Invoice::hapusListBelian() {
    listBelian.clear();
}

Belian Invoice::printTambahBelian() {
    string choice;
    Belian belian;
    string tempTeks;

    while (true) {
        cout << endl;
        printBorder('h', false);
        printTeks("TAMBAH BARANG", false);
        printTeks("Masukkan 'x' untuk keluar dari program Invoice.", false);
        printTeks("Masukkan ID barang yang ingin ditambahkan.", false);
        printBorder('b', false);
        printTeksTanya("ID Barang: ");
        getline(cin, choice);

        if (choice == "x" || choice == "X") {
            printBorder('b', false);
            printTeks("Keluar dari tambah Invoice.", false);
            belian.jumlah = 0;
            belian.id = -1;
            break;
        }

        try {
            belian.id = stoi(choice);
        } catch (...) {
            printTeks("ERR (Invoice::printTambahBelian): Invalid input.", false);
            printBorder('f', false);
            continue;
        }

        auto it = find_if(listBarang.begin(), listBarang.end(), [belian](const Item &item) { return item.id == belian.id; });
        if (it == listBarang.end()) {
            printTeks("ERR (Invoice::printTambahBelian): Item not found.", false);
            printBorder('f', false);
            continue;
        }
        auto ite = find_if(listBelian.begin(), listBelian.end(), [it](Belian &beli) { return beli.id == it->id; });
        bool itemDiInvoice = ite != listBelian.end();
        if ((it->jumlah < 1) || (itemDiInvoice && (it->jumlah - ite->jumlah < 1))) {
            printTeks("ERR: Item tidak cukup.", false);
            printBorder('f', false);
            continue;
        }
        belian.id = it->id;
        choice.clear();
        tempTeks = "Jumlah barang (tersedia: "+ to_string(it->jumlah);
        if (itemDiInvoice) tempTeks += "-" + to_string(ite->jumlah);
        tempTeks += "): ";
        printTeksTanya(tempTeks);
        getline(cin, choice);

        if (choice == "x" || choice == "X") {
            printBorder('b', false);
            printTeks("Keluar dari tambah Invoice.", false);
            belian.jumlah = 0;
            belian.id = -1;
            return belian;
        }

        try {
            belian.jumlah = stoi(choice);
        } catch (...) {
            printTeks("ERR (Invoice::printTambahBelian): Invalid input.",false);
            printBorder('f', false);
            continue;
        }

        if (belian.jumlah > it->jumlah) {
            printBorder('b', false);
            printTeks("WARN: Jumlah barang melebihi stok.", false);
            tempTeks = INFO "Barang yang dimasukkan ke Invoice adalah: " + to_string(it->jumlah);
            printTeks(tempTeks, false);
            printBorder('b', false);
            belian.jumlah = it->jumlah;
        }
        if (belian.jumlah == 0) {
            printTeks("ERR (Invoice::printTambahBelian): Jumlah barang tidak boleh 0.", false);
            printTeks("Barang yang dimasukkan ke Invoice adalah: 1", false);
            printBorder('b', false);
            belian.jumlah = 1;
        }

        printBorder('b', false);
        printTeks("Barang ditambahkan ke invoice.", false);
        printBorder('f', false);
        return belian;
    }
    printBorder('f', false);
    return belian;
}


int Invoice::mulaiBeli() {
    while (true) {
        Belian belian = printTambahBelian();
        if (belian.id == -1) {
            return -1;
        }

        if (belian.jumlah == 0) {
            printBorder('h', false);
            printTeks("Jumlah tidak valid.", false);
            printBorder('f', false);
            continue;
        }

        tambahBelian(belian);
    }
    return 0;
}

void Invoice::displaySementara() {
    // if (listBelian.empty()) return;
    int totalHarga = 0, jlh;

    printBorder('h', false);
    printTeks("INVOICE SEMENTARA", false);
    printBorder('b', false);
    printIsi(false);
    printBorder('f', false);
}



void Invoice::setListBarang(vector<Item> barangs) { listBarang = barangs; }
void Invoice::setNamaToko(string str) { namaToko = str; strUpper(namaToko); }
void Invoice::setPajak(int pajak) { this->pajak = pajak; }
void Invoice::setAlamatToko(string str) { alamatToko = str; }
void Invoice::setNoTelpToko(string str) { noTelpToko = str; }
void Invoice::setNamaKasir(string str) { namaKasir = str; }
int Invoice::getTotalBayar() { return totalBayar; }
