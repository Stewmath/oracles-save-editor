#include "helper.h"

namespace Helper {

QString vectorToString(QVector<uint8_t> v) {
    QString s;
    for (uint8_t c : v) {
        s += static_cast<char>(c);
    }

    return s;
}

// Parse an integer, accounting for hexadecimal
int parseInt(QString s) {
    if (s.size() >= 2 && s[0] == '0' && s[1] == 'x') {
        return s.right(s.size()-2).toInt(nullptr, 16);
    }
    else {
        return s.toInt();
    }
}

QString toHex(int i) {
    QString s;
    if (i == 0)
        s = "0";

    while (i != 0) {
        int d = i%16;
        if (d<10)
            s += (char)('0' + d);
        else
            s += (char)('A' + (d-10));
        i/=16;
    }

    s += "x0";
    std::reverse(s.begin(), s.end());
    return s;
}

int bcdToInt(int bcd, int maxVal, bool *error) {
    int ret = 0;
    int mult = 1;

    if (error != nullptr) {
        *error = false;
        if (bcd > maxVal)
            *error = true;
    }

    while (bcd != 0) {
        int digit = bcd % 16;

        if (digit >= 10 && error != nullptr)
            *error = true;

        ret += mult * digit;
        bcd /= 16;
        mult *= 10;
    }

    return ret;
}

int intToBcd(int i) {
    int ret = 0;
    int mult = 1;

    while (i != 0) {
        ret += mult * (i % 10);
        i /= 10;
        mult *= 16;
    }

    return ret;
}
}
