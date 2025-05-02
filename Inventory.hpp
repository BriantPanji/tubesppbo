#pragma once

#include "PanKasir.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>

namespace fs = filesystem;
using namespace std;

class Inventory : public PanKasir
{
private:
    string namaFile = "barang.txt";
    string dirCetakan = "barang/";
    ofstream fileCetakBarang;
    ifstream fileBarang;
    void loadFileBarang();
    void saveFileBarang();

public:
    Inventory();
    ~Inventory();

    void tambahBarang(ItemNoId);
    void tambahBarang(string, unsigned int, unsigned int, float);
    void hapusBarang(int);
    void hapusBarang();
    void editBarang(int, ItemNoId);
    void editBarang(int, string, unsigned int, unsigned int, float);
    void editBarang();
    void cetakListBarang();
    void tampilkanBarang(int);
    void tampilkanBarang();
    vector<Item> getListBarang() { return listBarang; }
};
