#include "Inventory.hpp"


Inventory::Inventory() {
    if (!fs::exists(dirCetakan)) {
        fs::create_directories(dirCetakan);
    }
    loadFileBarang();
}
Inventory::~Inventory() {
    saveFileBarang();
    fileCetakBarang.close();
}

void Inventory::loadFileBarang() {
    fileBarang.open(dirCetakan + namaFile);
    if (!fileBarang.is_open()) {
        cerr << ERR "ERR (Inventory::loadFileBarang): Failed to open file.\n";
        return;
    }
    Item brg;
    string line;
    listBarang.clear();
    
    while (getline(fileBarang, line)) {
        stringstream ss(line);
        string field;

        getline(ss, field, '|'); //id
        brg.id = stoi(field);

        getline(ss, brg.nama, '|'); //nama

        getline(ss, field, '|'); //jumlah
        brg.jumlah = (unsigned int) stoi(field);
        
        getline(ss, field, '|'); //harga
        brg.harga = (unsigned int) stoi(field);

        if (getline(ss, field, '|')) { //diskon (opsional)
            brg.diskon = stof(field);
        } else {
            brg.diskon = 0.0f;
        }

        listBarang.push_back(brg);
    }

    fileBarang.close();
}
void Inventory::saveFileBarang() {
    fileCetakBarang.open(dirCetakan + namaFile);
    if (!fileCetakBarang.is_open()) {
        cerr << ERR "ERR (Inventory::saveFileBarang): Failed to open file.\n";
        return;
    }
    for (auto i = listBarang.begin(); i != listBarang.end(); ++i) {
        fileCetakBarang << i->id << "|" << i->nama << "|" << i->jumlah << "|" << i->harga << "|" << i->diskon << endl;
    }
    fileCetakBarang.close();
}

void Inventory::tambahBarang(ItemNoId brg) {
    int lastId = listBarang.empty() ? 0 : listBarang.back().id;
    auto it = find_if(listBarang.begin(), listBarang.end(), [this, brg](Item &br) { return toUpperCopy(br.nama) == toUpperCopy(brg.nama); });
    if (it != listBarang.end())
        editBarang(it->id, brg);
    else
        listBarang.push_back({++lastId, brg.nama, brg.jumlah, brg.harga, brg.diskon});
}
void Inventory::tambahBarang(string namaBrg, unsigned int jlhBrg, unsigned int hargaBrg, float diskon) {
    tambahBarang({namaBrg, jlhBrg, hargaBrg, diskon});
}

bool Inventory::hapusBarang(int id) {
    auto it = find_if(listBarang.begin(), listBarang.end(), [id](Item &brg) { return brg.id == id; });
    if (it != listBarang.end()) {
        listBarang.erase(it);
        return true;
    } else {
        cerr << ERR "ERR (Inventory::hapusBarang): Item not found.\n";
        return false;
    }
}
// void Inventory::hapusBarang() {
//     string idStr;
//     cout << ASK "Masukkan ID barang yang ingin dihapus: ";
//     getline(cin, idStr);
//     int id = stoi(idStr);
//     hapusBarang(id);
// }

void Inventory::editBarang(int id, ItemNoId brg) {
    auto it = find_if(listBarang.begin(), listBarang.end(), [id](Item &item) { return item.id == id; });
    if (it != listBarang.end()) {
        if (brg.nama.empty()) brg.nama = it->nama;
        if (brg.jumlah == 0) brg.jumlah = it->jumlah;
        if (brg.harga == 0) brg.harga = it->harga;
        if (brg.diskon == -1) brg.diskon = it->diskon;
        *it = {id, brg.nama, brg.jumlah, brg.harga, brg.diskon};
    } else {
        cerr << ERR "ERR (Inventory::editBarang): Item not found.\n";
    }
}
void Inventory::editBarang(int id, string namaBrg, unsigned int jlhBrg, unsigned int hargaBrg, float diskon) {
    editBarang(id, {namaBrg, jlhBrg, hargaBrg, diskon});
}
// void Inventory::editBarang() {
//     string idStr;
//     cout << ASK "Masukkan ID barang yang ingin diedit: ";
//     getline(cin, idStr);
//     int id = stoi(idStr);

//     auto it = find_if(listBarang.begin(), listBarang.end(), [id](Item &item) { return item.id == id; });
//     if (it == listBarang.end()) {
//         cerr << ERR "ERR (Inventory::editBarang): Item not found.\n";
//         return;
//     }

//     ItemNoId brg;
//     cout << ASK "Masukkan nama barang baru (kosongkan jika tidak ingin mengubah): ";
//     getline(cin, brg.nama);
//     cout << ASK "Masukkan jumlah barang baru (0 jika tidak ingin mengubah): ";
//     cin >> brg.jumlah;
//     cout << ASK "Masukkan harga barang baru (0 jika tidak ingin mengubah): ";
//     cin >> brg.harga;
//     cout << ASK "Masukkan diskon barang baru (0 jika tidak ingin mengubah): ";
//     cin >> brg.diskon;

//     editBarang(id, brg);
// }

void Inventory::cetakListBarang() {
    cout << "╔═══════════════════════════════════════════════════╗\n";
    cout << "║ ID  NAMA BARANG                 STOK        HARGA ║\n";
    cout << "╠═══════════════════════════════════════════════════╣\n";
    for (const auto& item : listBarang) {
        cout << "║ " << left << setw(3) << item.id
             << left << setw(25) << item.nama
             << right << setw(8) << item.jumlah
             << right << setw(13) << item.harga
             << " ║\n";
    }
    cout << "╚═══════════════════════════════════════════════════╝\n";
}
void Inventory::cetakListBarang(vector<Item> barangs) {
    cout << "╔═══════════════════════════════════════════════════╗\n";
    cout << "║ ID  NAMA BARANG                 STOK        HARGA ║\n";
    cout << "╠═══════════════════════════════════════════════════╣\n";
    for (const auto& item : barangs) {
        cout << "║ " << left << setw(3) << item.id
             << left << setw(25) << item.nama
             << right << setw(8) << item.jumlah
             << right << setw(13) << item.harga
             << " ║\n";
    }
    cout << "╚═══════════════════════════════════════════════════╝\n";
}
void Inventory::cetakListDiskon(vector<Item> barangs) {
    cout << "╔═══════════════════════════════════════════════════╗\n";
    cout << "║ ID  NAMA BARANG               DISKON        HARGA ║\n";
    cout << "╠═══════════════════════════════════════════════════╣\n";
    for (const auto& item : barangs) {
        cout << "║ " << left << setw(3) << item.id
             << left << setw(25) << item.nama
             << right << setw(8) << to_string((int) (item.diskon*100)) + "%"
             << right << setw(13) << item.harga
             << " ║\n";
    }
    cout << "╚═══════════════════════════════════════════════════╝\n";
}

void Inventory::tampilkanBarang(int id) {
    auto it = find_if(listBarang.begin(), listBarang.end(), [id](Item &item) { return item.id == id; });
    if (it != listBarang.end()) {
        cout << SUCC << "Barang dengan ID " << id << " ditemukan.\n";
        cout << RES << "ID: " << it->id << ", Nama: " << it->nama
             << ", Stok: " << it->jumlah << ", Harga: " << it->harga
             << ", Diskon: " << (it->diskon * 100) << '%' << endl;
    } else {
        cerr << ERR "ERR (Inventory::tampilkanBarang): Item not found.\n";
    }
}
// void Inventory::tampilkanBarang() {
//     string idStr;
//     cout << ASK "Masukkan ID barang yang ingin ditampilkan: ";
//     getline(cin, idStr);
//     int id = stoi(idStr);
//     tampilkanBarang(id);
// }
