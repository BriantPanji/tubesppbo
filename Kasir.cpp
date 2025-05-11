// PROGRAM INI HANYA BISA DIJALANKAN DI WINDOWS
// PROGRAM INI HANYA BISA DIJALANKAN DI TERMINAL WINDOWS: cmd / powershell

#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <windows.h>
#include "Kasir.hpp"

using namespace std;

Kasir::Kasir() {
    system("cls");
    SetConsoleOutputCP(CP_UTF8);
    setListBarang(Inventory::getListBarang());
}

Kasir::~Kasir() {}

void Kasir::tambahBarang() {
    string namaBrg, idStr;
    unsigned int jlhBrg, hargaBrg;
    float diskon;
    bool validate = 1;

    do{
        system("pause");
        system("cls");

        
        printBorder('h', false);
        printTeks("TAMBAH BARANG", false);
        printBorder('b', false);
        printTeks("Ketik 'x' untuk Keluar dari program ini.", false);
        printBorder('b', false);
        printTeksTanya(ASK "Masukkkan nama barang: ");
        getline(cin, namaBrg);
        if (namaBrg == "x" || namaBrg == "X") {
            printBorder('f', false);
            printBorder('h', false);
            printTeks("Batal menambahkan barang.", false);
            printBorder('f', false);
            return;
        }
        if (namaBrg.empty() || namaBrg.length() < 2) {
            printBorder('b', false);
            printTeks(ERR "Nama barang tidak valid. Harus diisi.", false);
            printBorder('f', false);
            validate = 0;
            continue;
        }
       printTeksTanya(ASK "Masukkan jumlah barang: ");
        cin >> jlhBrg;
        if (jlhBrg <= 0) {
            printBorder('b', false);
            printTeks(ERR "Jumlah barang tidak valid. Harus lebih dari 0.", false);
            printBorder('f', false);
            validate = 0;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

       printTeksTanya(ASK "Masukkan harga barang: ");
        cin >> hargaBrg;
        if (hargaBrg <= 0) {
            printBorder('b', false);
            printTeks(ERR "Harga barang tidak valid. Harus lebih dari 0.", false);
            printBorder('f', false);
            validate = 0;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

       printTeksTanya(ASK "Masukkan diskon barang (0-100): ");
        cin >> diskon;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        diskon = diskon / 100.0f; // Convert to decimal
        if (diskon < 0 || diskon > 1) {
            printBorder('b', false);
            printTeks(ERR "Diskon tidak valid. Harus antara 0 dan 100.", false);
            printBorder('f', false);
            validate = 0;
            continue;
        }
        printBorder('f', false);
        
    } while(validate != 1);
    


    Inventory::tambahBarang(namaBrg, jlhBrg, hargaBrg, diskon);
    saveFileBarang();
    printBorder('h', false);
    printTeks("Barang baru telah ditambahkan.", false);
    printBorder('f', false);
}

void Kasir::hapusBarang() {
    string idStr;
    printBorder('h', false);
    printTeks("Menghapus Barang dari Inventory", false);
    printTeks("Masukkan 'x' untuk membatalkan menghapus", false);
    printBorder('b', false);
    printTeksTanya("Masukkan ID barang: ");
    getline(cin, idStr);
    printBorder('f', false);
    if (idStr == "x" || idStr == "X") {
        printBorder('h', false);
        printTeks("Batal menghapus barang.", false);
        printBorder('f', false);
        return;
    }
    if (idStr.empty()) {
        printBorder('h', false);
        printTeks(ERR "ID barang tidak valid. Harus diisi.", false);
        printBorder('f', false);
        system("cls");
        return hapusBarang();
    }
    int id = stoi(idStr);
    if (Inventory::hapusBarang(id)) {
        printBorder('h', false);
        printTeks("Barang berhasil dihapus", false);
        printBorder('f', false);
        saveFileBarang();
    } else {
        printBorder('h', false);
        printTeks("Barang gagal dihapus", false);
        printBorder('f', false);

    }
}

void Kasir::editBarang() {
    string idStr;
    printBorder('h', false);
    printTeks("Mengedit Barang dari Inventory", false);
    printTeks("Masukkan 'x' untuk membatalkan menghapus", false);
    printTeks("Masukkan '0' jika tidak ingin mengubah", false);
    printTeks("sebuah data", false);
    printBorder('b', false);
    printTeksTanya("Masukkan ID barang: ");
    getline(cin, idStr);
    printBorder('f', false);
    if (idStr == "x" || idStr == "X") {
        printBorder('h', false);
        printTeks("Batal mengedit barang.", false);
        printBorder('f', false);
        return;
    }
    if (idStr.empty()) {
        printBorder('h', false);
        printTeks(ERR "ID barang tidak valid. Harus diisi.", false);
        printBorder('f', false);
        system("cls");
        return hapusBarang();
    }

    int id = stoi(idStr);

    auto it = find_if(Inventory::listBarang.begin(), Inventory::listBarang.end(), [id](Item &item) { return item.id == id; });
    if (it == Inventory::listBarang.end()) {
        printBorder('h', false);
        printTeks("ERR (Kasir::editBarang): Item not found.", false);
        printBorder('f', false);
        return;
    }

    ItemNoId brg;

    printBorder('h', false);
    printTeks("Masukkan nama barang baru (" + it->nama + ")", " ", false);
    printTeksTanya("Nama: ");
    getline(cin, brg.nama);
    if (brg.nama == "0") brg.nama.clear();
    if (brg.nama.empty() || brg.nama.length() < 3) {
        printBorder('b', false);
        printTeks("Nama tidak valid!", false);
        printBorder('f', false);
        system("pause");
        system("cls");
        return editBarang();
    }

    printBorder('b', false);
    printTeks("Masukkan jumlah barang baru (" + to_string(it->jumlah) + ")", " ", false);
    printTeksTanya("Jumlah: ");
    cin >> brg.jumlah; 
    if (brg.jumlah < 0) {
        printBorder('b', false);
        printTeks("Jumlah tidak valid!", false);
        printBorder('f', false);
        system("pause");
        system("cls");
        return editBarang();
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    printBorder('b', false);
    printTeks("Masukkan harga barang baru (" + to_string(it->harga) + ")", " ", false);
    printTeksTanya("Harga: ");
    cin >> brg.harga;
    if (brg.harga < 0) {
        printBorder('b', false);
        printTeks("Harga tidak valid!", false);
        printBorder('f', false);
        system("pause");
        system("cls");
        return editBarang();
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    printBorder('b', false);
    printTeks("Masukkan diskon barang baru (" + to_string((int)(it->diskon*100)) + "%)", " ", false);
    printTeks("Masukkan '-' untuk tidak mengubah persen", " ", false);
    printTeksTanya("Diskon: ");
    string tempStr;
    getline(cin, tempStr);
    if (tempStr == "-")
        brg.diskon = -1;
    else
        brg.diskon = stof(tempStr);
    if (brg.diskon < -1) {
        printBorder('b', false);
        printTeks("Diskon tidak valid!", false);
        printBorder('f', false);
        system("pause");
        system("cls");
        return editBarang();
    }
    
    printBorder('f', false);
    Inventory::editBarang(id, brg);
    saveFileBarang();
    printBorder('h', false);
    printTeks("Barang selesai di-Edit!", false);
    printBorder('f', false);
}

void Kasir::tampilkanBarang() {
    string idStr;
    cout << ASK "Masukkan ID barang yang ingin ditampilkan: ";
    getline(cin, idStr);
    int id = stoi(idStr);
    Inventory::tampilkanBarang(id);
}

void Kasir::tampilkanBarangDiskon() {
    vector<int> listId;
    for (auto &i : Inventory::listBarang) {
        if (i.diskon > 0) {
            listId.push_back(i.id);
        }
    }
    if (listId.empty()) {
        cout << ERR "Tidak ada barang dengan diskon.\n";
        return;
    }
    printBorder('h', false);
    cout << "║ ID  NAMA BARANG                 STOK        HARGA ║\n";
    printBorder('b', false);
    for (auto &i : listId) {
        auto it = find_if(Inventory::listBarang.begin(), Inventory::listBarang.end(), [i](Item &item) { return item.id == i; });
        if (it != Inventory::listBarang.end()) {
            cout << "║ " << setw(2) << it->id << "  " << setw(20) << it->nama << setw(10) << it->jumlah << setw(10) << Inventory::formatRupiah(it->harga) << " ║\n";
        }
    }
    cout << "0\n";
    cout << SUCC "Barang dengan diskon telah ditampilkan.\n";
}

void Kasir::printHeader() {
    printBorder('h', false);
    printTeks("SISTEM KASIR", false);
    printBorder('b', false);
    printTeks("1. Cetak Bon", " ", false);
    printTeks("2. Tampilkan Semua Barang", " ", false);
    printTeks("3. Tampilkan Barang Diskon", " ", false);
    printTeks("4. Tambah Barang", " ", false);
    printTeks("5. Hapus Barang", " ", false);
    printTeks("6. Edit Barang", " ", false);
    printTeks("0. Keluar dari Program", " ", false);
    printBorder('b', false);
}

char Kasir::getPilihan() {
    char pilihan;
    printTeksTanya("Pilih menu (0-6): ");
    pilihan = cin.get();
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
    printBorder('f', false);
    if (pilihan < '0' || pilihan > '6') {
        printBorder('h', false);
        printTeks("Pilihan tidak valid. Silakan coba lagi.", false);
        printBorder('f', false);
        printBorder('h', false);
        return getPilihan();
    }
    return pilihan;
}

char Kasir::beliLagi() {
    char pilihan;

    printBorder('h', false);
    printTeks("1. Tambah Produk ke Invoice", " ", false);
    printTeks("2. Selesai dan Cetak Invoice", " ", false);
    printTeks("3. Batalkan Invoice", " ", false);
    printBorder('b', false);
    printTeksTanya("Pilihan: ");
    
    pilihan = cin.get();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    pilihan = tolower(pilihan);
    printBorder('b', false);
    switch (pilihan) {
        case '1':
            printTeks("Melanjutkan Invoice...", false);
            printBorder('f', false);
            return '1';
        case '2':
            printTeks("Mencetak Invoice...", false);
            printBorder('f', false);
            return '2';
        case '3':
            printTeks("Membatalkan Invoice...", false);
            printBorder('f', false);
            return '3';
        default:
            printTeks(string(ERR "Pilihan tidak valid. Silakan coba lagi."), false);
            printBorder('f', false);
            return beliLagi();
    }
}

void Kasir::kurangiInventory(vector<Belian> listBeli) {
    for (auto &i : listBeli) {
        auto it = find_if(Inventory::listBarang.begin(), Inventory::listBarang.end(), [i](Item &item) { return item.id == i.id; });
        if (it != Inventory::listBarang.end()) {
            it->jumlah -= i.jumlah;
            if (it->jumlah <= 0) {
                it->jumlah = 0;
            }
        }
    }
    setListBarang(Inventory::listBarang);
}

void Kasir::mainPage() {
    printHeader();
    char beliLagi;
    char pilihan = getPilihan();
    while (pilihan) {
        vector<Item> listDiskon;
        copy_if(Inventory::listBarang.begin(), Inventory::listBarang.end(), back_inserter(listDiskon), [](Item &item) { return item.diskon > 0; });

        switch(pilihan) {
            case '0':
                printBorder('h', false);
                printTeks("Program Kasir Selesai", false);
                printTeks("Terima kasih!", false);
                printBorder('f', false);
                return;
            case '1':
                do {
                    mulaiBeli();
                    displaySementara();
                    beliLagi = Kasir::beliLagi();
                } while (beliLagi == '1');
                if (beliLagi == '2') {
                    cetakBon(true);
                    kurangiInventory(getListBelian());
                    hapusListBelian();
                    saveFileBarang();
                }
                break;
            case '2':
                cetakListBarang();
                break;
            case '3':
                cout << endl;
                printBorder('h', false);
                printTeks("BARANG DENGAN DISKON", false);
                printBorder('f', false);
                if (listDiskon.empty()) {
                    printBorder('h', false);
                    printTeks("Tidak ada barang dengan diskon.", false);
                    printBorder('f', false);
                } else {
                    cetakListDiskon(listDiskon);
                }
                break;
            case '4':
                printBorder('h', false);
                printTeks("Menambahkan barang ke Inventory", false);
                printBorder('f', false);
                tambahBarang();
                break;
            case '5':
                hapusBarang();
                break;
            case '6':
                editBarang();
                break;
        }
        system("pause");
        system("cls");
        printHeader();
        pilihan = getPilihan();
    }
}
