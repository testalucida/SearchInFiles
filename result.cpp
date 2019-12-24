#include "result.h"
#include <my/StringHelper.h>

using namespace std;

Result::Result(std::string resultstring) {
    vector<string> matches;
    my::StringHelper::instance().tokenize(resultstring.c_str(), '\n', matches);
    set(matches);
    if(getCount() == 0) add("Kein Treffer.");
}

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
