#include "editableitem.h"
#include "helper.h"
#include <QLayout>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>

const QVector<QString> EditableItem::DataTypeStrings = {
    "Byte",
    "Word",
    "Byte BCD",
    "Word BCD",
    "String",
    "Bit",
    "Bits",
};

EditableItem::EditableItem(SaveFile *file) {
    this->saveFile = file;
}

int EditableItem::getIntValue() {
    int addr = address - 0xc5b0;
    int val;

    switch (dataType) {
    case DATATYPE_BYTE:
        return saveFile->getByte(addr);
    case DATATYPE_WORD:
        return saveFile->getWord(addr);
    case DATATYPE_BYTE_BCD:
        return Helper::bcdToInt(saveFile->getByte(addr), getMaximum()); // TODO: check for format error
    case DATATYPE_WORD_BCD:
        return Helper::bcdToInt(saveFile->getWord(addr), getMaximum()); // TODO: check for format error
    case DATATYPE_BIT:
    case DATATYPE_BITS:
        assert(firstBit != -1 && lastBit != -1);
        return (saveFile->getByte(addr) & getBitmask()) >> firstBit;
    default:
        assert(false);
    }
}

int EditableItem::getMaximum() {
    if (maxVal != -1)
        return maxVal;
    switch (dataType) {
    case DATATYPE_BYTE:
        return 0xff;
    case DATATYPE_WORD:
        return 0xffff;
    case DATATYPE_BYTE_BCD:
        return 99;
    case DATATYPE_WORD_BCD:
        return 9999;
    case DATATYPE_BIT:
        return 1;
    case DATATYPE_BITS:
        return (1<<(lastBit-firstBit+1))-1;
    default:
        assert(false);
    }
}

void EditableItem::save() {
    int addr = convertAddress(address);

    switch (dataType) {
    case DATATYPE_BYTE: {
        uint8_t val = (uint8_t)static_cast<QSpinBox*>(linkedWidget)->value();
        saveFile->setByte(addr, val);
        break;
    }
    case DATATYPE_WORD: {
        uint16_t val = (uint16_t)reinterpret_cast<QSpinBox*>(linkedWidget)->value();
        saveFile->setWord(addr, val);
        break;
    }
    case DATATYPE_BYTE_BCD: {
        uint8_t val = (uint8_t)static_cast<QSpinBox*>(linkedWidget)->value();
        saveFile->setByte(addr, Helper::intToBcd(val));
        break;
    }
    case DATATYPE_WORD_BCD: {
        uint16_t val = (uint16_t)static_cast<QSpinBox*>(linkedWidget)->value();
        saveFile->setWord(addr, Helper::intToBcd(val));
        break;
    }
    case DATATYPE_BIT: {
        uint8_t val = static_cast<QCheckBox*>(linkedWidget)->checkState() == Qt::Checked ? 1 : 0;
        uint8_t oldVal = saveFile->getByte(addr) & ~getBitmask();
        saveFile->setByte(addr, (val<<firstBit) | oldVal);
        break;
    }
    case DATATYPE_BITS: {
        uint8_t val = (uint8_t)static_cast<QSpinBox*>(linkedWidget)->value();
        uint8_t oldVal = saveFile->getByte(addr) & ~getBitmask();
        saveFile->setByte(addr, (val<<firstBit) | oldVal);
        break;
    }
    case DATATYPE_STRING:
        // TODO
        break;
    default:
        assert(false);
    }
}


// Private methods

int EditableItem::convertAddress(int addr) {
    return addr - 0xc5b0;
}

uint8_t EditableItem::getBitmask() {
    uint8_t val = 0;
    for (int i=firstBit;i<=lastBit;i++)
        val |= (1<<i);
    return val;
}
