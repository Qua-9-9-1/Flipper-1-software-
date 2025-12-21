#include "PageManager.h"

PageManager::PageManager() {}

PageManager* PageManager::getInstance() {
    if (!instance) instance = new PageManager();
    return instance;
}

void PageManager::pushPage(IPage* page) {
    if (page) {
        pageStack.push_back(page);
        page->onEnter();
    }
}

void PageManager::popPage() {
    if (pageStack.size() > 1) {
        pageStack.pop_back();
        pageStack.back()->onEnter(); 
    }
}

IPage* PageManager::getCurrentPage() {
    if (pageStack.empty()) return nullptr;
    return pageStack.back();
}

void PageManager::switchPage(IPage* page) {
    pageStack.clear();
    pushPage(page);
}

PageManager* PageManager::instance = nullptr;