#include "PageManager.h"

PageManager* PageManager::getInstance() {
    if (!_instance) _instance = new PageManager();
    return _instance;
}

void PageManager::pushPage(IPage* page) {
    if (page) {
        _pageStack.push_back(page);
        page->onEnter();
    }
}

void PageManager::popPage() {
    if (_pageStack.size() > 1) {
        _pageStack.pop_back();
        _pageStack.back()->onEnter(); 
    }
}

IPage* PageManager::getCurrentPage() {
    if (_pageStack.empty()) return nullptr;
    return _pageStack.back();
}

void PageManager::switchPage(IPage* page) {
    _pageStack.clear();
    pushPage(page);
}

PageManager* PageManager::_instance = nullptr;