/***************************************************************
 * Name:      Thomas_Tawadros_ClientApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Thomas Tawadros ()
 * Created:   2019-06-24
 * Copyright: Thomas Tawadros ()
 * License:
 **************************************************************/

#include "Thomas_Tawadros_ClientApp.h"

//(*AppHeaders
#include "Thomas_Tawadros_ClientMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(Thomas_Tawadros_ClientApp);

Thomas_Tawadros_ClientFrame* MainFrame;

bool Thomas_Tawadros_ClientApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	MainFrame = new Thomas_Tawadros_ClientFrame(0);
    	MainFrame->Show();
    	SetTopWindow(MainFrame);
    }
    //*)
    return wxsOK;

}
