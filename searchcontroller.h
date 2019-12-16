
/* 
 * File:   searchcontroller.h
 * Author: martin
 *
 * Created on 16. Dezember 2019, 11:48
 */

#ifndef SEARCHCONTROLLER_H
#define SEARCHCONTROLLER_H

#include <vector>

#include <pthread.h>

typedef pthread_t Thread;
extern "C" {
    typedef void *(Thread_Func)(void *);
}


class Fl_Widget;

class SearchController {
public:
    SearchController();
    ~SearchController() {}
    static void onStartSearch_stat(Fl_Widget*, void* pThis) {
        ((SearchController*)pThis)->onStartSearch();
    }
    void onStartSearch();
    void nextSearch();
    static void onCancelSearch_stat(Fl_Widget*, void* pThis) {
        ((SearchController*)pThis)->onCancelSearch();
    }
    void onCancelSearch();
private:
    bool isEmpty(const char* pStr);
private:
    int _threadIdx = 0;
    static const int MAX_THREADS = 4;
    Thread _threads[MAX_THREADS];
};

#endif /* SEARCHCONTROLLER_H */

