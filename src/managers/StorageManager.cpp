#include "StorageManager.hpp"

StorageManager* StorageManager::_instance = nullptr;

StorageManager::StorageManager() : _isMounted(false), _spi(nullptr) {}

StorageManager* StorageManager::getInstance() {
    if (!_instance) _instance = new StorageManager();
    return _instance;
}

bool StorageManager::init() {
    uint8_t cardType = 0;

#ifdef PIN_SD_CD
    pinMode(PIN_SD_CD, INPUT_PULLUP);
    if (digitalRead(PIN_SD_CD) == HIGH) {
        Serial.println("[Storage] SD Card not present!");
        _isMounted = false;
        return false;
    }
#endif
    _spi = new SPIClass(HSPI);
    _spi->begin(PIN_SD_SCK, PIN_SD_MISO, PIN_SD_MOSI, PIN_SD_CS);
    if (!SD.begin(PIN_SD_CS, *_spi)) {
        Serial.println("[Storage] SD Mount Failed!");
        _isMounted = false;
        return false;
    }
    cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        Serial.println("[Storage] No SD card attached");
        _isMounted = false;
        return false;
    }
    Serial.printf("[Storage] SD Mounted. Size: %lluMB\n", SD.cardSize() / (1024 * 1024));
    _isMounted = true;
    createFoldersIfMissing();
    return true;
}

void StorageManager::createFoldersIfMissing() {
    const char* folders[] = {"/Flipper-1",     "/Flipper-1/settings", "/Flipper-1/subghz",
                             "/Flipper-1/nfc", "/Flipper-1/badusb",   "/Flipper-1/ir/remote"};

    for (const char* path : folders) {
        if (!SD.exists(path)) {
            Serial.printf("[Storage] Creating dir: %s\n", path);
            SD.mkdir(path);
        }
    }
}

std::vector<String> StorageManager::listFiles(const char* folderPath) {
    std::vector<String> fileList = {};
    File                root = File();
    File                file = File();
    String              name = "";

    if (!_isMounted) return fileList;
    root = SD.open(folderPath);
    if (!root || !root.isDirectory()) {
        Serial.println("[Storage] Failed to open directory");
        return fileList;
    }
    file = root.openNextFile();
    while (file) {
        if (!file.isDirectory()) {
            name = String(file.name());
            if (name.startsWith("/")) {
                name = name.substring(1);
            }
            fileList.push_back(name);
        }
        file = root.openNextFile();
    }
    return fileList;
}

String StorageManager::readFile(const char* path) {
    File   file = File();
    String payload = "";

    if (!_isMounted) return "";
    file = SD.open(path);
    if (!file) {
        Serial.println("[Storage] Failed to open file for reading");
        return "";
    }

    payload = "";
    while (file.available()) {
        payload += (char)file.read();
    }
    file.close();
    return payload;
}

bool StorageManager::writeFile(const char* path, const char* content) {
    File file = File();

    if (!_isMounted) return false;
    file = SD.open(path, FILE_WRITE);
    if (!file) {
        Serial.println("[Storage] Failed to open file for writing");
        return false;
    }

    if (file.print(content)) {
        Serial.println("[Storage] File written");
        file.close();
        return true;
    } else {
        Serial.println("[Storage] Write failed");
        file.close();
        return false;
    }
}

bool StorageManager::appendFile(const char* path, const char* content) {
    File file = File();

    if (!_isMounted) return false;
    file = SD.open(path, FILE_APPEND);
    if (!file) return false;

    bool res = file.print(content);
    file.close();
    return res;
}