#include "xmlparser.h"
#include "editableitem.h"
#include "helper.h"

XmlParser::XmlParser(QString filename)
{
    this->filename = filename;
}

QVector<EditableItemSection> XmlParser::getEditableItems(SaveFile *saveFile) {
    QFile file(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QXmlStreamReader xml(file.readAll());

    QVector<EditableItemSection> sections;

    QString currentElement;
    EditableItemSection currentSection = EditableItemSection();
    EditableItem currentItem(saveFile);

    while (!xml.atEnd()) {
        xml.readNext();
        QString type = xml.tokenString();
        QString name = xml.name().toString();
        QString text = xml.text().toString();

        QTextStream(stdout) << type << ", " << name << ", " << text << endl;

        if (type == "StartElement") {
            currentElement = name;
            if (currentElement == "item")
                currentItem.name = "<UNNAMED>";
            else if (currentElement == "section") {
                currentSection.name = xml.attributes().value("name").toString();
            }
        }
        else if (type == "Characters") {
            if (currentElement == "name") {
                assert(currentItem.name != "");
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
                currentItem.maxVal = Helper::parseInt(text);
            }
            else if (currentElement == "firstbit") {
                currentItem.firstBit = Helper::parseInt(text);

                while (currentItem.firstBit >= 8) {
                    currentItem.firstBit -= 8;
                    currentItem.address++;
                }

                if (currentItem.lastBit == -1)
                    currentItem.lastBit = currentItem.firstBit;
            }
            else if (currentElement == "lastbit") {
                currentItem.lastBit = Helper::parseInt(text);
            }
            else if (currentElement == "hint") {
                currentItem.hint = text;
            }
        }
        else if (type == "EndElement") {
            if (name == "item" && currentItem.address != -1) {
                assert(currentItem.name != "");
                currentSection.items.push_back(currentItem);
                currentItem = EditableItem(saveFile);
            }
            else if (name == "section") {
                sections.push_back(currentSection);
                currentSection = EditableItemSection();
            }
            currentElement = "";
        }
    }
    if (xml.hasError()) {
        QTextStream(stdout) << xml.errorString() << endl;
    }

    file.close();

    return sections;
}
