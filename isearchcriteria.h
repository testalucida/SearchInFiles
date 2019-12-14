

/* 
 * File:   isearchcriteria.h
 * Author: martin
 *
 * Created on 13. Dezember 2019, 19:59
 */

#ifndef ISEARCHCRITERIA_H
#define ISEARCHCRITERIA_H

#include <string>

struct ISearchCriteria {
    std::string searchText;
    bool isRegex = false;
    bool ignoreCase = true;
    bool matchWord = false;
    std::string filepattern; // *.cpp, *.cc, *.h, ...
    bool searchInBinaries = false;
    std::string startDir;
    bool searchRecursive = true;
};


#endif /* ISEARCHCRITERIA_H */

