#include "editableitemwindow.h"
#include "helper.h"

#include <QLayout>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>

EditableItemWindow::EditableItemWindow(QWidget *parent, SaveFile *saveFile, QVector<EditableItem> it) : QWidget(parent)
{
    this->saveFile = saveFile;
    this->items = it;

    QWidget *window = new QWidget(this);

    QFormLayout *layout = new QFormLayout(window);
    window->setLayout(layout);

    for (auto &item : items) {
        QLabel *label = new QLabel(item.name);
        QWidget *field;

        switch (item.dataType) {
        case EditableItem::DATATYPE_BYTE:
        case EditableItem::DATATYPE_WORD:
        case EditableItem::DATATYPE_BYTE_BCD:
        case EditableItem::DATATYPE_WORD_BCD:
        case EditableItem::DATATYPE_BITS: {
            QSpinBox *spinBox = new QSpinBox(window);
            spinBox->setRange(0, item.getMaximum());
            spinBox->setValue(item.getIntValue());
            field = spinBox;
            break;
        }
        case EditableItem::DATATYPE_BOOL: {
            QCheckBox *checkBox = new QCheckBox(window);
            checkBox->setCheckState(item.getIntValue() != 0 ? Qt::Checked : Qt::Unchecked);
            field = checkBox;
            break;
        }
        case EditableItem::DATATYPE_STRING: // TODO
        default:
            field = new QLabel("Unknown data type");
            break;
        }

        label->setToolTip(item.hint);
        field->setToolTip(item.hint);

        item.linkedWidget = field;
        layout->addRow(label, field);

        // Debugging: print data
        QTextStream(stdout) << "Item name: " << item.name << '\n';
        QTextStream(stdout) << "\tAddress: " << Helper::toHex(item.address) << '\n';
        QTextStream(stdout) << "\tData type: " << EditableItem::DataTypeStrings[item.dataType] << '\n';
    }

    this->setLayout(new QHBoxLayout(this));
    this->layout()->addWidget(window);
}


void EditableItemWindow::save() {
    for (EditableItem &item : items) {
        item.save();
    }
}
