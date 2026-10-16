#pragma once
#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <map>
#include <vector>

class KeyValueParser {
   public:
    KeyValueParser() = default;

    bool loadFromFile(const char* filepath);
    bool parseString(const String& content);
    bool saveToFile(const char* filepath);

    String get(const String& key, const String& defaultValue = "") const;

    int      getInt(const String& key, int defaultValue = 0) const;
    uint32_t getUInt(const String& key, uint32_t defaultValue = 0) const;
    uint64_t getHex64(const String& key, uint64_t defaultValue = 0) const;

    void set(const String& key, const String& value);
    void set(const String& key, int value);
    void set(const String& key, uint32_t value);
    void setHex(const String& key, uint64_t value, uint8_t minHexDigits = 0);

    bool hasKey(const String& key) const;
    void clear();
    String serialize() const;

   private:
    static String trim(const String& str);

    std::vector<String>      _orderedKeys;
    std::map<String, String> _map;
};
