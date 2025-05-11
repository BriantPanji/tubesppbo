#pragma once

#ifndef PANKASIR_HPP
#define PANKASIR_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#define INFO "|i| "
#define ERR "|x| "
#define WARN "|!| "
#define RES "|~| "
#define SUCC "|✓| "
#define ASK "|?| "

struct Item
{
    int id;
    string nama;
    unsigned int jumlah;
    unsigned int harga;
    float diskon = 0;
};
struct ItemNoId
{
    std::string nama;
    unsigned int jumlah;
    unsigned int harga;
    float diskon = 0;
};


class PanKasir
{
protected:
    const int totalWidth = 49;
    vector<Item> listBarang;

    vector<string> listError;
    string formatRupiah(int harga);
    string strTrunc(string, int);
    void strUpper(string &str);
    string toUpperCopy(const string &str);

public:
    PanKasir();
    ~PanKasir();
};

#endif