
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

class Result;

class Grep {
public:
    Grep(const ISearchCriteria& crit);
    const Result& search();
    ~Grep() {}
private:
    void prepareCommand();
    void provideFilePattern();
    void createResult(const std::string& grepresult);
private:
    const ISearchCriteria& _crit;
    std::string _command;
    Result _result;
};

#endif /* GREP_H */

