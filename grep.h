
/* 
 * File:   grep.h
 * Author: martin
 *
 * Created on 14. Dezember 2019, 10:59
 */

#ifndef GREP_H
#define GREP_H

#include "isearchcriteria.h"
#include "result.h"

#include <string>
#include <functional>
using namespace std::placeholders;

enum Reason {
    SEARCH_FINISHED,
    SEARCH_CANCELLED
};

//typedef std::function<void(const char*)> GrepCreatedCallback;
//typedef std::function<void(Reason)> GrepFinishedCallback;
//
//typedef void (*CommandCreatedCallback)(const char*);
//typedef void (*PipeCreatedCallback)(FILE*);
//typedef void (*SearchFinishedCallback)(ResultPtr);

class Grep {
public:
    bool stop = false;
    Grep(const ISearchCriteria& crit, int pipefd);
    void /*const Result&*/ search();
    static std::string prepareCommand( const ISearchCriteria& crit );
    static void ltrim( std::string &s );
    ~Grep() {}
private:
    //void prepareCommand();
//    void provideFilePattern();
 
private:
    const ISearchCriteria& _crit;
    int _pipefd;
    std::string _command;
    FILE* _pPipe = NULL;
};

#endif /* GREP_H */

