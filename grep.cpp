#include "grep.h"

#include <my/StringHelper.h>

#include <iostream>
#include <unistd.h>
#include <algorithm>

using namespace std;

Grep::Grep(const ISearchCriteria& crit) 
: _crit(crit)
{    
}

const Result& Grep::search() {
    prepareCommand();
    fprintf(stderr, "command: %s\n", _command.c_str());
    
    char buffer[128];
    string grepresult = "";
    
    FILE* pipe = popen(_command.c_str(), "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            grepresult += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    
//    fprintf(stderr, "grepresult: %s\n", grepresult.c_str());
    
    createResult(grepresult);
    return _result;
}

void Grep::prepareCommand() {
        /*
     -i  ignore case
     -c  print only a count of selected lines per FILE
     -r  recursive
     -e, --regexp=MUSTER       MUSTER als regulären Ausdruck verwenden
     -w, --word-regexp         MUSTER passt nur auf ganze Wörter
     -x, --line-regexp         MUSTER passt nur auf ganze Zeilen

     --include=DATEIMUSTER  durchsucht nur Dateien, die DATEIMUSTER entsprechen
     --exclude=DATEIMUSTER  überspringt Dateien und Verzeichnisse, die 
                              DATEIMUSTER entsprechen
     --exclude-from=DATEI   überspringt Dateien, die einem Dateimuster
                              in DATEI entsprechen.
     --exclude-dir=MUSTER   Verzeichnisse, die MUSTER entsprechen, 
                              werden übersprungen
     -l                     print only names of FILEs with selected lines
                              (not the matching file line)

     * 
     * Beispiel:
     * grep -r --include=*.{cpp,h} "onStartSuche"
         * * Output: SearchInFiles/main.cpp:    _btnStart->callback(onStartSuche);
         * NB: diese Schreibweise: *.{cpp,h} ...funktioniert nur im Terminal. 
         * Im Programm muss für jedes Pattern ein --include= Statement 
         * generiert werden. 
     
     */
    
    //const char* cmd = "grep -r --include=*.cpp --include=*.h 'onStartSuche' /home/martin/Projects/cpp/SearchInFiles";
    
    _command = "grep ";
    if(_crit.isRegex) _command += "-e "; 
    if(_crit.matchWord) _command += "-w ";
    if(_crit.searchRecursive) _command += "-r ";
    if(_crit.ignoreCase) _command += "-i ";
    if(_crit.searchInBinaries) {
        _command += "--text ";
    } else {
        _command += "--binary-files=without-match ";
    }
    
    provideFilePattern();
    
    _command += ("\"" + _crit.searchText + "\" ");
    _command += _crit.startDir;
}

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

void Grep::provideFilePattern() {
    vector<string> patterns;
    my::StringHelper::instance().
        tokenize(_crit.filepattern.c_str(), ',', patterns);
    for(auto& pattern : patterns) {
        ltrim(pattern);
        _command += ("--include=\"" + pattern + "\" ");
    }
}

void Grep::createResult(const std::string& grepresult) {
    vector<string> matches;
    my::StringHelper::instance().tokenize(grepresult.c_str(), '\n', matches);
    _result.set(matches);
    if(_result.getCount() == 0) _result.add("Kein Treffer.");
}


