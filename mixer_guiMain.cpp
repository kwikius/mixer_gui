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

#include "mixer_guiMain.h"

BEGIN_EVENT_TABLE(mixer_guiFrame, wxFrame)
    EVT_CLOSE(mixer_guiFrame::OnClose)
    EVT_MENU(idMenuQuit, mixer_guiFrame::OnQuit)
    EVT_MENU(idMenuAbout, mixer_guiFrame::OnAbout)
END_EVENT_TABLE()

mixer_guiFrame::mixer_guiFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title),m_actuator_drawing{nullptr}
{
#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
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


mixer_guiFrame::~mixer_guiFrame()
{
}

void mixer_guiFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void mixer_guiFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void mixer_guiFrame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox("mixer lang gui V0.1");
}
