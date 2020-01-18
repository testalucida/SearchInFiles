#include "grep.h"

#include <my/StringHelper.h>

#include <iostream>
#include <unistd.h>
#include <algorithm>

using namespace std;

Grep::Grep(const ISearchCriteria& crit, int pipefd)
: _crit(crit), _pipefd(pipefd)
{
}

void /*const Result&*/ Grep::search()
{
    if( _command.empty() ) {
        _command = prepareCommand( _crit );
        string command = ":::CMD:::";
        command.append( _command.c_str( ) ).append( ":::CMD_END:::" );
        write( _pipefd, command.c_str( ), command.length( ) );
    }

    _pPipe = popen( _command.c_str( ), "r" );
    if (!_pPipe) throw std::runtime_error( "popen() failed!" );
    
    char buffer[128];
    string grepresult = "";
    try {
        while (fgets( buffer, sizeof buffer, _pPipe ) != NULL) {
            grepresult += buffer;
        }
    } catch (...) {
        pclose( _pPipe );
        throw;
    }
    pclose( _pPipe );

    //write grepresult to communication pipe:
    write( _pipefd, grepresult.c_str( ), grepresult.length( ) );
    string endmarker = ":::CONV_END:::";
    write( _pipefd, endmarker.c_str( ), endmarker.length( ) );
    
    //return _result;
}

string Grep::prepareCommand( const ISearchCriteria& crit )
{
    /*
 -i  ignore case
 -c  print only a count of selected lines per FILE
 -n  Zeilennummer ausgeben
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

    string command = "grep -n ";
    if (crit.isRegex) command += "-e ";
    if (crit.matchWord) command += "-w ";
    if (crit.searchRecursive) command += "-r ";
    if (crit.ignoreCase) command += "-i ";
    if (crit.searchInBinaries) {
        command += "--text ";
    } else {
        command += "--binary-files=without-match ";
    }
    
    //provide file pattern:
    vector<string> patterns;
    my::StringHelper::instance( ).
            tokenize( crit.filepattern.c_str( ), ',', patterns );
    for (auto& pattern : patterns) {
        ltrim( pattern );
        command += ("--include=\"" + pattern + "\" ");
    }
    
    if( crit.excludeHiddenFolders ) {
        command += "--exclude-dir='.*' ";
    } else {
        //exclude .git anyways
        command += "--exclude-dir='.git' ";
    }

    command += ("\"" + crit.searchText + "\" ");
    command += crit.startDir;
    
    return command;
}

// trim from start (in place)
void Grep::ltrim( string &s ) {
    s.erase( s.begin( ), std::find_if( s.begin( ), s.end( ), [](int ch)
    {
        return !std::isspace( ch );
    } ) );
}

//void Grep::provideFilePattern()
//{
//    
//}
