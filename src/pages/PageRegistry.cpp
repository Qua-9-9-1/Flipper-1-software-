#include "PageRegistry.hpp"

namespace PageRegistry {
PageMainMenu mainMenuPage;
PageBoot     bootPage;
PageError    errorPage;

PageList        subGhzMenuPage("Sub-GHz");
PageList        nfcMenuPage("NFC / RFID");
PageList        IRPage("IR");
PageIR          IRReceiverPage;
PageList        badUSBPage("System / BadUSB");
PageList        settingsPage("Settings");
PageControlTest controlTestPage;
PageLEDTest     ledTestPage;

void setupPages() {
    subGhzMenuPage.addItem("Read / Sniff", nullptr);
    subGhzMenuPage.addItem("Saved", nullptr);
    subGhzMenuPage.addItem("Add Manually", nullptr);

    nfcMenuPage.addItem("Read Card", nullptr);
    nfcMenuPage.addItem("Emulate", nullptr);

    IRPage.addItem("Universal Remote", nullptr);
    IRPage.addItem("Capture Signal", &IRReceiverPage);
    IRPage.addItem("Saved Signals", nullptr);
    badUSBPage.addItem("Windows", nullptr, 0);
    badUSBPage.addItem("macOS", nullptr, 1);
    badUSBPage.addItem("Linux", nullptr);
    // badUSBPage.addItem("Payloads", nullptr);
    // badUSBPage.addItem("Create Payload", nullptr);
    std::vector<String> scripts = StorageManager::getInstance()->listFiles("/Flipper-1/badusb");

    if (scripts.empty()) {
        badUSBPage.addItem("No Scripts found", nullptr);
    } else {
        static std::vector<String> persistentNames;
        persistentNames = scripts;

        for (size_t i = 0; i < persistentNames.size(); i++) {
            badUSBPage.addItem(persistentNames[i].c_str(), nullptr, i);
        }
    }

    settingsPage.addItem("Display", nullptr);
    settingsPage.addItem("Sound", nullptr);
    settingsPage.addItem("Controls Test", &controlTestPage);
    settingsPage.addItem("LED Test", &ledTestPage);
    settingsPage.addItem("System Info", nullptr);
    settingsPage.addItem("About", nullptr);

    mainMenuPage.addIcon(GHZ, &subGhzMenuPage);
    mainMenuPage.addIcon(NFC_RFID, &nfcMenuPage);
    mainMenuPage.addIcon(IR, &IRPage);
    mainMenuPage.addIcon(USB, &badUSBPage);
    mainMenuPage.addIcon(SETTINGS, &settingsPage);
}
}  // namespace PageRegistry