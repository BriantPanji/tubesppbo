#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Item {
    int id;
    string nama;
    int unsigned jumlah;
    int unsigned harga;
    float diskon = 0;
};

class TubesKasir {
    protected:
        const int totalWidth = 49;
        vector<Item> listBarang;

        vector<string> listError;
        string formatRupiah(int harga);
        string strTrunc(string, int);
        void strUpper(string &str) { transform(str.begin(), str.end(), str.begin(), ::toupper); }
    public:
        TubesKasir() {}
        ~TubesKasir() {}
};
string TubesKasir::formatRupiah(int harga) {
    string hrgStr = to_string(harga);

    for (int i = hrgStr.length() - 3; i > 0; i-=3 )
        hrgStr.insert(i, ".");
    return "Rp" + hrgStr;
}
string TubesKasir::strTrunc(string str, int len) {
    if (str.length() > len) {
        return str.substr(0, len - 3) + "...";
    } else {
        return str;
    }
}