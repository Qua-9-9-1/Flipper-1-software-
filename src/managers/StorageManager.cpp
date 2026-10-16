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
        Serial.println("[Storage] SD Card not present");
        _isMounted = false;
        return false;
    }
#endif
    _spi = new SPIClass(HSPI);
    _spi->begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SD_CS);
    if (!SD.begin(PIN_SD_CS, *_spi)) {
        Serial.println("[Storage] SD mount failed");
        _isMounted = false;
        return false;
    }
    cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        Serial.println("[Storage] No SD card attached");
        _isMounted = false;
        return false;
    }
    Serial.printf("[Storage] SD mounted. Size: %lluMB\n", SD.cardSize() / (1024 * 1024));
    _isMounted = true;
    createFoldersIfMissing();
    createDemoFilesIfMissing();
    return true;
}

void StorageManager::createFoldersIfMissing() {
    const char* folders[] = {
        "/Flipper-1",
        "/Flipper-1/subghz",
        "/Flipper-1/ir",
        "/Flipper-1/nfc",
        "/Flipper-1/badusb",
        "/Flipper-1/settings"
    };

    for (const char* path : folders) {
        if (!SD.exists(path)) {
            Serial.printf("[Storage] Creating directory: %s\n", path);
            SD.mkdir(path);
        }
    }
}

void StorageManager::createDemoFilesIfMissing() {
    if (!SD.exists("/Flipper-1/subghz/House_portal.sub")) {
        SubGhzFile sub1;
        sub1.frequency = 433920000;
        sub1.preset    = "FSK_PULSE";
        sub1.protocol  = "Princeton";
        sub1.bits      = 24;
        sub1.key       = "00 00 00 00 00 00 A1 B2";
        sub1.save("/Flipper-1/subghz/House_portal.sub");
    }
    if (!SD.exists("/Flipper-1/subghz/Garage_Door.sub")) {
        SubGhzFile sub2;
        sub2.frequency = 868350000;
        sub2.preset    = "AM650";
        sub2.protocol  = "Came";
        sub2.bits      = 12;
        sub2.key       = "00 00 00 00 00 00 0E 42";
        sub2.save("/Flipper-1/subghz/Garage_Door.sub");
    }

    if (!SD.exists("/Flipper-1/ir/TV_Power.ir")) {
        IrFile ir1;
        ir1.name     = "TV_Power";
        ir1.protocol = "NEC";
        ir1.address  = 0x00000000;
        ir1.command  = 0x00FFA15D;
        ir1.bits     = 32;
        ir1.save("/Flipper-1/ir/TV_Power.ir");
    }

    if (!SD.exists("/Flipper-1/nfc/flag.nfc")) {
        NfcFile nfc1;
        nfc1.deviceType = "ISO14443A";
        nfc1.uid        = "04 A1 B2 C3 D4 E5 F6";
        nfc1.atqa       = "00 04";
        nfc1.sak        = "08";
        nfc1.save("/Flipper-1/nfc/flag.nfc");
    }

    if (!SD.exists("/Flipper-1/badusb/demo_windows.txt")) {
        writeFile("/Flipper-1/badusb/demo_windows.txt", "DELAY 500\nSTRING Hello Windows!");
    }
    if (!SD.exists("/Flipper-1/badusb/rickroll.txt")) {
        writeFile("/Flipper-1/badusb/rickroll.txt", "STRING never gonna give you up");
    }
}

bool StorageManager::renameFile(const char* oldPath, const char* newPath) {
    if (!_isMounted) return false;
    return SD.rename(oldPath, newPath);
}

std::vector<String> StorageManager::listFiles(const char* folderPath) {
    std::vector<String> fileList = {};
    File                root = File();
    File                file = File();
    String              name = "";

    if (!_isMounted) return fileList;
    root = SD.open(folderPath);
    if (!root || !root.isDirectory()) {
        Serial.printf("[Storage] Failed to open directory: %s\n", folderPath);
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

std::vector<String> StorageManager::listSubGhzFiles() {
    return listFiles("/Flipper-1/subghz");
}

std::vector<String> StorageManager::listIrFiles() {
    return listFiles("/Flipper-1/ir");
}

std::vector<String> StorageManager::listNfcFiles() {
    return listFiles("/Flipper-1/nfc");
}

std::vector<String> StorageManager::listBadUsbFiles() {
    return listFiles("/Flipper-1/badusb");
}

bool StorageManager::loadSubGhz(const char* filename, SubGhzFile& out) {
    String fullPath = filename;
    if (!fullPath.startsWith("/")) fullPath = "/Flipper-1/subghz/" + fullPath;
    return out.load(fullPath.c_str());
}

bool StorageManager::saveSubGhz(const char* filename, const SubGhzFile& in) {
    String fullPath = filename;
    if (!fullPath.startsWith("/")) fullPath = "/Flipper-1/subghz/" + fullPath;
    return in.save(fullPath.c_str());
}

bool StorageManager::loadIr(const char* filename, IrFile& out) {
    String fullPath = filename;
    if (!fullPath.startsWith("/")) fullPath = "/Flipper-1/ir/" + fullPath;
    return out.load(fullPath.c_str());
}

bool StorageManager::saveIr(const char* filename, const IrFile& in) {
    String fullPath = filename;
    if (!fullPath.startsWith("/")) fullPath = "/Flipper-1/ir/" + fullPath;
    return in.save(fullPath.c_str());
}

bool StorageManager::loadNfc(const char* filename, NfcFile& out) {
    String fullPath = filename;
    if (!fullPath.startsWith("/")) fullPath = "/Flipper-1/nfc/" + fullPath;
    return out.load(fullPath.c_str());
}

bool StorageManager::saveNfc(const char* filename, const NfcFile& in) {
    String fullPath = filename;
    if (!fullPath.startsWith("/")) fullPath = "/Flipper-1/nfc/" + fullPath;
    return in.save(fullPath.c_str());
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