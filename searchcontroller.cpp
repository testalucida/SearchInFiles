#include "searchcontroller.h"

#include "main_window.h"

#include <FL/fl_ask.H>

#include <iostream>

using namespace std;

static int create_thread(Thread& t, Thread_Func* f, void* p) {
  return pthread_create((pthread_t*)&t, 0, f, p);
}

extern "C" void* nextSearch_func(void* pThis) {
    ((SearchController*)pThis)->nextSearch();
}

SearchController::SearchController() {
    _btnStart->callback(onStartSearch_stat, this);
    _btnCancel->callback(onCancelSearch_stat, this);
}

void SearchController::onStartSearch() {
    if(isEmpty(_txtSuchtext->value())) {
        fl_alert("Es ist kein zu suchender Text eingegeben.");
        return;
    }
    if(isEmpty(_txtSuchVerzeichnis->value())) {
        fl_alert("Es ist kein Suchverzeichnis eingegeben.");
        return;
    }
    
    _btnStart->deactivate();
    
    if( _threadIdx < MAX_THREADS - 1) {
        ++_threadIdx;
        create_thread(_threads[_threadIdx], nextSearch_func, this);
    }
}

void SearchController::nextSearch() {
    cerr << "hier next search";
}

void SearchController::onCancelSearch() {
    
}

bool SearchController::isEmpty(const char* pStr) {
    for(const char* p = pStr; *p; p++) {
        if(*p > ' ') return false;
    }
    return true;
}