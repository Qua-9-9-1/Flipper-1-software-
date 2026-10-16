#pragma once
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>
#include "KeyValueParser.hpp"
#include "System.hpp"

struct IrFile {
    String   filetype = "Flipper IR File";
    int      version  = 1;
    String   name     = "Power";
    String   type     = "parsed";
    String   protocol = "NEC";
    uint32_t address  = 0;
    uint64_t command  = 0;
    uint16_t bits     = 32;

    bool load(const char* filepath) {
        KeyValueParser parser;
        if (!parser.loadFromFile(filepath)) return false;

        filetype = parser.get("Filetype", filetype);
        version  = parser.getInt("Version", version);
        name     = parser.get("Name", name);
        type     = parser.get("Type", type);
        protocol = parser.get("Protocol", protocol);
        address  = (uint32_t)parser.getHex64("Address", address);
        command  = parser.getHex64("Command", command);
        bits     = (uint16_t)parser.getInt("Bits", bits);
        return true;
    }

    bool save(const char* filepath) const {
        KeyValueParser parser;
        parser.set("Filetype", filetype);
        parser.set("Version", version);
        parser.set("Name", name);
        parser.set("Type", type);
        parser.set("Protocol", protocol);
        parser.setHex("Address", address, 8);
        parser.setHex("Command", command, 8);
        parser.set("Bits", (int)bits);
        return parser.saveToFile(filepath);
    }

    IrSignal toIrSignal() const {
        IrSignal sig;
        sig.protocol = strToDecodeType(protocol.c_str());
        sig.value    = command;
        sig.bits     = bits;
        sig.address  = (uint16_t)address;
        return sig;
    }

    void fromIrSignal(const IrSignal& sig, const char* sigName = "Signal") {
        name     = sigName;
        protocol = typeToString(sig.protocol);
        command  = sig.value;
        bits     = sig.bits;
        address  = sig.address;
        type     = "parsed";
    }
};
