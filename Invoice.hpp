// Invoice.hpp
#pragma once
#include "PanKasir.hpp"
#include "Inventory.hpp"
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <filesystem>

class Invoice : public PanKasir {
private:
    const std::string dirCetakan = "bon/";
    int totalBayar, pajak;
    std::string namaToko, alamatToko, noTelpToko, namaKasir;

    std::ofstream fileCetakBon;
    std::string namaFile;
    std::vector<ItemNoId> listBelian;

    std::tm *ltm;
    std::string bulanNow;

    std::string getWaktu();
    std::string randomId();
    void printTeks(std::string);
    void printTeks(std::string, std::string);
    void printTeksHarga(std::string, std::string, std::string);
    void printTeksSisi(std::string, char side = 'l');
    void printHeader();
    void printFooter();

public:
    Invoice();
    ~Invoice();

    void setListBarang(std::vector<Item> barangs);
    void setNamaToko(std::string str);
    void setPajak(int pajak);
    void setAlamatToko(std::string str);
    void setNoTelpToko(std::string str);
    void setNamaKasir(std::string str);

    void tambahBarang(ItemNoId);
    void tambahBarang(std::string, unsigned int, unsigned int, float);
    void printIsi();
    void cetakBon();

    int getTotalBayar();
    void debug();
};
