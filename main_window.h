// generated by Fast Light User Interface Designer (fluid) version 1,0400

#ifndef main_window_h
#define main_window_h
#include <FL/Fl.H>
#include "resulttable.h"
#include <FL/Fl_Double_Window.H>
extern Fl_Double_Window *main_win;
#include <FL/Fl_Input.H>
extern Fl_Input *_txtSuchtext;
extern Fl_Input *_txtSuchVerzeichnis;
#include <FL/Fl_Return_Button.H>
extern Fl_Return_Button *_btnStart;
#include <FL/Fl_Button.H>
extern Fl_Button *_btnCancel;
#include <FL/Fl_Check_Button.H>
extern Fl_Check_Button *_cbRegex;
extern Fl_Button *_btnOpenFileDialog;
extern Fl_Check_Button *_cbRekursiv;
extern Fl_Input *_txtFilePattern;
extern Fl_Check_Button *_cbMatchWord;
#include <FL/Fl_Group.H>
extern Fl_Group *_grpResults;
extern ResultTable *_table;
#include <FL/Fl_Box.H>
extern Fl_Check_Button *_cbCaseSensitiv;
extern Fl_Check_Button *_cbIncludeBinaries;
#include <FL/Fl_Output.H>
extern Fl_Output *_outStatus;
extern Fl_Output *_outCommand;
extern Fl_Check_Button *_cbHiddenFolders;
Fl_Double_Window* make_window();
#endif
