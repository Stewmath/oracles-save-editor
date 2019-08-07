#include "mainwindow.h"
#include "xmlparser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    if (argc > 1)
        w.setSaveFile(argv[1]);
    else
        w.setSaveFile("/home/matthew/programs/ages/LynnaLab/oracles-disasm/Ages_Screwaround.sav");
    w.show();

    return a.exec();
}
