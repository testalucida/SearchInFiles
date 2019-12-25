
#include "resulttable.h"

#include <FL/fl_draw.H>

// Handle drawing all cells in table
void ResultTable::draw_cell(TableContext context, 
			  int R, int C, int X, int Y, int W, int H)
{
    switch ( context )
    {
	case CONTEXT_STARTPAGE:
	    fl_font(FL_HELVETICA, 12);
	    return;

	case CONTEXT_COL_HEADER:
//	    fl_push_clip(X, Y, W, H);
//	    {
//		fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, col_header_color());
//		fl_color(FL_BLACK);
//		fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
//	    }
//	    fl_pop_clip();
	    return;

	case CONTEXT_ROW_HEADER:
	    fl_push_clip(X, Y, W, H);
	    {
		fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, row_header_color());
		fl_color(FL_BLACK);
		fl_draw("Fundstelle", X, Y, W, H, FL_ALIGN_CENTER);
	    }
	    fl_pop_clip();
	    return;

	case CONTEXT_CELL:
	{
	    fl_push_clip(X, Y, W, H);
	    {
	        // BG COLOR
		fl_color( row_selected(R) ? selection_color() : cell_bgcolor);
		fl_rectf(X, Y, W, H);

		// TEXT
		fl_color(cell_fgcolor);
                const char* s = _pResult->get(R);
		fl_draw(s, X, Y, W, H, FL_ALIGN_LEFT);

		// BORDER
		fl_color(color()); 
		fl_rect(X, Y, W, H);
	    }
	    fl_pop_clip();
	    return;
	}

	case CONTEXT_TABLE:
	    return;

	case CONTEXT_ENDPAGE:
	case CONTEXT_RC_RESIZE:
	case CONTEXT_NONE:
	    return;
    }
}

// Callback whenever someone clicks on different parts of the table
void ResultTable::event_callback(Fl_Widget*, void *data)
{
    ResultTable *o = (ResultTable*)data;
    fprintf(stderr, "resulttable event_callback.\n");
    o->event_callback2();
}

void ResultTable::event_callback2()
{
    int R = callback_row(),
        C = callback_col();
    TableContext context = callback_context();
    const char *name = label() ? label() : "?";
//    printf("'%s' callback: Row=%d Col=%d Context=%d Event=%d InteractiveResize? %d\n",
//                  name, R, C, (int)context, (int)Fl::event(), (int)is_interactive_resize());
}

void ResultTable::setResult(Result* pResult) { 
    _pResult = pResult; 
    cols(1);
    rows(pResult->getCount());
    col_width(0, 1000);
    //rows(pResult->getCount());
}