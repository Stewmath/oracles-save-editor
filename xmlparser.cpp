#include "xmlparser.h"
#include "editableitem.h"
#include "helper.h"

XmlParser::XmlParser(QString filename)
{
    this->filename = filename;
}

QVector<EditableItem> XmlParser::getEditableItems(SaveFile *saveFile) {
    QFile file(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QXmlStreamReader xml(file.readAll());
    QVector<EditableItem> items;

    QString currentElement;
    EditableItem currentItem(saveFile);
    currentItem.name = "INVALID";

    while (!xml.atEnd()) {
        xml.readNext();
        QString type = xml.tokenString();
        QString name = "", text = "";
        if (xml.name().string() != nullptr)
                name = QString(xml.name().unicode(), xml.name().size());
        if (xml.text().string() != nullptr)
                text = QString(xml.text().unicode(), xml.text().size());

        QTextStream(stdout) << type << ", " << name << ", " << text << endl;

        if (type == "StartElement") {
            currentElement = name;
            if (currentElement == "item")
                currentItem.name = "<UNNAMED>";
        }
        else if (type == "Characters") {
            if (currentElement == "name") {
                assert(currentItem.name != "INVALID");
                currentItem.name = text;
            }
            else if (currentElement == "addr") {
                currentItem.address = Helper::parseInt(text);
            }
            else if (currentElement == "agesaddr" && saveFile->getGameType() == GAMETYPE_AGES) {
                currentItem.address = Helper::parseInt(text);
            }
            else if (currentElement == "ssnsaddr" && saveFile->getGameType() == GAMETYPE_SEASONS) {
                currentItem.address = Helper::parseInt(text);
            }
            else if (currentElement == "datatype") {
                currentItem.dataType = EditableItem::DataType(EditableItem::DataTypeStrings.indexOf(text));
            }
            else if (currentElement == "max") {
                currentItem.maxVal = text.toInt();
            }
            else if (currentElement == "firstbit") {
                currentItem.firstBit = text.toInt();
            }
            else if (currentElement == "lastbit") {
                currentItem.lastBit = text.toInt();
            }
            else if (currentElement == "hint") {
                currentItem.hint = text;
            }
        }
        else if (type == "EndElement") {
            if (name == "item" && currentItem.address != -1) {
                assert(currentItem.name != "INVALID");
                items.push_back(currentItem);
                currentItem = EditableItem(saveFile);
                currentItem.name = "INVALID";
            }
            currentElement = "";
        }
    }
    if (xml.hasError()) {
        QTextStream(stdout) << xml.errorString() << endl;
    }

    file.close();

    return items;
}
