#pragma once

#ifndef KASIR_HPP
#define KASIR_HPP

#include <iostream>
#include <vector>
#include "Inventory.hpp"
#include "Invoice.hpp"

class Kasir : public Inventory, public Invoice {
private:
    void printHeader();
    char getPilihan();

public:
    Kasir();
    ~Kasir();

    void mainPage();
    char beliLagi();

    void tambahBarang();
    void hapusBarang();
    void editBarang();
    void tampilkanBarang();
    void tampilkanBarangDiskon();
    void kurangiInventory(std::vector<Belian>);
};

#endif // KASIR_HPP
