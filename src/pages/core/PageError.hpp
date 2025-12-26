#pragma once
#include "Page.hpp"
#include "PageManager.hpp"
#

class PageError : public IPage {
   public:
    PageError();
    void setError(const char* msg);
    void onEnter() override {}
    void onEvent(AppEvent* event) override;
    void draw(U8G2* u8g2) override;

   private:
    const char* errorMessage;
    int         selected;
};