#include "main_window.h"

#include <iostream>
#include <unistd.h>


void onStartSuche(Fl_Widget*, void*) {
    
}

void onCancel(Fl_Widget*, void*) {
    
}

int main() {
    //Fl::scheme("gtk+");
    Fl::scheme("gleam");
    
    Fl_Double_Window* pWin = make_window();
    _txtFilePattern->value(".*");
    char filename[FILENAME_MAX];
    getcwd(filename, FILENAME_MAX);
    _txtSuchVerzeichnis->value(filename);
    _cbRekursiv->value(1);
    
    _btnStart->callback(onStartSuche);
    _btnCancel->callback(onCancel);
    
    pWin->show();
    return Fl::run();
}
