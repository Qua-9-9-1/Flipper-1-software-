#pragma once
#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

#include <vector>

#include "PinDefinitions.h"
#include "SubGhzFile.hpp"
#include "IrFile.hpp"
#include "NfcFile.hpp"

class StorageManager {
   public:
    StorageManager();
    static StorageManager* getInstance();
    bool                   init();

    String                 readFile(const char* path);
    bool                   writeFile(const char* path, const char* content);
    bool                   appendFile(const char* path, const char* content);
    bool                   renameFile(const char* oldPath, const char* newPath);
    std::vector<String>    listFiles(const char* folderPath);
    bool                   isReady() { return _isMounted; }

    std::vector<String>    listSubGhzFiles();
    std::vector<String>    listIrFiles();
    std::vector<String>    listNfcFiles();
    std::vector<String>    listBadUsbFiles();

    bool loadSubGhz(const char* filename, SubGhzFile& out);
    bool saveSubGhz(const char* filename, const SubGhzFile& in);

    bool loadIr(const char* filename, IrFile& out);
    bool saveIr(const char* filename, const IrFile& in);

    bool loadNfc(const char* filename, NfcFile& out);
    bool saveNfc(const char* filename, const NfcFile& in);

   private:
    void                   createFoldersIfMissing();
    void                   createDemoFilesIfMissing();
    static StorageManager* _instance;
    bool                   _isMounted;
    SPIClass*              _spi;
};