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
    subGhzMenuPage.addItem("Read / Sniff");
    subGhzMenuPage.addItem("Signals");
    subGhzMenuPage.addItem("Add Manually");

    nfcMenuPage.addItem("Read Card");
    nfcMenuPage.addItem("Cards");

    IRPage.addItem("Read", {PageActions::openPage(&IRReceiverPage)});
    IRPage.addItem("Signals");
    IRPage.addItem("Universal Remote");

    badUSBPage.addItemWithIcon("Windows", {}, 1);
    badUSBPage.addItemWithIcon("macOS", {}, 2);
    badUSBPage.addItem("Linux");
    // badUSBPage.addItem("Payloads");
    // badUSBPage.addItem("Create Payload");
    std::vector<String> scripts = StorageManager::getInstance()->listFiles("/Flipper-1/badusb");

    if (scripts.empty()) {
        badUSBPage.addItem("No Scripts found");
    } else {
        static std::vector<String> persistentNames;
        persistentNames = scripts;

        for (size_t i = 0; i < persistentNames.size(); i++) {
            String fullPath = "/Flipper-1/badusb/" + persistentNames[i];
            badUSBPage.addItem(persistentNames[i].c_str(), {PageActions::runBadUsb(fullPath)});
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