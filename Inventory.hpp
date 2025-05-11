#pragma once

#ifndef INVENTORY_HPP
#define INVENTORY_HPP

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

    
    public:
    Inventory();
    ~Inventory();
    
    void loadFileBarang();
    void saveFileBarang();

    void tambahBarang(ItemNoId);
    void tambahBarang(string, unsigned int, unsigned int, float);
    bool hapusBarang(int);
    // void hapusBarang();
    void editBarang(int, ItemNoId);
    void editBarang(int, string, unsigned int, unsigned int, float);
    // void editBarang();
    void cetakListBarang();
    void cetakListBarang(vector<Item>);
    void cetakListDiskon(vector<Item>);
    void tampilkanBarang(int);
    // void tampilkanBarang();
    vector<Item> getListBarang() { loadFileBarang(); return listBarang; }
};

#endif