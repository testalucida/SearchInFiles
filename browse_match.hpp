
/* 
 * File:   browse_match.hpp
 * Author: martin
 *
 * Created on 8. Januar 2020, 14:16
 */

#ifndef BROWSE_MATCH_HPP
#define BROWSE_MATCH_HPP

#include <Fl/Fl_Double_Window.H>
#include <Fl/Fl_Browser.H>
#include <iostream>

class MatchBrowserWindow : public Fl_Double_Window {
    public:
        MatchBrowserWindow( int w, int h, const char* pLbl = NULL )
                : Fl_Double_Window( w, h, pLbl ) {
                    
                }
        virtual ~MatchBrowserWindow() {
            std::cerr << "MatchBrowserWindow now destructing." << std::endl;
        }
};

class MatchBrowser : public Fl_Browser {
public:
    MatchBrowser( int x, int y, int w, int h, const char* pLbl = NULL ) 
            : Fl_Browser( x, y, w, h, pLbl ) {
                
            }
    virtual ~MatchBrowser() {
        std::cerr << "MatchBrowser now destructing." << std::endl;
    }
};

#endif /* BROWSE_MATCH_HPP */

