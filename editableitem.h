#ifndef EDITABLEITEM_H
#define EDITABLEITEM_H

#include <QtCore>
#include "savefile.h"


class EditableItem
{
public:
    enum DataType {
        DATATYPE_INVALID = -1,
        DATATYPE_BYTE = 0,
        DATATYPE_WORD,
        DATATYPE_BYTE_BCD,
        DATATYPE_WORD_BCD,
        DATATYPE_STRING,
        DATATYPE_BIT,
        DATATYPE_BITS,
    };

    const static QVector<QString> DataTypeStrings;

public:
    EditableItem(SaveFile *file);

    int getIntValue();
    int getMaximum();

    void save();

private:
    int convertAddress(int addr);
    uint8_t getBitmask(); // For DATATYPE_BITS only

public:
    QString name = "";
    int address = -1;
    DataType dataType = DATATYPE_INVALID;
    int maxVal = -1;
    int firstBit = -1, lastBit = -1; // For DATATYPE_BITS only
    QString hint;

    QWidget *linkedWidget = nullptr;

private:
    SaveFile *saveFile;
};

struct EditableItemSection {
    QString name;
    QVector<EditableItem> items;
};

#endif // EDITABLEITEM_H
