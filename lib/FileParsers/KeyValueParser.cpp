#include "KeyValueParser.hpp"

String KeyValueParser::trim(const String& str) {
    int start = 0;
    while (start < (int)str.length() && (str[start] == ' ' || str[start] == '\t' || str[start] == '\r' || str[start] == '\n')) {
        start++;
    }
    int end = str.length() - 1;
    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\r' || str[end] == '\n')) {
        end--;
    }
    return str.substring(start, end + 1);
}

void KeyValueParser::clear() {
    _orderedKeys.clear();
    _map.clear();
}

bool KeyValueParser::loadFromFile(const char* filepath) {
    clear();
    if (!SD.exists(filepath)) {
        return false;
    }

    File file = SD.open(filepath, FILE_READ);
    if (!file) {
        return false;
    }

    String currentLine = "";
    while (file.available()) {
        char c = (char)file.read();
        if (c == '\n' || c == '\r') {
            if (currentLine.length() > 0) {
                String line = trim(currentLine);
                if (line.length() > 0 && line[0] != '#' && line[0] != ';') {
                    int sepIdx = line.indexOf(':');
                    if (sepIdx != -1) {
                        String key = trim(line.substring(0, sepIdx));
                        String val = trim(line.substring(sepIdx + 1));
                        set(key, val);
                    }
                }
                currentLine = "";
            }
        } else {
            currentLine += c;
        }
    }

    if (currentLine.length() > 0) {
        String line = trim(currentLine);
        if (line.length() > 0 && line[0] != '#' && line[0] != ';') {
            int sepIdx = line.indexOf(':');
            if (sepIdx != -1) {
                String key = trim(line.substring(0, sepIdx));
                String val = trim(line.substring(sepIdx + 1));
                set(key, val);
            }
        }
    }

    file.close();
    return true;
}

bool KeyValueParser::parseString(const String& content) {
    clear();
    int startIdx = 0;
    while (startIdx < (int)content.length()) {
        int nextLine = content.indexOf('\n', startIdx);
        if (nextLine == -1) nextLine = content.length();
        String line = trim(content.substring(startIdx, nextLine));
        startIdx = nextLine + 1;

        if (line.length() == 0 || line[0] == '#' || line[0] == ';') continue;
        int sepIdx = line.indexOf(':');
        if (sepIdx != -1) {
            String key = trim(line.substring(0, sepIdx));
            String val = trim(line.substring(sepIdx + 1));
            set(key, val);
        }
    }
    return true;
}

bool KeyValueParser::saveToFile(const char* filepath) {
    File file = SD.open(filepath, FILE_WRITE);
    if (!file) {
        return false;
    }

    String serialized = serialize();
    size_t written = file.print(serialized);
    file.close();
    return written == serialized.length();
}

String KeyValueParser::serialize() const {
    String output = "";
    for (const auto& key : _orderedKeys) {
        auto it = _map.find(key);
        if (it != _map.end()) {
            output += key + ": " + it->second + "\n";
        }
    }
    return output;
}

String KeyValueParser::get(const String& key, const String& defaultValue) const {
    auto it = _map.find(key);
    if (it != _map.end()) return it->second;
    return defaultValue;
}

int KeyValueParser::getInt(const String& key, int defaultValue) const {
    String val = get(key);
    if (val.length() == 0) return defaultValue;
    return val.toInt();
}

uint32_t KeyValueParser::getUInt(const String& key, uint32_t defaultValue) const {
    String val = get(key);
    if (val.length() == 0) return defaultValue;
    return (uint32_t)strtoul(val.c_str(), nullptr, 10);
}

uint64_t KeyValueParser::getHex64(const String& key, uint64_t defaultValue) const {
    String val = get(key);
    if (val.length() == 0) return defaultValue;

    String cleaned = "";
    for (size_t i = 0; i < val.length(); i++) {
        char c = val[i];
        if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
            cleaned += c;
        }
    }
    if (cleaned.startsWith("0x") || cleaned.startsWith("0X")) {
        cleaned = cleaned.substring(2);
    }
    return strtoull(cleaned.c_str(), nullptr, 16);
}

void KeyValueParser::set(const String& key, const String& value) {
    if (!hasKey(key)) {
        _orderedKeys.push_back(key);
    }
    _map[key] = value;
}

void KeyValueParser::set(const String& key, int value) {
    set(key, String(value));
}

void KeyValueParser::set(const String& key, uint32_t value) {
    set(key, String(value));
}

void KeyValueParser::setHex(const String& key, uint64_t value, uint8_t minHexDigits) {
    char buf[20];
    if (minHexDigits > 0) {
        snprintf(buf, sizeof(buf), "%0*llX", (int)minHexDigits, value);
    } else {
        snprintf(buf, sizeof(buf), "%llX", value);
    }
    set(key, String(buf));
}

bool KeyValueParser::hasKey(const String& key) const {
    return _map.find(key) != _map.end();
}
