// SaveGame = entire save file.
// SaveFile = a single save file within a SaveGame.

#ifndef SAVEGAME_H
#define SAVEGAME_H

#include "savefile.h"
#include <QtCore/QtCore>

class SaveGame
{
public:
    const static int SAVEGAME_SIZE = 0x2000; // Size of entire file (including all 3 saves)

public:
    SaveGame(QString filename);
    ~SaveGame();

    void reloadFile();
    SaveFile *getFileIndex(int index);

    uint8_t *getRawData(int addr);

    void save();

private:
    QString filename;
    uint8_t rawBuffer[SAVEGAME_SIZE];
    SaveFile *saveFiles[3];
};

#endif // SAVEGAME_H
