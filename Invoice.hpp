// Invoice.hpp
#pragma once

#ifndef INVOICE_HPP
#define INVOICE_HPP

#include "PanKasir.hpp"
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <filesystem>

struct Belian {
    int id;
    unsigned int jumlah;
};

class Invoice : public PanKasir {
private:
    const std::string dirCetakan = "bon/";
    int totalBayar, pajak;
    std::string namaToko, alamatToko, noTelpToko, namaKasir;

    std::ofstream fileCetakBon;
    std::string namaFile;
    std::vector<Belian> listBelian;
    std::vector<Item> listBarang;
    
    std::tm *ltm;
    std::string bulanNow;
    
    std::string getWaktu();
    std::string randomId();
    
protected:
    void printBorder(char, bool);
    void printTeksTanya(std::string);

    void printTeks(std::string, bool);
    void printTeks(std::string, std::string, bool);
    void printTeksHarga(std::string, std::string, std::string, bool);
    void printTeksSisi(std::string, char, bool);
    void printHeader(bool);
    void printFooter(bool);

    Belian printTambahBelian();

    vector<Belian> getListBelian() { return listBelian; }
    void hapusListBelian();



public:
    Invoice();
    ~Invoice();

    void setListBarang(std::vector<Item> barangs);
    void setNamaToko(std::string str);
    void setPajak(int pajak);
    void setAlamatToko(std::string str);
    void setNoTelpToko(std::string str);
    void setNamaKasir(std::string str);

    void tambahBelian(int, unsigned int);
    void tambahBelian(Belian);
    void editBelian(int, unsigned int);
    void hapusBelian(int);

    int mulaiBeli();
    void displaySementara();
    // void tambahBelian(std::string, unsigned int, unsigned int, float);
    void printIsi(bool);
    void cetakBon(bool);

    int getTotalBayar();
};

#endif