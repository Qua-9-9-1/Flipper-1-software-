#pragma once
#include <Arduino.h>
#include "KeyValueParser.hpp"

struct NfcFile {
    String filetype   = "Flipper NFC File";
    int    version    = 1;
    String deviceType = "ISO14443A";
    String uid        = "04 A1 B2 C3 D4 E5 F6";
    String atqa       = "00 04";
    String sak        = "08";

    bool load(const char* filepath) {
        KeyValueParser parser;
        if (!parser.loadFromFile(filepath)) return false;

        filetype   = parser.get("Filetype", filetype);
        version    = parser.getInt("Version", version);
        deviceType = parser.get("Device type", deviceType);
        uid        = parser.get("UID", uid);
        atqa       = parser.get("ATQA", atqa);
        sak        = parser.get("SAK", sak);
        return true;
    }

    bool save(const char* filepath) const {
        KeyValueParser parser;
        parser.set("Filetype", filetype);
        parser.set("Version", version);
        parser.set("Device type", deviceType);
        parser.set("UID", uid);
        parser.set("ATQA", atqa);
        parser.set("SAK", sak);
        return parser.saveToFile(filepath);
    }
};
