#pragma once
#include <Arduino.h>
#include "KeyValueParser.hpp"

struct SubGhzFile {
    String   filetype = "Flipper SubGhz Key File";
    int      version  = 1;
    uint32_t frequency = 433920000;
    String   preset    = "FSK_PULSE";
    String   protocol  = "Princeton";
    uint8_t  bits      = 24;
    String   key       = "00 00 00 00 00 00 A1 B2";

    bool load(const char* filepath) {
        KeyValueParser parser;
        if (!parser.loadFromFile(filepath)) return false;

        filetype  = parser.get("Filetype", filetype);
        version   = parser.getInt("Version", version);
        frequency = parser.getUInt("Frequency", frequency);
        preset    = parser.get("Preset", preset);
        protocol  = parser.get("Protocol", protocol);
        bits      = (uint8_t)parser.getInt("Bit", bits);
        key       = parser.get("Key", key);
        return true;
    }

    bool save(const char* filepath) const {
        KeyValueParser parser;
        parser.set("Filetype", filetype);
        parser.set("Version", version);
        parser.set("Frequency", frequency);
        parser.set("Preset", preset);
        parser.set("Protocol", protocol);
        parser.set("Bit", (int)bits);
        parser.set("Key", key);
        return parser.saveToFile(filepath);
    }
};
