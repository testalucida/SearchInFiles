#include "main_window.h"
#include "isearchcriteria.h"
#include "result.h"
#include "resulttable.h"
#include "grep.h"

#include <FL/Fl_Native_File_Chooser.H>

#include <iostream>
#include <unistd.h>

using namespace std;

void onOpenFileDialog(Fl_Widget*, void*) {
    Fl_Native_File_Chooser native;
    native.title("Startverzeichnis auswählen");
    //native.directory(G_filename->value());
    native.type(Fl_Native_File_Chooser::BROWSE_DIRECTORY);
    // Show native chooser
    switch ( native.show() ) {
      case -1: /*ERROR -- todo*/; break;	// ERROR
      case  1: /*CANCEL*/; fl_beep(); break;    // CANCEL
      default: 				// PICKED DIR
        if ( native.filename() ) {
            /*provide searchpath with selected directory*/
            _txtSuchVerzeichnis->value(native.filename());
        } else {
          /* do nothing */
        }
        break;
    }
}

bool isEmpty(const char* pStr) {
    for(const char* p = pStr; *p; p++) {
        if(*p > ' ') return false;
    }
    return true;
}

ISearchCriteria& provideSearchCriteria(ISearchCriteria& crit) {
    crit.searchText = _txtSuchtext->value();
    crit.isRegex = (_cbRegex->value() > 0);
    crit.ignoreCase = (_cbCaseSensitiv->value() < 1);
    crit.matchWord = (_cbMatchWord->value() > 0);
    crit.filepattern = _txtFilePattern->value();
    crit.searchInBinaries = (_cbIncludeBinaries->value() > 0);
    crit.startDir = _txtSuchVerzeichnis->value();
    crit.searchRecursive = (_cbRekursiv->value() > 0);
    return crit;
}

Result g_result;

void onStartSuche(Fl_Widget*, void*) {
    if(isEmpty(_txtSuchtext->value())) {
        fl_alert("Es ist kein zu suchender Text eingegeben.");
        return;
    }
    if(isEmpty(_txtSuchVerzeichnis->value())) {
        fl_alert("Es ist kein Suchverzeichnis eingegeben.");
        return;
    }
    
    ISearchCriteria crit;
    provideSearchCriteria(crit);
    Grep grep(crit);
    const Result& result = grep.search();
    g_result = result;
    _table->setResult(g_result);
}

void onCancel(Fl_Widget*, void*) {
    
}

void testGrep() {
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
     *   => SearchInFiles/main.cpp:    _btnStart->callback(onStartSuche);
     */
    
    const char* cmd = "grep -r --include=*.cpp --include=*.h 'onStartSuche' /home/martin/Projects/cpp/SearchInFiles";
   
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    
    fprintf(stderr, "result: %s\n", result.c_str());
    
}

int main() {
//    testGrep();
//    return 0;
    
    //Fl::scheme("gtk+");
    Fl::scheme("gleam");
    
    Fl_Double_Window* pWin = make_window();
    _txtFilePattern->value("*.cpp, *.cxx, *.cc, *.c, *.h, *.hpp, *.hxx");
    char filename[FILENAME_MAX];
    getcwd(filename, FILENAME_MAX);
    _txtSuchVerzeichnis->value(filename);
    _btnOpenFileDialog->callback(onOpenFileDialog);
    
    _cbRekursiv->value(1);
    
    _btnStart->callback(onStartSuche);
    _btnCancel->callback(onCancel);
    
    _outStatus->value("Bereit.");
    
    pWin->show();
    return Fl::run();
}
