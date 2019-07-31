#ifndef HELPER_H
#define HELPER_H

#include <QtCore/QtCore>

namespace Helper {

QString vectorToString(QVector<uint8_t> v);
int parseInt(QString s);
QString toHex(int i);
int bcdToInt(int bcd, int maxVal, bool *error = nullptr);
int intToBcd(int i);

}

#endif // HELPER_H
