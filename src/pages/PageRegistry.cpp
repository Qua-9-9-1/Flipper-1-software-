#include "PageRegistry.hpp"

PageMainMenu mainMenu;
PageBoot     bootPage;

PageList        subGhzMenu("Sub-GHz");
PageList        nfcMenu("NFC / RFID");
PageList        IR("IR");
PageList        badUSB("System / BadUSB");
PageList        settings("Settings");
PageControlTest controlTestPage;

namespace PageRegistry {

void setupPages() {
    subGhzMenu.addItem("Read / Sniff", nullptr);
    subGhzMenu.addItem("Saved", nullptr);
    subGhzMenu.addItem("Add Manually", nullptr);

    nfcMenu.addItem("Read Card", nullptr);
    nfcMenu.addItem("Emulate", nullptr);

    IR.addItem("Universal Remote", nullptr);
    IR.addItem("Capture Signal", nullptr);
    IR.addItem("Saved Signals", nullptr);

    badUSB.addItem("Windows", nullptr, 0);
    badUSB.addItem("macOS", nullptr, 1);
    badUSB.addItem("Linux", nullptr);
    // badUSB.addItem("Payloads", nullptr);
    // badUSB.addItem("Create Payload", nullptr);

    settings.addItem("Display", nullptr);
    settings.addItem("Sound", nullptr);
    settings.addItem("System Info", nullptr);
    settings.addItem("About", nullptr);
    settings.addItem("Controls Test", &controlTestPage);

    mainMenu.addIcon(ICON_RADIO, &subGhzMenu);
    mainMenu.addIcon(ICON_NFC, &nfcMenu);
    mainMenu.addIcon(ICON_IR, &IR);
    mainMenu.addIcon(ICON_USB, &badUSB);
    mainMenu.addIcon(ICON_SETTINGS, &settings);
}
}  // namespace PageRegistry