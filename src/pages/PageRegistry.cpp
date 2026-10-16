#include "PageRegistry.hpp"
#include "core/TextInputPage.hpp"

namespace PageRegistry {
PageMainMenu mainMenuPage;
PageBoot     bootPage;
PageError    errorPage;

PageList        subGhzMenuPage("Sub-GHz");
PageList        subGhzSignalsPage("Sub-GHz Signals");
PageList        nfcMenuPage("NFC / RFID");
PageList        IRPage("IR");
PageIR          IRReceiverPage;
PageList        badUSBPage("System / BadUSB");
PageList        settingsPage("Settings");
PageControlTest controlTestPage;
PageLEDTest     ledTestPage;
TextInputPage   textInputPage("Rename Signal", nullptr, 0);

static char subGhzSignal1[16] = "Garage Door";
static char subGhzSignal2[16] = "Car Key 433";
static char subGhzSignal3[16] = "Gate Remote";

void setupPages() {
    subGhzSignalsPage.addItem(subGhzSignal1, {
        ItemOption("Open", []() {
            Serial.printf("[Sub-GHz] Opening signal page: %s\n", subGhzSignal1);
        }),
        ItemOption("Rename", []() {
            textInputPage.setTarget("Rename Signal", subGhzSignal1, sizeof(subGhzSignal1));
            PageManager::getInstance()->pushPage(&textInputPage);
        }),
        ItemOption("Details", []() {
            Serial.printf("[Sub-GHz] Details for %s: Freq=433.92MHz | Modulation=AM650 | Key=0xA1B2C3\n", subGhzSignal1);
        })
    });

    subGhzSignalsPage.addItem(subGhzSignal2, {
        ItemOption("Open", []() {
            Serial.printf("[Sub-GHz] Opening signal page: %s\n", subGhzSignal2);
        }),
        ItemOption("Rename", []() {
            textInputPage.setTarget("Rename Signal", subGhzSignal2, sizeof(subGhzSignal2));
            PageManager::getInstance()->pushPage(&textInputPage);
        }),
        ItemOption("Details", []() {
            Serial.printf("[Sub-GHz] Details for %s: Freq=433.92MHz | Protocol=KeeLoq | Seed=0x987654\n", subGhzSignal2);
        })
    });

    subGhzSignalsPage.addItem(subGhzSignal3, {
        ItemOption("Open", []() {
            Serial.printf("[Sub-GHz] Opening signal page: %s\n", subGhzSignal3);
        }),
        ItemOption("Rename", []() {
            textInputPage.setTarget("Rename Signal", subGhzSignal3, sizeof(subGhzSignal3));
            PageManager::getInstance()->pushPage(&textInputPage);
        }),
        ItemOption("Details", []() {
            Serial.printf("[Sub-GHz] Details for %s: Freq=868.35MHz | Protocol=Came | ID=0x55AA12\n", subGhzSignal3);
        })
    });

    subGhzMenuPage.addItem("Read / Sniff");
    subGhzMenuPage.addItem("Signals", {PageActions::openPage(&subGhzSignalsPage)});
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