/***************************************************************
 * Name:      mixer_guiApp.cpp
 * Purpose:   Code for Application Class
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

#include "app.hpp"
#include "main_frame.hpp"
#include "document.hpp"

#include "wx/joystick.h"

IMPLEMENT_APP(mixer_guiApp);

mixer_guiApp::mixer_guiApp()
:m_document{new document}
,m_app_config {new wxConfig(wxT("Mixer GUI"))}
{
}

bool mixer_guiApp::OnInit()
{
    wxJoystick stick(wxJOYSTICK1);
    if (!stick.IsOk()){
       wxMessageBox(wxT("No joystick detected!"));
       return false;
    }
    try {
      main_frame* frame = new main_frame(0L, _("mixer lang gui"));
      frame->Show();
      return true;
   }catch (std::exception & e){
     wxASSERT(false);
     return false;
   }
}

mixer_guiApp::~mixer_guiApp()
{
   delete m_document;
   delete m_app_config;
}
