/***************************************************************
 * Name:      Thomas_Tawadros_ClientMain.h
 * Purpose:   Defines Application Frame
 * Author:    Thomas Tawadros ()
 * Created:   2019-06-24
 * Copyright: Thomas Tawadros ()
 * License:
 **************************************************************/

#ifndef THOMAS_TAWADROS_CLIENTMAIN_H
#define THOMAS_TAWADROS_CLIENTMAIN_H

//(*Headers(Thomas_Tawadros_ClientFrame)
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>
//*)

class Thomas_Tawadros_ClientFrame: public wxFrame
{
    public:

        Thomas_Tawadros_ClientFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~Thomas_Tawadros_ClientFrame();
        friend void printg(std::string n, int t);

    private:

        //(*Handlers(Thomas_Tawadros_ClientFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        void OnMenuItem4Selected(wxCommandEvent& event);
        void OnTextCtrl1TextEnter(wxCommandEvent& event);
        //*)

        //(*Identifiers(Thomas_Tawadros_ClientFrame)
        static const long ID_STATICTEXT1;
        static const long ID_LISTBOX1;
        static const long ID_STATICTEXT2;
        static const long ID_LISTBOX2;
        static const long ID_TEXTCTRL1;
        static const long ID_PANEL1;
        static const long ID_MENUITEM1;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(Thomas_Tawadros_ClientFrame)
        wxPanel* Panel1;
        wxListBox* MessageBox;
        wxStatusBar* StatusBar1;
        wxListBox* UserBox;
        wxStaticText* StaticText1;
        wxTextCtrl* TextCtrl1;
        wxStaticText* StaticText2;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // THOMAS_TAWADROS_CLIENTMAIN_H
