#include "PanKasir.hpp"

PanKasir::PanKasir() {}

PanKasir::~PanKasir() {
    if (listError.empty()) return;
    cerr << endl << "Error & Warn: " << endl;
    for (auto &err : listError) {
        cerr << " - " << err << endl;
    }
}

void PanKasir::strUpper(string &str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
}

string PanKasir::toUpperCopy(const string &str) {
    string upperStr = str;
    transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
}

string PanKasir::formatRupiah(int harga) {
    string hrgStr = to_string(harga);

    for (int i = hrgStr.length() - 3; i > 0; i-=3 )
        hrgStr.insert(i, ".");
    return "Rp" + hrgStr;
}
string PanKasir::strTrunc(string str, int len) {
    if (str.length() > len) {
        return str.substr(0, len - 3) + "...";
    } else {
        return str;
    }
}