#ifndef EDITABLEITEMWINDOW_H
#define EDITABLEITEMWINDOW_H

#include "editableitem.h"
#include <QMainWindow>


class EditableItemWindow : public QWidget
{
public:
    EditableItemWindow(QWidget *parent, SaveFile *saveFile, QVector<EditableItem> items);

    void save();

private:
    SaveFile *saveFile;
    QVector<EditableItem> items;
};

#endif // EDITABLEITEMWINDOW_H
