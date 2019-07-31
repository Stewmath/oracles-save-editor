#include "savefile.h"
#include "savegame.h"
#include "helper.h"

SaveFile::SaveFile(SaveGame *saveGame, int fileIndex)
{
    this->saveGame = saveGame;
    this->fileIndex = fileIndex;

    memcpy(this->fileBuffer, saveGame->getRawData(16 + SAVEFILE_SIZE * fileIndex), SAVEFILE_SIZE);
}

GameType SaveFile::getGameType() {
    QString str = Helper::vectorToString(slice(2, 10));

    QTextStream(stdout) << "File string: " << str << '\n';

    if (str == "Z11216-0")
        return GAMETYPE_SEASONS;
    else if (str == "Z21216-0")
        return GAMETYPE_AGES;
    else
        return GAMETYPE_INVALID;
}

uint8_t SaveFile::getByte(int addr) {
    return fileBuffer[addr];
}

uint16_t SaveFile::getWord(int addr) {
    return fileBuffer[addr] | (fileBuffer[addr+1]<<8);
}

QVector<uint8_t> SaveFile::slice(int start, int end) {
    QVector<uint8_t> ret;

    for (int i=start;i<end;i++)
        ret.push_back(fileBuffer[i]);

    return ret;
}

void SaveFile::setByte(int addr, uint8_t val) {
    fileBuffer[addr] = val;
}

void SaveFile::setWord(int addr, uint16_t val) {
    fileBuffer[addr] = val&0xff;
    fileBuffer[addr+1] = val>>8;
}

void SaveFile::save() {
    // Fix checksum
    uint16_t checksum = calculateChecksum();
    setWord(0, checksum);

    memcpy(saveGame->getRawData(16 + SAVEFILE_SIZE * fileIndex), fileBuffer, SAVEFILE_SIZE);
    saveGame->save();
}

bool SaveFile::hasValidChecksum() {
    uint16_t calc = calculateChecksum();

    return calc == getWord(0);
}


// Private methods

uint16_t SaveFile::calculateChecksum() {
    uint16_t checksum = 0;

    for (int i=2; i<SAVEFILE_SIZE; i+=2) {
        checksum += getWord(i);
    }

    return checksum;
}
