#include "main_window.h"
#include "isearchcriteria.h"
#include "result.h"
#include "resulttable.h"
#include "grep.h"

#include <FL/Fl_Native_File_Chooser.H>


#include <iostream>
#include <unistd.h>

#include <pthread.h>

using namespace std;


typedef pthread_t Fl_Thread;
extern "C" {
  typedef void *(Fl_Thread_Func)(void *);
}

static int fl_create_thread(Fl_Thread& t, Fl_Thread_Func* f, void* p) {
  return pthread_create((pthread_t*)&t, 0, f, p);
}

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

void awakeCallback(void *userdata) {
    //runs in main thread
    _btnStart->activate();
}

void onCommandCreated(const char* pMsg) {
    //runs in worker threads
    fprintf(stderr, "command: %s\n", pMsg);
    Fl::lock();
    _outCommand->value(pMsg);
    Fl::unlock();
    Fl::awake();
}

void onGrepFinished(ResultPtr pResult) {
    
}

Result g_result;

extern "C" void* nextSearch(void*) {
    //runs in worker threads
    ISearchCriteria crit;
    provideSearchCriteria(crit);
    Grep grep(crit);
    grep.registerCommandCreatedCallback(onCommandCreated);
    grep.registerSearchFinishedCallback(onGrepFinished);
    const Result& result = grep.search();
    g_result = result;
    Fl::lock();
    _table->setResult(g_result);
    Fl::unlock();
    Fl::awake(awakeCallback, NULL);
    return NULL;
}

Fl_Thread next_thread;
void onStartSuche(Fl_Widget*, void*) {
    if(isEmpty(_txtSuchtext->value())) {
        fl_alert("Es ist kein zu suchender Text eingegeben.");
        return;
    }
    if(isEmpty(_txtSuchVerzeichnis->value())) {
        fl_alert("Es ist kein Suchverzeichnis eingegeben.");
        return;
    }
    
    _btnStart->deactivate();
    
    fl_create_thread(next_thread, nextSearch, NULL);
 
}

void onCancel(Fl_Widget*, void*) {
    /* Cancel THREAD immediately or at the next possibility.  */
    int rc =  pthread_cancel((pthread_t) next_thread);
    string msg = (rc != 0) ? "Abbrechen der Suche nicht erfolgreich." :
                             "Suche abgebrochen.";
    _outStatus->value(msg.c_str());
    _btnStart->activate();
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


#include "searchcontroller.h"

int main() {
    //Fl::scheme("gtk+");
    Fl::scheme("gleam");
    
    Fl_Double_Window* pWin = make_window();
    _txtFilePattern->value("*.cpp, *.cxx, *.cc, *.c, *.h, *.hpp, *.hxx");
    char filename[FILENAME_MAX];
    char* resp = getcwd(filename, FILENAME_MAX);
    _txtSuchVerzeichnis->value(filename);
    _btnOpenFileDialog->callback(onOpenFileDialog);
    
    _cbRekursiv->value(1);
    
    //_btnStart->callback(onStartSuche);
    //_btnCancel->callback(onCancel);
    
    _outStatus->value("Bereit.");
    
    SearchController();
    
    pWin->show();
    
    Fl::lock();
    
    return Fl::run();
}
