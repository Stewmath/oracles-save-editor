#include <QLayout>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QScrollArea>
#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "editableitem.h"
#include "editableitemwindow.h"
#include "helper.h"
#include "xmlparser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QAction *saveAction = this->ui->mainToolBar->addAction(QIcon::fromTheme("document-save"), "&Save");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete saveGame;
}

void MainWindow::setSaveFile(QString filename) {
    saveGame = new SaveGame(filename);
    saveFile = saveGame->getFileIndex(0);

    assert(saveFile->getGameType() != GAMETYPE_INVALID); // TODO: fail gracefully
    // TODO: offer to fix checksum
    QTextStream(stdout) << saveFile->hasValidChecksum() << '\n';

    setupLayout();
}

void MainWindow::setupLayout() {
    assert(saveFile != NULL);

    XmlParser parser(":/data/ages.xml");

    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);

    for (const EditableItemSection &section : parser.getEditableItems(saveFile)) {
        EditableItemWindow *window = new EditableItemWindow(this, saveFile, section.items);
        editableItemWindows.push_back(window);

        QScrollArea *scrollArea = new QScrollArea();
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidget(window);

        ui->tabWidget->addTab(scrollArea, section.name);
    }
}

void MainWindow::save() {
    QTextStream(stdout) << "Save\n";

    for (EditableItemWindow *window : editableItemWindows)
        window->save();

    saveFile->save();
    saveGame->save();
}
