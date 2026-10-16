#include "PageRegistry.hpp"
#include "core/TextInputPage.hpp"
#include "managers/StorageManager.hpp"
#include "SubGhzFile.hpp"
#include "IrFile.hpp"
#include "NfcFile.hpp"

namespace PageRegistry {
PageMainMenu mainMenuPage;
PageBoot     bootPage;
PageError    errorPage;

PageList        subGhzMenuPage("Sub-GHz");
PageList        subGhzSignalsPage("Sub-GHz Signals");
PageList        nfcMenuPage("NFC / RFID");
PageList        nfcCardsPage("Saved NFC Cards");
PageList        IRPage("IR");
PageList        irSignalsPage("Saved IR Signals");
PageIR          IRReceiverPage;
PageList        badUSBPage("System / BadUSB");
PageList        settingsPage("Settings");
PageControlTest controlTestPage;
PageLEDTest     ledTestPage;
TextInputPage   textInputPage("Rename Signal", nullptr, 0);

static char subGhzSignal1[16] = "Garage Door";
static char subGhzSignal2[16] = "Car Key 433";
static char subGhzSignal3[16] = "Gate Remote";
static char renameBuffer[32];

static String stripExtension(const String& filename) {
    int dotIndex = filename.lastIndexOf('.');
    if (dotIndex > 0) {
        return filename.substring(0, dotIndex);
    }
    return filename;
}

void setupPages() {
    std::vector<String> subFiles = StorageManager::getInstance()->listSubGhzFiles();
    if (subFiles.empty()) {
        subGhzSignalsPage.addItem(subGhzSignal1, {
            ItemOption("Open", []() {
                Serial.printf("[Sub-GHz] Opening signal: %s\n", subGhzSignal1);
            }),
            ItemOption("Rename", []() {
                strncpy(renameBuffer, subGhzSignal1, sizeof(renameBuffer) - 1);
                renameBuffer[sizeof(renameBuffer) - 1] = '\0';
                textInputPage.setTarget("Rename Signal", renameBuffer, sizeof(renameBuffer), [](const char* newName) {
                    if (!newName || newName[0] == '\0') return;
                    strncpy(subGhzSignal1, newName, sizeof(subGhzSignal1) - 1);
                    subGhzSignal1[sizeof(subGhzSignal1) - 1] = '\0';
                    subGhzSignalsPage.updateItemTitle(0, newName);
                    Serial.printf("[Sub-GHz] Renamed to: %s\n", newName);
                });
                PageManager::getInstance()->pushPage(&textInputPage);
            }),
            ItemOption("Details", []() {
                Serial.printf("[Sub-GHz] Details for %s: Freq=433.92MHz | Modulation=FSK_PULSE | Key=A1B2C3\n", subGhzSignal1);
            })
        });

        subGhzSignalsPage.addItem(subGhzSignal2, {
            ItemOption("Open", []() {
                Serial.printf("[Sub-GHz] Opening signal: %s\n", subGhzSignal2);
            }),
            ItemOption("Rename", []() {
                strncpy(renameBuffer, subGhzSignal2, sizeof(renameBuffer) - 1);
                renameBuffer[sizeof(renameBuffer) - 1] = '\0';
                textInputPage.setTarget("Rename Signal", renameBuffer, sizeof(renameBuffer), [](const char* newName) {
                    if (!newName || newName[0] == '\0') return;
                    strncpy(subGhzSignal2, newName, sizeof(subGhzSignal2) - 1);
                    subGhzSignal2[sizeof(subGhzSignal2) - 1] = '\0';
                    subGhzSignalsPage.updateItemTitle(1, newName);
                    Serial.printf("[Sub-GHz] Renamed to: %s\n", newName);
                });
                PageManager::getInstance()->pushPage(&textInputPage);
            }),
            ItemOption("Details", []() {
                Serial.printf("[Sub-GHz] Details for %s: Freq=433.92MHz | Protocol=KeeLoq | Seed=987654\n", subGhzSignal2);
            })
        });

        subGhzSignalsPage.addItem(subGhzSignal3, {
            ItemOption("Open", []() {
                Serial.printf("[Sub-GHz] Opening signal: %s\n", subGhzSignal3);
            }),
            ItemOption("Rename", []() {
                strncpy(renameBuffer, subGhzSignal3, sizeof(renameBuffer) - 1);
                renameBuffer[sizeof(renameBuffer) - 1] = '\0';
                textInputPage.setTarget("Rename Signal", renameBuffer, sizeof(renameBuffer), [](const char* newName) {
                    if (!newName || newName[0] == '\0') return;
                    strncpy(subGhzSignal3, newName, sizeof(subGhzSignal3) - 1);
                    subGhzSignal3[sizeof(subGhzSignal3) - 1] = '\0';
                    subGhzSignalsPage.updateItemTitle(2, newName);
                    Serial.printf("[Sub-GHz] Renamed to: %s\n", newName);
                });
                PageManager::getInstance()->pushPage(&textInputPage);
            }),
            ItemOption("Details", []() {
                Serial.printf("[Sub-GHz] Details for %s: Freq=868.35MHz | Protocol=Came | ID=55AA12\n", subGhzSignal3);
            })
        });
    } else {
        static std::vector<String> persistentSubNames;
        persistentSubNames = subFiles;
        for (size_t i = 0; i < persistentSubNames.size(); i++) {
            String fName = persistentSubNames[i];
            String displayName = stripExtension(fName);
            int itemIndex = (int)i;
            subGhzSignalsPage.addItem(displayName.c_str(), {
                ItemOption("Open", [fName]() {
                    SubGhzFile file;
                    if (StorageManager::getInstance()->loadSubGhz(fName.c_str(), file)) {
                        Serial.printf("[Sub-GHz] Emulating %s (Freq: %lu Hz, Proto: %s, Key: %s)\n",
                                      fName.c_str(), (unsigned long)file.frequency, file.protocol.c_str(), file.key.c_str());
                    }
                }),
                ItemOption("Rename", [fName, itemIndex]() {
                    String base = stripExtension(fName);
                    strncpy(renameBuffer, base.c_str(), sizeof(renameBuffer) - 1);
                    renameBuffer[sizeof(renameBuffer) - 1] = '\0';
                    textInputPage.setTarget("Rename Signal", renameBuffer, sizeof(renameBuffer), [fName, itemIndex](const char* newName) {
                        if (!newName || newName[0] == '\0') return;
                        String oldPath = "/Flipper-1/subghz/" + fName;
                        String newPath = "/Flipper-1/subghz/" + String(newName) + ".sub";
                        if (StorageManager::getInstance()->renameFile(oldPath.c_str(), newPath.c_str())) {
                            subGhzSignalsPage.updateItemTitle(itemIndex, newName);
                            Serial.printf("[Sub-GHz] File renamed from %s to %s.sub\n", fName.c_str(), newName);
                        } else {
                            Serial.printf("[Sub-GHz] Failed to rename %s on SD\n", fName.c_str());
                        }
                    });
                    PageManager::getInstance()->pushPage(&textInputPage);
                }),
                ItemOption("Details", [fName]() {
                    SubGhzFile file;
                    if (StorageManager::getInstance()->loadSubGhz(fName.c_str(), file)) {
                        Serial.printf("[Sub-GHz] Details: %s | Freq: %lu Hz | Preset: %s | Protocol: %s | Bits: %d | Key: %s\n",
                                      fName.c_str(), (unsigned long)file.frequency, file.preset.c_str(), file.protocol.c_str(),
                                      (int)file.bits, file.key.c_str());
                    }
                })
            });
        }
    }

    subGhzMenuPage.addItem("Read / Sniff");
    subGhzMenuPage.addItem("Signals", {PageActions::openPage(&subGhzSignalsPage)});
    subGhzMenuPage.addItem("Add Manually");

    std::vector<String> nfcFiles = StorageManager::getInstance()->listNfcFiles();
    if (nfcFiles.empty()) {
        nfcCardsPage.addItem("Badge_Immeuble", {
            ItemOption("Emulate", []() {
                Serial.println("[NFC] Emulating card: Badge_Immeuble");
            }),
            ItemOption("Details", []() {
                Serial.println("[NFC] Details: Type=ISO14443A, UID=04 A1 B2 C3 D4 E5 F6");
            })
        });
    } else {
        static std::vector<String> persistentNfcNames;
        persistentNfcNames = nfcFiles;
        for (size_t i = 0; i < persistentNfcNames.size(); i++) {
            String fName = persistentNfcNames[i];
            String displayName = stripExtension(fName);
            int itemIndex = (int)i;
            nfcCardsPage.addItem(displayName.c_str(), {
                ItemOption("Emulate", [fName]() {
                    NfcFile file;
                    if (StorageManager::getInstance()->loadNfc(fName.c_str(), file)) {
                        Serial.printf("[NFC] Emulating %s (UID: %s)\n", fName.c_str(), file.uid.c_str());
                    }
                }),
                ItemOption("Rename", [fName, itemIndex]() {
                    String base = stripExtension(fName);
                    strncpy(renameBuffer, base.c_str(), sizeof(renameBuffer) - 1);
                    renameBuffer[sizeof(renameBuffer) - 1] = '\0';
                    textInputPage.setTarget("Rename NFC", renameBuffer, sizeof(renameBuffer), [fName, itemIndex](const char* newName) {
                        if (!newName || newName[0] == '\0') return;
                        String oldPath = "/Flipper-1/nfc/" + fName;
                        String newPath = "/Flipper-1/nfc/" + String(newName) + ".nfc";
                        if (StorageManager::getInstance()->renameFile(oldPath.c_str(), newPath.c_str())) {
                            nfcCardsPage.updateItemTitle(itemIndex, newName);
                            Serial.printf("[NFC] File renamed from %s to %s.nfc\n", fName.c_str(), newName);
                        }
                    });
                    PageManager::getInstance()->pushPage(&textInputPage);
                }),
                ItemOption("Details", [fName]() {
                    NfcFile file;
                    if (StorageManager::getInstance()->loadNfc(fName.c_str(), file)) {
                        Serial.printf("[NFC] Details: %s | Type: %s | UID: %s | ATQA: %s | SAK: %s\n",
                                      fName.c_str(), file.deviceType.c_str(), file.uid.c_str(), file.atqa.c_str(), file.sak.c_str());
                    }
                })
            });
        }
    }

    nfcMenuPage.addItem("Read Card");
    nfcMenuPage.addItem("Cards", {PageActions::openPage(&nfcCardsPage)});

    std::vector<String> irFiles = StorageManager::getInstance()->listIrFiles();
    if (irFiles.empty()) {
        irSignalsPage.addItem("TV_Power", {
            ItemOption("Send", []() {
                Serial.println("[IR] Transmitting TV_Power (NEC, Cmd: 0x00FFA15D)");
            }),
            ItemOption("Details", []() {
                Serial.println("[IR] Details: TV_Power | Proto: NEC | Bits: 32");
            })
        });
    } else {
        static std::vector<String> persistentIrNames;
        persistentIrNames = irFiles;
        for (size_t i = 0; i < persistentIrNames.size(); i++) {
            String fName = persistentIrNames[i];
            String displayName = stripExtension(fName);
            int itemIndex = (int)i;
            irSignalsPage.addItem(displayName.c_str(), {
                ItemOption("Send", [fName]() {
                    IrFile file;
                    if (StorageManager::getInstance()->loadIr(fName.c_str(), file)) {
                        IrSignal sig = file.toIrSignal();
                        if (irQueue) xQueueSend(irQueue, &sig, 0);
                        Serial.printf("[IR] Sent: %s (Proto: %s, Cmd: 0x%llX)\n", fName.c_str(), file.protocol.c_str(), file.command);
                    }
                }),
                ItemOption("Rename", [fName, itemIndex]() {
                    String base = stripExtension(fName);
                    strncpy(renameBuffer, base.c_str(), sizeof(renameBuffer) - 1);
                    renameBuffer[sizeof(renameBuffer) - 1] = '\0';
                    textInputPage.setTarget("Rename IR", renameBuffer, sizeof(renameBuffer), [fName, itemIndex](const char* newName) {
                        if (!newName || newName[0] == '\0') return;
                        String oldPath = "/Flipper-1/ir/" + fName;
                        String newPath = "/Flipper-1/ir/" + String(newName) + ".ir";
                        if (StorageManager::getInstance()->renameFile(oldPath.c_str(), newPath.c_str())) {
                            irSignalsPage.updateItemTitle(itemIndex, newName);
                            Serial.printf("[IR] File renamed from %s to %s.ir\n", fName.c_str(), newName);
                        }
                    });
                    PageManager::getInstance()->pushPage(&textInputPage);
                }),
                ItemOption("Details", [fName]() {
                    IrFile file;
                    if (StorageManager::getInstance()->loadIr(fName.c_str(), file)) {
                        Serial.printf("[IR] Details: %s | Proto: %s | Addr: 0x%lX | Cmd: 0x%llX | Bits: %d\n",
                                      fName.c_str(), file.protocol.c_str(), (unsigned long)file.address, file.command, (int)file.bits);
                    }
                })
            });
        }
    }

    IRPage.addItem("Read", {PageActions::openPage(&IRReceiverPage)});
    IRPage.addItem("Signals", {PageActions::openPage(&irSignalsPage)});
    IRPage.addItem("Universal Remote");

    badUSBPage.addItemWithIcon("Windows", {}, 1);
    badUSBPage.addItemWithIcon("macOS", {}, 2);
    badUSBPage.addItem("Linux");

    std::vector<String> scripts = StorageManager::getInstance()->listBadUsbFiles();
    if (scripts.empty()) {
        badUSBPage.addItem("No Scripts found");
    } else {
        static std::vector<String> persistentNames;
        persistentNames = scripts;

        for (size_t i = 0; i < persistentNames.size(); i++) {
            String fullPath = "/Flipper-1/badusb/" + persistentNames[i];
            String displayName = stripExtension(persistentNames[i]);
            badUSBPage.addItem(displayName.c_str(), {PageActions::runBadUsb(fullPath)});
        }
    }

    settingsPage.addItem("Display");
    settingsPage.addItem("Sound");
    settingsPage.addItem("Controls Test", {PageActions::openPage(&controlTestPage)});
    settingsPage.addItem("LED Test", {PageActions::openPage(&ledTestPage)});
    settingsPage.addItem("System Info");
    settingsPage.addItem("About");

    mainMenuPage.addIcon(GHZ, &subGhzMenuPage);
    mainMenuPage.addIcon(NFC_RFID, &nfcMenuPage);
    mainMenuPage.addIcon(IR, &IRPage);
    mainMenuPage.addIcon(USB, &badUSBPage);
    mainMenuPage.addIcon(SETTINGS, &settingsPage);
}
}  // namespace PageRegistry