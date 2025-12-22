#pragma once
#include <vector>
#include "Page.h"

class PageManager {    
    public:
        PageManager() {};
        static PageManager* getInstance();
        void pushPage(IPage* page);
        void popPage();
        IPage* getCurrentPage();
        void switchPage(IPage* page); // Go to page whithout history
    private:
        std::vector<IPage*> pageStack;
        static PageManager* instance;
};
