/***************************************************************
 * Name:      mixer_guiMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Andy (kwikius@yahoo.com)
 * Created:   2017-02-20
 * Copyright: Andy (www.zoomworks.org)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "main_frame.hpp"
#include "app.hpp"
#include "actuator_drawing.hpp"

BEGIN_EVENT_TABLE(main_frame, wxFrame)
    EVT_CLOSE(main_frame::OnClose)
    EVT_MENU(idMenuQuit, main_frame::OnQuit)
    EVT_MENU(idMenuAbout, main_frame::OnAbout)

END_EVENT_TABLE()

main_frame::main_frame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title),m_actuator_drawing{nullptr}
{
#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    fileMenu->Append(idMenuConnect, _("&Connect\tF1"), _("Connect"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText("mixer lang gui V0.1");

#endif // wxUSE_STATUSBAR
  
    m_actuator_drawing = new actuator_drawing(this);
}


main_frame::~main_frame()
{
   delete m_actuator_drawing;
}


bool main_frame::Destroy()
{
   return wxFrame::Destroy();
}

void main_frame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void main_frame::OnQuit(wxCommandEvent &event)
{
    
    Destroy();
}

void main_frame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox("mixer lang gui V0.1");
}
