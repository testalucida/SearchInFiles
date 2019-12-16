// generated by Fast Light User Interface Designer (fluid) version 1,0400

#include "main_window.h"

Fl_Input *_txtSuchtext=(Fl_Input *)0;

Fl_Input *_txtSuchVerzeichnis=(Fl_Input *)0;

Fl_Return_Button *_btnStart=(Fl_Return_Button *)0;

Fl_Button *_btnCancel=(Fl_Button *)0;

Fl_Check_Button *_cbRegex=(Fl_Check_Button *)0;

Fl_Button *_btnOpenFileDialog=(Fl_Button *)0;

Fl_Check_Button *_cbRekursiv=(Fl_Check_Button *)0;

Fl_Input *_txtFilePattern=(Fl_Input *)0;

Fl_Check_Button *_cbMatchWord=(Fl_Check_Button *)0;

Fl_Group *_grpResults=(Fl_Group *)0;

ResultTable *_table=(ResultTable *)0;

Fl_Check_Button *_cbCaseSensitiv=(Fl_Check_Button *)0;

Fl_Check_Button *_cbIncludeBinaries=(Fl_Check_Button *)0;

Fl_Output *_outStatus=(Fl_Output *)0;

Fl_Output *_outCommand=(Fl_Output *)0;

Fl_Double_Window* make_window() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = new Fl_Double_Window(600, 600, "Textsuche in Dateien");
    w = o; if (w) {/* empty */}
    o->color((Fl_Color)38);
    o->labelcolor((Fl_Color)46);
    { _txtSuchtext = new Fl_Input(139, 20, 366, 25, "Suche Text: ");
      _txtSuchtext->tooltip("Gib hier zu suchenden Text oder einen regul\303\244ren Ausdruck ein");
      _txtSuchtext->box(FL_FLAT_BOX);
      _txtSuchtext->selection_color((Fl_Color)40);
      _txtSuchtext->labelfont(1);
      _txtSuchtext->labelcolor((Fl_Color)53);
      _txtSuchtext->textfont(1);
    } // Fl_Input* _txtSuchtext
    { _txtSuchVerzeichnis = new Fl_Input(140, 191, 366, 25, "in Verzeichnis: ");
      _txtSuchVerzeichnis->box(FL_FLAT_BOX);
      _txtSuchVerzeichnis->labelfont(1);
      _txtSuchVerzeichnis->labelcolor((Fl_Color)53);
      _txtSuchVerzeichnis->textfont(1);
    } // Fl_Input* _txtSuchVerzeichnis
    { _btnStart = new Fl_Return_Button(12, 260, 130, 25, "Suche starten");
      _btnStart->box(FL_THIN_UP_BOX);
      _btnStart->down_box(FL_DOWN_BOX);
      _btnStart->color((Fl_Color)44);
      _btnStart->labelfont(1);
      _btnStart->labelsize(12);
      _btnStart->labelcolor((Fl_Color)55);
    } // Fl_Return_Button* _btnStart
    { _btnCancel = new Fl_Button(148, 260, 130, 25, "Abbrechen");
      _btnCancel->box(FL_THIN_UP_BOX);
      _btnCancel->down_box(FL_DOWN_BOX);
      _btnCancel->color((Fl_Color)43);
      _btnCancel->labelfont(1);
      _btnCancel->labelsize(12);
      _btnCancel->labelcolor((Fl_Color)55);
    } // Fl_Button* _btnCancel
    { _cbRegex = new Fl_Check_Button(514, 18, 70, 25, " Regex");
      _cbRegex->tooltip("Kennzeichnet, ob der eingegebene Suchtext ein regul\303\244rer Ausdruck ist");
      _cbRegex->down_box(FL_DOWN_BOX);
      _cbRegex->color(FL_LIGHT2);
      _cbRegex->labelfont(1);
      _cbRegex->labelcolor((Fl_Color)53);
    } // Fl_Check_Button* _cbRegex
    { _btnOpenFileDialog = new Fl_Button(514, 191, 25, 25, "...");
      _btnOpenFileDialog->box(FL_THIN_UP_BOX);
      _btnOpenFileDialog->down_box(FL_DOWN_BOX);
      _btnOpenFileDialog->color((Fl_Color)43);
      _btnOpenFileDialog->labelfont(1);
      _btnOpenFileDialog->labelsize(12);
      _btnOpenFileDialog->labelcolor((Fl_Color)55);
    } // Fl_Button* _btnOpenFileDialog
    { _cbRekursiv = new Fl_Check_Button(140, 221, 70, 25, " rekursiv suchen");
      _cbRekursiv->down_box(FL_DOWN_BOX);
      _cbRekursiv->color(FL_LIGHT2);
      _cbRekursiv->labelfont(1);
      _cbRekursiv->labelcolor((Fl_Color)53);
    } // Fl_Check_Button* _cbRekursiv
    { _txtFilePattern = new Fl_Input(139, 118, 366, 25, "File pattern: ");
      _txtFilePattern->tooltip("Keine Eingabe: Suche in allen; *.h: Suche nur in .h-Dateien; Pattern durch Ko\
mma trennen");
      _txtFilePattern->box(FL_FLAT_BOX);
      _txtFilePattern->selection_color((Fl_Color)40);
      _txtFilePattern->labelfont(1);
      _txtFilePattern->labelcolor((Fl_Color)53);
      _txtFilePattern->textfont(1);
    } // Fl_Input* _txtFilePattern
    { _cbMatchWord = new Fl_Check_Button(139, 73, 70, 25, " nur ganzes Wort suchen");
      _cbMatchWord->tooltip("Sucht den eingegebenen Suchtext nur als ganzes Wort, nicht als Bestandteil ei\
nes anderen Wortes");
      _cbMatchWord->down_box(FL_DOWN_BOX);
      _cbMatchWord->color(FL_LIGHT2);
      _cbMatchWord->labelfont(1);
      _cbMatchWord->labelcolor((Fl_Color)53);
    } // Fl_Check_Button* _cbMatchWord
    { _grpResults = new Fl_Group(10, 391, 580, 169);
      _grpResults->box(FL_FLAT_BOX);
      _grpResults->color((Fl_Color)35);
      { _table = new ResultTable(10, 391, 580, 169);
        _table->box(FL_THIN_UP_FRAME);
        _table->color((Fl_Color)35);
        _table->selection_color(FL_BACKGROUND_COLOR);
        _table->labeltype(FL_NORMAL_LABEL);
        _table->labelfont(0);
        _table->labelsize(12);
        _table->labelcolor(FL_FOREGROUND_COLOR);
        _table->align(Fl_Align(FL_ALIGN_TOP));
        _table->when(FL_WHEN_RELEASE);
        _table->end();
      } // ResultTable* _table
      _grpResults->end();
    } // Fl_Group* _grpResults
    { Fl_Box* o = new Fl_Box(12, 368, 150, 25, "Treffer in Dateien");
      o->labelfont(1);
      o->labelcolor((Fl_Color)53);
    } // Fl_Box* o
    { _cbCaseSensitiv = new Fl_Check_Button(139, 49, 70, 25, " Gro\303\237-/Kleinschreibung beachten");
      _cbCaseSensitiv->down_box(FL_DOWN_BOX);
      _cbCaseSensitiv->color(FL_LIGHT2);
      _cbCaseSensitiv->labelfont(1);
      _cbCaseSensitiv->labelcolor((Fl_Color)53);
    } // Fl_Check_Button* _cbCaseSensitiv
    { _cbIncludeBinaries = new Fl_Check_Button(139, 147, 70, 25, " auch in Bin\303\244rdateien suchen");
      _cbIncludeBinaries->down_box(FL_DOWN_BOX);
      _cbIncludeBinaries->color(FL_LIGHT2);
      _cbIncludeBinaries->labelfont(1);
      _cbIncludeBinaries->labelcolor((Fl_Color)53);
    } // Fl_Check_Button* _cbIncludeBinaries
    { _outStatus = new Fl_Output(0, 575, 600, 25);
      _outStatus->box(FL_FLAT_BOX);
      _outStatus->color((Fl_Color)46);
      _outStatus->labeltype(FL_NO_LABEL);
      _outStatus->textsize(12);
    } // Fl_Output* _outStatus
    { _outCommand = new Fl_Output(10, 327, 580, 25);
      _outCommand->box(FL_FLAT_BOX);
      _outCommand->color((Fl_Color)46);
      _outCommand->labeltype(FL_NO_LABEL);
      _outCommand->textsize(12);
    } // Fl_Output* _outCommand
    { Fl_Box* o = new Fl_Box(7, 303, 220, 25, "Erzeugtes grep Command");
      o->labelfont(1);
      o->labelcolor((Fl_Color)53);
    } // Fl_Box* o
    o->end();
    o->resizable(o);
  } // Fl_Double_Window* o
  return w;
}