#pragma once

#include <Arduino.h>
#include <vector>

#include "Page.hpp"
#include "PageManager.hpp"
#include "helpers/AudioHelper.h"
#include "helpers/LedHelper.h"

class PageDetails : public IPage {
   public:
    explicit PageDetails(const char* title = "Details");

    void setTitle(const char* title);
    void setText(const String& text);
    void setLines(const std::vector<String>& lines);
    void addLine(const String& line);
    void clear();

    void show(const char* title, const String& text);
    void show(const char* title, const std::vector<String>& lines);

    void onEnter() override;
    void onEvent(AppEvent* event) override;
    void draw(U8G2* u8g2) override;

   private:
    void drawHeader(U8G2* u8g2) const;
    void drawTextLines(U8G2* u8g2) const;
    void drawScrollbar(U8G2* u8g2) const;
    void wrapAndAddLine(const String& line);

    String              _title;
    std::vector<String> _lines;
    int                 _scrollOffset;
};
