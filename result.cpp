#include "result.h"

using namespace std;

void Result::set(std::vector<std::string> matches) {
    for(auto match : matches) {
        _matches.push_back(match);
    }
}

const char* Result::get(int row) const {
    //string match = _matches.at(row);
    //return match.c_str();
    return _matches.at(row).c_str();
}
