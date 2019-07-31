// SaveGame = entire save file.
// SaveFile = a single save file within a SaveGame.

#ifndef SAVEFILE_H
#define SAVEFILE_H

class SaveGame;

#include <QtCore/QtCore>

enum GameType {
    GAMETYPE_SEASONS = 0,
    GAMETYPE_AGES = 1,
    GAMETYPE_INVALID = 2
};


class SaveFile
{
public:
    const static int SAVEFILE_SIZE = 0x550; // Size of single savefile

public:
    SaveFile(SaveGame *saveGame, int fileIndex);

    GameType getGameType();

    uint8_t getByte(int addr);
    uint16_t getWord(int addr);
    QVector<uint8_t> slice(int start, int end);

    void setByte(int addr, uint8_t val);
    void setWord(int addr, uint16_t val);

    /** Call this AFTER saving all EditableItem's for this file. */
    void save();

    bool hasValidChecksum();

private:
    uint16_t calculateChecksum();

private:
    SaveGame *saveGame;
    int fileIndex;
    uint8_t fileBuffer[SAVEFILE_SIZE];
};

#endif // SAVEFILE_H
