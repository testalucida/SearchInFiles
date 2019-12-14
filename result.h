
/* 
 * File:   result.h
 * Author: martin
 *
 * Created on 14. Dezember 2019, 11:38
 */

#ifndef RESULT_H
#define RESULT_H

#include <string>
#include <vector>

class Result {
public:
    Result() {}
    void add(const char* s) {_matches.push_back(s);}
    void set(std::vector<std::string> matches);
    const char* get(int row) const;
    int getCount() const {return _matches.size();}
private:
    std::vector<std::string> _matches;
};


#endif /* RESULT_H */

