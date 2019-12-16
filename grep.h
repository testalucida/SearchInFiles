
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

typedef std::function<void(const char*)> GrepCreatedCallback;
typedef std::function<void(Reason)> GrepFinishedCallback;

typedef void (*CommandCreatedCallback)(const char*);
typedef void (*SearchFinishedCallback)(ResultPtr);

class Grep {
public:
    Grep(const ISearchCriteria& crit);
    const Result& search();
    void registerGrepCreatedCallback(GrepCreatedCallback fnc) {
        _grepCreatedCallback = fnc;
    }
    void registerGrepFinishedCallback(GrepFinishedCallback fnc) {
        _finishCallback = fnc;
    }
    void registerCommandCreatedCallback(CommandCreatedCallback pccc) {
        _pccc = pccc;
    }
    void registerSearchFinishedCallback(SearchFinishedCallback psfc) {
        _psfc = psfc;
    }
    void cancel();
    ~Grep() {}
private:
    void prepareCommand();
    void provideFilePattern();
    void createResult(const std::string& grepresult);
private:
    const ISearchCriteria& _crit;
    std::string _command;
    FILE* _pPipe = NULL;
    Result _result;
    GrepCreatedCallback _grepCreatedCallback;
    GrepFinishedCallback _finishCallback;
    CommandCreatedCallback _pccc = NULL;
    SearchFinishedCallback _psfc = NULL;
};

#endif /* GREP_H */

