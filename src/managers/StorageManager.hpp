#pragma once
#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

#include <vector>

#include "PinDefinitions.h"

class StorageManager {
   public:
    StorageManager();
    static StorageManager* getInstance();
    bool                   init();
    String                 readFile(const char* path);
    bool                   writeFile(const char* path, const char* content);
    bool                   appendFile(const char* path, const char* content);
    std::vector<String>    listFiles(const char* folderPath);
    bool                   isReady() { return _isMounted; }

   private:
    void                   createFoldersIfMissing();
    static StorageManager* _instance;
    bool                   _isMounted;
    SPIClass*              _spi;
};