/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   resulttable.h
 * Author: martin
 *
 * Created on 14. Dezember 2019, 00:19
 */

#ifndef RESULTTABLE_H
#define RESULTTABLE_H

    
#include <FL/Fl_Table_Row.H>

#include "result.h"

#include <string>

class ResultTable : public Fl_Table_Row
{
private:
    Fl_Color cell_bgcolor;				// color of cell's bg color
    Fl_Color cell_fgcolor;				// color of cell's fg color
    const Result* _pResult;
protected:
    void draw_cell(TableContext context,  		// table cell drawing
    		   int R=0, int C=0, int X=0, int Y=0, int W=0, int H=0);
    static void event_callback(Fl_Widget*, void*);
    void event_callback2();				// callback for table events

public:
    ResultTable(int x, int y, int w, int h, const char *l=0) : Fl_Table_Row(x,y,w,h,l)
    {
        rows(0);
        cols(1);
        cell_bgcolor = FL_WHITE;
        cell_fgcolor = FL_BLACK;
        callback(&event_callback, (void*)this);
	end();
    }
    ~ResultTable() { }
    Fl_Color GetCellFGColor() const { return(cell_fgcolor); }
    Fl_Color GetCellBGColor() const { return(cell_bgcolor); }
    void SetCellFGColor(Fl_Color val) { cell_fgcolor = val; }
    void SetCellBGColor(Fl_Color val) { cell_bgcolor = val; }
    void setResult(const Result& result);
};


#endif /* RESULTTABLE_H */

