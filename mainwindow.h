#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "savegame.h"
#include <QMainWindow>

class EditableItemWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setSaveFile(QString filename);
    void setupLayout();

private slots:
    void save();

private:
    Ui::MainWindow *ui;

    SaveGame *saveGame;
    SaveFile *saveFile;
    QVector<EditableItemWindow*> editableItemWindows;
};

#endif // MAINWINDOW_H
