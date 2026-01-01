#pragma once
#include <vector>

#include "Page.hpp"

class PageManager {
   public:
    PageManager() {};
    static PageManager* getInstance();
    void                pushPage(IPage* page);
    void                popPage();
    IPage*              getCurrentPage();
    void                switchPage(IPage* page);

   private:
    std::vector<IPage*> _pageStack;
    static PageManager* _instance;
};
