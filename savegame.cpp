#include "savegame.h"
#include "helper.h"

SaveGame::SaveGame(QString f)
{
    this->filename = f;
    reloadFile();
}

SaveGame::~SaveGame() {
    for (int i=0;i<3;i++)
        delete saveFiles[i];
}

void SaveGame::reloadFile() {
    QTextStream(stdout) << "Opening " << filename << '\n';
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    file.read(reinterpret_cast<char*>(rawBuffer), 0x2000);

    for (int i=0;i<3;i++) {
        saveFiles[i] = new SaveFile(this, i);
    }

    file.close();
}

SaveFile *SaveGame::getFileIndex(int index) {
    assert(index >= 0 && index <= 2);

    return saveFiles[index];
}

uint8_t *SaveGame::getRawData(int addr) {
    return rawBuffer + addr;
}

void SaveGame::save() {
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    file.write(reinterpret_cast<char*>(rawBuffer), SAVEGAME_SIZE);
    file.close();
}
