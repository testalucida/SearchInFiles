#include "main_window.h"
#include "browser.h"
#include "browse_match.hpp"
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

Fl_Double_Window* pMainWin;
pid_t cpid = -1;
pthread_t grep_command_thread;
pthread_t workerthread;
pthread_t browserthread;
int pipefds[2];

void report_and_exit(const char* msg) { //report and exit
    perror( msg );
    exit( -1 );
    /** failure **/
}

void onOpenFileDialog(Fl_Widget*, void*) {
    Fl_Native_File_Chooser native;
    native.title( "Startverzeichnis auswählen" );
    //native.directory(G_filename->value());
    native.type( Fl_Native_File_Chooser::BROWSE_DIRECTORY );
    // Show native chooser
    switch (native.show( )) {
    case -1: /*ERROR -- todo*/;
        break; // ERROR
    case 1: /*CANCEL*/;
        fl_beep( );
        break; // CANCEL
    default: // PICKED DIR
        if (native.filename( )) {
            /*provide searchpath with selected directory*/
            _txtSuchVerzeichnis->value( native.filename( ) );
        } else {
            /* do nothing */
        }
        break;
    }
}

ISearchCriteria& provideSearchCriteria(ISearchCriteria& crit) {
    crit.searchText = _txtSuchtext->value( );
    crit.isRegex = (_cbRegex->value( ) > 0);
    crit.ignoreCase = (_cbCaseSensitiv->value( ) < 1);
    crit.matchWord = (_cbMatchWord->value( ) > 0);
    crit.filepattern = _txtFilePattern->value( );
    crit.excludeHiddenFolders = ( _cbHiddenFolders->value() < 1 );
    crit.searchInBinaries = (_cbIncludeBinaries->value( ) > 0);
    crit.startDir = _txtSuchVerzeichnis->value( );
    crit.searchRecursive = (_cbRekursiv->value( ) > 0);
    return crit;
}

void* createGrepCommand( void* p ) {
    //ISearchCriteria* pSearchCrit = (ISearchCriteria*) p;
    ISearchCriteria sc;
    provideSearchCriteria( sc );
    string cmd = Grep::prepareCommand( sc );
    Fl::lock( );
    _outCommand->value( cmd.c_str( ) );
    Fl::unlock( );
    Fl::awake( (void*) NULL );
}

bool isEmpty(const char* pStr) {
    for (const char* p = pStr; *p; p++) {
        if (*p > ' ') return false;
    }
    return true;
}

//****************  CHILD PROCESS  ***********************

void die(int signum) {
    printf( "\tChild confirming received signal: %i\n", signum );
    puts( "\tChild about to terminate gracefully..." );
    sleep( 1 );
    puts( "\tChild terminating now..." );
    _exit( 0 ); /* fast-track notification of parent */
}

void set_kill_handler() {
    struct sigaction current;

    //clear the signal set:
    sigemptyset( &current.sa_mask );

    //enables setting sa_handler, not sa_action:
    current.sa_flags = 0;

    //specify a handler:
    current.sa_handler = die;

    //register the handler:
    sigaction( SIGTERM, &current, NULL );
}

//Grep* g_pGrep;
Result g_result;

void search(int pipefd) {
    //child process running in workerthread

    set_kill_handler( );

    ISearchCriteria crit;
    provideSearchCriteria( crit );
    Grep grep( crit, pipefd );
    grep.search( );

}
//****************  END CHILD PROCESS  ***********************

/**
 * removes leading and trailing rubbish from Grep's result.
 * @param s: result string from Grep. Might contain
 * informations like :::CMD_END::: and :::CONV_END:::.
 * These informations will be removed so that only the 
 * grep result remains.
 */
string& strip(string& s) {
    string cmd_end = ":::CMD_END:::";
    string conv_end = ":::CONV_END:::";
    int posA = s.find( cmd_end );
    if (posA != string::npos) {
        posA += cmd_end.length( );
    } else {
        posA = 0;
    }
    int posB = s.find( conv_end );
    posB = (posB > -1) ? posB : s.length( ) - 1;
    s = s.substr( posA, posB - posA );
    return s;
}

void* createPipeAndFork(void*) {
    //workerthread's start function: 
    //establish communication pipe and create child process
    //call code to be executed in child process (search(..))

    //create pipe:
    if (pipe( pipefds ) < 0) {
        report_and_exit( "pipe creation failed." );
    }

    //fork:
    if ((cpid = fork( )) < 0) {
        report_and_exit( "fork failed." );
    }

    if (cpid == 0) {
        //CHILD
        //close pipe for reading, child is only writing
        close( pipefds[READ_END] );

        //child process
        //the search result is written into the pipe which is read by
        //the main process.
        cerr << "CHILD: before search()" << endl;
        search( pipefds[WRITE_END] );
        cerr << "CHILD: search ended, leaving child process" << endl;
        _exit( 0 );
    } else {
        //PARENT
        //close pipe for writing, parent is only reading
        close( pipefds[WRITE_END] );

        //setting new group id - needed in case of process cancelling
        setpgid( cpid, cpid );

        /********************************************************************/

        char buf[1];
        string received;
        while (1) {
            while (read( pipefds[READ_END], &buf, 1 ) > 0) {
                received += buf;
            }
            if (received.length( ) > 0) {
                cerr << "PARENT: read from pipe: " << received.c_str( ) << endl;

                int posS, posE;
                if ((posS = received.find( ":::CMD:::" )) != string::npos) {
                    if ((posE = received.find( ":::CMD_END:::", posS + 1 )) !=
                            string::npos) {
                        posS += strlen( ":::CMD:::" );
                        int len = posE - posS;
                        string command = received.substr( posS, len );
                        Fl::lock( );
                        _outCommand->value( command.c_str( ) );
                        Fl::unlock( );
                        Fl::awake( (void*) NULL );
                    } else {
                        throw
                        std::runtime_error( ":::CMD::: without :::CMD_END:::" );
                    }
                }
                if (received.find( ":::CONV_END:::" ) != string::npos) {
                    //end while loop, child is finished
                    break;
                }

                received.clear( );
            }

        } //while

        close( pipefds[READ_END] );

        /********************************************************************/

        Result* pResult = new Result( strip( received ) );
        Fl::lock( );
        _table->setResult( pResult );
        _btnStart->activate( );
        pMainWin->cursor( Fl_Cursor::FL_CURSOR_ARROW );
        Fl::unlock( );
        Fl::awake( (void*) NULL );

        wait( NULL ); //wait for end of child process
        cpid = -1;
        cerr << "PARENT: end of process" << endl;
    }
}

void clearResult() {
    _outCommand->value( "" );
    _table->clearResult( );
}

void onStartSuche(Fl_Widget*, void*) {
    _outStatus->value( "Bereit." );
    _outCommand->value( "" );
    cerr << "outCommand cleared? --> " << _outCommand->value() << endl; 
    _table->clearResult( );
    //validates input and 
    //create new thread which in turn creates search process
    if (isEmpty( _txtSuchtext->value( ) )) {
        fl_alert( "Es ist kein zu suchender Text eingegeben." );
        return;
    }
    if (isEmpty( _txtSuchVerzeichnis->value( ) )) {
        fl_alert( "Es ist kein Suchverzeichnis eingegeben." );
        return;
    }

    _btnStart->deactivate( );
    pMainWin->cursor( Fl_Cursor::FL_CURSOR_WAIT );
    
    void* p = NULL;
    int rc = pthread_create( &grep_command_thread, 0, createGrepCommand, p );
    if (rc != 0) {
        fprintf( stderr, "Couldn't create grep command thread." );
    }
    
    rc = pthread_create( &workerthread, 0, createPipeAndFork, p );
    if (rc != 0) {
        fprintf( stderr, "Couldn't create pipe and fork thread." );
    }

}

void cancelWorkerThread() {
    int rc = pthread_cancel( workerthread );
    string msg = (rc != 0) ? "Abbrechen der Suche nicht erfolgreich." :
            "Suche abgebrochen.";
    _outStatus->value( msg.c_str( ) );
    _btnStart->activate( );
    _btnCancel->activate( );
    pMainWin->cursor( Fl_Cursor::FL_CURSOR_ARROW );
    clearResult( );
}

void onCancel(Fl_Widget*, void*) {
    if (cpid > 0) {
        int rc;
        _btnCancel->deactivate( );
        rc = killpg( cpid, SIGTERM );
        if (-1 == rc) {
            report_and_exit( "kill failed" );
        }
        wait( NULL ); /** wait for child to terminate **/
        puts( "My child terminated, about to exit myself..." );
        cpid = -1;

        cancelWorkerThread( );
    }
    
    _outStatus->value( "Bereit." );
    _outCommand->value( "" );
    _table->clearResult();
}

void browserCallback(Fl_Widget* pWin, void*) {
    MatchBrowserWindow* p = (MatchBrowserWindow*) pWin;
    p->hide();
    //delete p;
    /*
    Fl::lock();
    p->hide( );
    //Fl::delete_widget( p );
    Fl::unlock();
    Fl::awake((void*)NULL);
    */
}

void closeButtonCallback(Fl_Widget* pBtn, void*) {
    Fl_Button* p = (Fl_Button*) pBtn;
    p->parent()->hide();
}

void showInBrowser(const char* pPathnfile, int line) {
    MatchBrowserWindow* pWin = make_browser( );
    pWin->callback( browserCallback );
    pWin->position( pMainWin->x( ) + pMainWin->w( ), pMainWin->y( ) + 100 );
//    _pBtnClose->callback( closeButtonCallback );
    _pBrowser->type( FL_MULTI_BROWSER );
    _pBrowser->load( pPathnfile );
    _pBrowser->select( line );
    _pBrowser->topline( line );
    pWin->show( );

}

/*
void* startBrowserThread( void* args ) {
    Fl::lock();
    showInBrowser((const char*)args, 10);
    Fl::unlock();
    Fl::awake((void*)NULL);
}
*/

void onTableRowClick(Fl_Widget*, void* p) {
    if (Fl::event_clicks( )) { //double click
        int evt = (int) Fl::event( );
        if (evt == 2) {
            ResultTable* pTable = ((ResultTable*) p);
            Fl_Table::TableContext context = pTable->callback_context( );
            if (context == Fl_Table::CONTEXT_CELL) {
                int r = pTable->callback_row( ),
                        c = pTable->callback_col( );

                const char* val = pTable->getResult( )->get( r );
                vector<string> parts;
                my::StringHelper::instance( ).tokenize( val, ':', parts );
                //fprintf(stderr, "onTableRowClick: r = %d, c = %d\n", r, c);
                if (parts.size( ) != 3) {
                    //fl_alert("onTableRowClick: tokenizing failed.\n");
                    //return;
                }
                int line = atoi( parts[1].c_str( ) );
                fprintf( stderr, "line: %d\n", line );
                fprintf( stderr, "path: %s\n", parts[0].c_str( ) );
                /*
                int rc = 
                pthread_create( &browserthread, 0, startBrowserThread, 
                        (void*)parts[0].c_str( ) );
                if (rc != 0) {
                    fprintf( stderr, "Couldn't create browser thread." );
                }
                */
                showInBrowser( parts[0].c_str( ), line );
            }

        }
    }

}

/*
 TODO:
 * Status bei neuem Suchvorgang zurücksetzen
 * "Kein Treffer" in der Result-Table löschen bei neuem Suchvorgang
 * Den Suchfortschritt im Status anzeigen - man erkennt sonst nicht,
 * ob eine Suche noch läuft oder eingefroren ist.
 * Mauszeiger auf busy stellen, wenn Start gedrückt wurde
 */


int main() {
    //Fl::scheme("gtk+");
    Fl::scheme( "gleam" );

    pMainWin = make_window( );
    _txtFilePattern->value( "*.cpp, *.cxx, *.cc, *.c, *.h, *.hpp, *.hxx" );
    char filename[FILENAME_MAX];
    char* resp = getcwd( filename, FILENAME_MAX );
    _txtSuchVerzeichnis->value( filename );
    _btnOpenFileDialog->callback( onOpenFileDialog );

    _cbRekursiv->value( 1 );

    _btnStart->callback( onStartSuche );
    _btnCancel->callback( onCancel );
    _table->callback( onTableRowClick, _table );

    _outStatus->value( "Bereit." );

    //SearchController();

    pMainWin->show( );

    Fl::lock( );

    return Fl::run( );
}
