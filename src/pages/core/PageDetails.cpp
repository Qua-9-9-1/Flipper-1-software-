#include "PageDetails.hpp"

#include <algorithm>

namespace {
constexpr int SCREEN_WIDTH               = 128;
constexpr int SCREEN_HEIGHT              = 64;
constexpr int HEADER_HEIGHT              = 16;
constexpr int LINE_HEIGHT                = 11;
constexpr int LINES_PER_SCREEN           = 4;
constexpr int TEXT_X                     = 4;
constexpr int MAX_CHARS_PER_LINE         = 19;
constexpr int SCROLLBAR_X                = 123;
constexpr int SCROLLBAR_WIDTH            = 5;
constexpr int SCROLLBAR_THUMB_MIN_HEIGHT = 4;
}  // namespace

PageDetails::PageDetails(const char* title)
    : _title(title ? title : "Details"), _scrollOffset(0) {}

void PageDetails::setTitle(const char* title) {
    _title = title ? title : "Details";
}

void PageDetails::clear() {
    _lines.clear();
    _scrollOffset = 0;
}

void PageDetails::addLine(const String& line) {
    wrapAndAddLine(line);
}

void PageDetails::setLines(const std::vector<String>& lines) {
    clear();
    for (const auto& line : lines) {
        wrapAndAddLine(line);
    }
}

void PageDetails::setText(const String& text) {
    clear();
    int start = 0;
    const int len = text.length();

    for (int i = 0; i <= len; ++i) {
        if (i == len || text.charAt(i) == '\n') {
            String sub = text.substring(start, i);
            if (sub.endsWith("\r")) {
                sub.remove(sub.length() - 1);
            }
            wrapAndAddLine(sub);
            start = i + 1;
        }
    }
}

void PageDetails::show(const char* title, const String& text) {
    setTitle(title);
    setText(text);
    PageManager::getInstance()->pushPage(this);
}

void PageDetails::show(const char* title, const std::vector<String>& lines) {
    setTitle(title);
    setLines(lines);
    PageManager::getInstance()->pushPage(this);
}

void PageDetails::wrapAndAddLine(const String& line) {
    if (line.length() <= static_cast<unsigned int>(MAX_CHARS_PER_LINE)) {
        _lines.push_back(line);
        return;
    }

    int start = 0;
    const int totalLen = line.length();

    while (start < totalLen) {
        int remaining = totalLen - start;
        if (remaining <= MAX_CHARS_PER_LINE) {
            _lines.push_back(line.substring(start));
            break;
        }

        int splitPos = -1;
        for (int i = start + MAX_CHARS_PER_LINE; i > start; --i) {
            char c = line.charAt(i);
            if (c == ' ' || c == '|' || c == ',' || c == ';') {
                splitPos = i;
                break;
            }
        }

        if (splitPos <= start) {
            splitPos = start + MAX_CHARS_PER_LINE;
            _lines.push_back(line.substring(start, splitPos));
            start = splitPos;
        } else {
            _lines.push_back(line.substring(start, splitPos));
            start = (line.charAt(splitPos) == ' ') ? splitPos + 1 : splitPos;
        }
    }
}

void PageDetails::onEnter() {
    _scrollOffset = 0;
    setLEDMode(LED_MODE_BATTERY, 100);
}

void PageDetails::onEvent(AppEvent* event) {
    if (event->type != EVENT_BUTTON_CLICK) {
        return;
    }

    const int maxScroll = std::max(0, static_cast<int>(_lines.size()) - LINES_PER_SCREEN);

    switch (event->value) {
        case BTN_UP:
            if (_scrollOffset > 0) {
                _scrollOffset--;
                playSound(SOUND_TICK);
            }
            break;

        case BTN_DOWN:
            if (_scrollOffset < maxScroll) {
                _scrollOffset++;
                playSound(SOUND_TICK);
            }
            break;

        case BTN_BACK:
        case BTN_OK:
            playSound(SOUND_TICK);
            PageManager::getInstance()->popPage();
            break;

        default:
            break;
    }
}

void PageDetails::draw(U8G2* u8g2) {
    u8g2->clearBuffer();
    drawHeader(u8g2);
    drawTextLines(u8g2);
    drawScrollbar(u8g2);
}

void PageDetails::drawHeader(U8G2* u8g2) const {
    u8g2->setDrawColor(1);
    u8g2->drawBox(0, 0, SCREEN_WIDTH, HEADER_HEIGHT);

    u8g2->setDrawColor(0);
    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->drawStr(TEXT_X, (HEADER_HEIGHT + 8) / 2, _title.c_str());
}

void PageDetails::drawTextLines(U8G2* u8g2) const {
    u8g2->setDrawColor(1);
    u8g2->setFont(u8g2_font_6x10_tf);

    const int totalLines = static_cast<int>(_lines.size());
    const int endLine    = std::min(totalLines, _scrollOffset + LINES_PER_SCREEN);

    for (int i = _scrollOffset; i < endLine; ++i) {
        const int visualIndex = i - _scrollOffset;
        const int yPos        = HEADER_HEIGHT + (visualIndex * LINE_HEIGHT) + 9;
        u8g2->drawStr(TEXT_X, yPos, _lines[i].c_str());
    }
}

void PageDetails::drawScrollbar(U8G2* u8g2) const {
    const int totalLines = static_cast<int>(_lines.size());
    if (totalLines <= LINES_PER_SCREEN) {
        return;
    }

    const int totalHeight = SCREEN_HEIGHT - HEADER_HEIGHT;
    int thumbHeight = (totalHeight * LINES_PER_SCREEN) / totalLines;
    if (thumbHeight < SCROLLBAR_THUMB_MIN_HEIGHT) {
        thumbHeight = SCROLLBAR_THUMB_MIN_HEIGHT;
    }

    const int maxScroll = totalLines - LINES_PER_SCREEN;
    const int thumbY    = HEADER_HEIGHT + ((totalHeight - thumbHeight) * _scrollOffset) / maxScroll;

    u8g2->drawFrame(SCROLLBAR_X, HEADER_HEIGHT, SCROLLBAR_WIDTH, totalHeight);
    u8g2->drawBox(SCROLLBAR_X + 1, thumbY, SCROLLBAR_WIDTH - 2, thumbHeight);
}
