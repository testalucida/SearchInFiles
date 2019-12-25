#include "main_window.h"
#include "isearchcriteria.h"
#include "result.h"
#include "resulttable.h"
#include "grep.h"
#include "searchcontroller.h"

#include "my/StringHelper.h"

#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Table.H>
#include <FL/Fl_Select_Browser.H>

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>

using namespace std;

#define READ_END 0
#define WRITE_END 1

pid_t cpid = -1;
pthread_t workerthread;
int pipefds[2];

void report_and_exit(const char* msg) { //report and exit
    perror(msg);
    exit(-1);
    /** failure **/
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

FILE* g_pPipe;
void onPipeCreated(FILE* pPipe) {
    cerr << "pipe created.\n";
    g_pPipe = pPipe;
}

//****************  CHILD PROCESS  ***********************

void die(int signum) {
    printf("\tChild confirming received signal: %i\n", signum);
    puts("\tChild about to terminate gracefully...");
    sleep(1);
    puts("\tChild terminating now...");
    _exit(0); /* fast-track notification of parent */
}

void set_kill_handler() {
    struct sigaction current;
    sigemptyset(&current.sa_mask);
    /* clear the signal set */
    current.sa_flags = 0;
    /* enables setting sa_handler, not sa_action */
    current.sa_handler = die;
    /* specify a handler */
    sigaction(SIGTERM, &current, NULL);
    /* register the handler */
}

//Grep* g_pGrep;
Result g_result;
void search(int pipefd) {
    //child process running in workerthread
    
    set_kill_handler();
    
    ISearchCriteria crit;
    provideSearchCriteria(crit);
    Grep grep(crit, pipefd);
  
    grep.registerCommandCreatedCallback(onCommandCreated);
    grep.search();
    //grep.registerPipeCreatedCallback(onPipeCreated);
    //grep.registerSearchFinishedCallback(onGrepFinished);
//    const Result& result = grep.search();
//    g_result = result;
//    Fl::lock();
//    _table->setResult(g_result);
//    Fl::unlock();
//    Fl::awake(awakeCallback, NULL);
}
//****************  END CHILD PROCESS  ***********************

void* createPipeAndFork( void* ) {
    //workerthread's start function: 
    //establish communication pipe and create child process
    //call code to be executed in child process (search(..))
    
    //create pipe:
    if(pipe(pipefds) < 0) {
        report_and_exit("pipe creation failed.");
    }
    //fork:
    if( (cpid = fork()) < 0 ) {
        report_and_exit("fork failed.");
    }
    
    if(cpid == 0) {
        //CHILD
        //close pipe for reading, child is only writing
        close(pipefds[READ_END]);
        
        //child process
        //the search result is written into the pipe which is read by
        //the main process.
        search(pipefds[WRITE_END]);
        cpid = -1;
        fprintf(stderr, "search ended, leaving child process.\n");
        _exit(0);
    } else {
        //PARENT
        //close pipe for writing, parent is only reading
        close(pipefds[WRITE_END]);
        
        //setting new group id - needed in case of process cancelling
        setpgid(cpid, cpid);
        
        //we wait 'til child's termination...
        wait(NULL);
        cpid = -1; //child process doesn't exist any more
        //...and then read search result from pipe:
        char buf; // 1 byte read buffer
        string resultstring;
        while( read(pipefds[READ_END], &buf, 1) > 0 ) {
            resultstring += buf;
        }
        
        close(pipefds[READ_END]);
        
//        fprintf(stderr, 
//                "Result from communication pipe read in main process:\n%s\n", 
//                resultstring.c_str());
        Result* pResult = new Result(resultstring);
        Fl::lock();
        _table->setResult(pResult);
        Fl::unlock();
        Fl::awake(awakeCallback, NULL);
        fprintf(stderr, "workerthread terminating.\n");
    }
}

void onStartSuche(Fl_Widget*, void*) {
    //validates input and 
    //create new thread which in turn creates search process
    if(isEmpty(_txtSuchtext->value())) {
        fl_alert("Es ist kein zu suchender Text eingegeben.");
        return;
    }
    if(isEmpty(_txtSuchVerzeichnis->value())) {
        fl_alert("Es ist kein Suchverzeichnis eingegeben.");
        return;
    }
    
    _btnStart->deactivate();
    
    void* p = NULL;
    int rc = pthread_create(&workerthread, 0, createPipeAndFork, p);
    if (rc != 0) {
        fprintf(stderr, "Couldn't create thread." );
    }
 
}

void cancelWorkerThread() {
    int rc =  pthread_cancel(workerthread);
    string msg = (rc != 0) ? "Abbrechen der Suche nicht erfolgreich." :
                                 "Suche abgebrochen.";
    _outStatus->value(msg.c_str());
    _btnStart->activate();
    _btnCancel->activate();
}

void onCancel(Fl_Widget*, void*) {
    if( cpid > 0 ) {
        int rc;
        _btnCancel->deactivate();
        rc = killpg(cpid, SIGTERM);
        if (-1 == rc) {
            report_and_exit("kill failed");
        }
        wait(NULL); /** wait for child to terminate **/
        puts("My child terminated, about to exit myself...");
        cpid = -1;
       
        
        cancelWorkerThread();
    }
}

void showInBrowser(const char* pPathnfile, int line) {
   Fl_Select_Browser* pBrowser = 
           new Fl_Select_Browser(500, 150, 300, 350, pPathnfile);
   pBrowser->type(FL_MULTI_BROWSER);
   pBrowser->load(pPathnfile);
   pBrowser->select(line);
}

void onTableRowClick(Fl_Widget*, void* p) {
    if(Fl::event_clicks()) { //double click
        int evt = (int)Fl::event();
        if(evt == 2) {
            ResultTable* pTable = ((ResultTable*)p);
            Fl_Table::TableContext context = pTable->callback_context();
            if(context == Fl_Table::CONTEXT_CELL) {
                int r = pTable->callback_row(),
                    c = pTable->callback_col();
                
                const char* val = pTable->getResult()->get(r);
                vector<string> parts;
                my::StringHelper::instance().tokenize(val, ':', parts);
                //fprintf(stderr, "onTableRowClick: r = %d, c = %d\n", r, c);
                if(parts.size() != 3) {
                    //fl_alert("onTableRowClick: tokenizing failed.\n");
                    //return;
                }
                int line = atoi(parts[1].c_str());
                fprintf(stderr, "line: %d\n", line);
                fprintf(stderr, "path: %s\n", parts[0].c_str());
                showInBrowser(parts[0].c_str(), line);
            }

        }
    }
    
}


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
    
    _btnStart->callback(onStartSuche);
    _btnCancel->callback(onCancel);
    _table->callback(onTableRowClick, _table);
    
    _outStatus->value("Bereit.");
    
    //SearchController();
    
    pWin->show();
    
    Fl::lock();
    
    return Fl::run();
}
