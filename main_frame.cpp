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
    EVT_MENU(idMenuConnect, main_frame::OnConnect)

   EVT_THREAD(wxJOYSTICK_TRY_OPEN, main_frame::OnJoystickTryOpen)
   EVT_THREAD(wxJOYSTICK_STATUS,main_frame::OnJoystickStatus)
   EVT_THREAD(wxJOYSTICK_CLOSED,main_frame::OnJoystickClosed)
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
}

void main_frame::OnJoystickStatus(wxThreadEvent& event)
{
   int const channel = event.GetInt();
   if ( channel >= 0){
     {
         wxCriticalSectionLocker enter(m_joystick_CS);
         m_actuator_drawing->m_actuators[channel]->set_value(m_actuator_drawing->m_actuator_channel[channel]/ 32767.0);
     }
      wxMessageBox("Joystick Chan");
   }else{
      destroy_thread();
      wxMessageBox("Joystick Error");
   }
}

void main_frame::wait_for_thread_deleted()
{
   bool thread_still_alive = true;
   while(thread_still_alive) {
      { 
         wxCriticalSectionLocker enter(m_joystick_CS);
         thread_still_alive = m_joystick_thread != nullptr;
      }
      if ( thread_still_alive){
         wxThread::This()->Sleep(1);
      }
   }
}

void main_frame::destroy_thread()
{      
   {
      wxCriticalSectionLocker enter(m_joystick_CS);
      if (this->m_joystick_thread) {        // does the thread still exist?
         wxMessageOutputDebug().Printf("MYFRAME: deleting thread");
         if (this->m_joystick_thread->Delete() != wxTHREAD_NO_ERROR )
         wxLogError("Can't delete the thread!");
      }
   } 
   wait_for_thread_deleted();
}

bool main_frame::Destroy()
{
   destroy_thread();
   return wxFrame::Destroy();
}

void main_frame::Connect()
{
   // need to mark the thread running state
   {
      wxCriticalSectionLocker enter(m_joystick_CS);
      if (m_joystick_thread != nullptr) {  return;}  
   }
   m_joystick_thread = new joystick_thread{this,"/dev/input/js0"};

   if ( m_joystick_thread->Create() == wxTHREAD_NO_ERROR ) {
 
      m_joystick_thread->Run();
     wxMessageBox("Thread Running");
   }else{
      wxMessageBox("Can't create the thread!");
      delete m_joystick_thread;
      m_joystick_thread= nullptr;
   }
}


void main_frame::OnJoystickClosed(wxThreadEvent& ev)
{
   wait_for_thread_deleted();
}

void main_frame::OnConnect(wxCommandEvent &event)
{

    Connect();
}

void main_frame::OnJoystickTryOpen(wxThreadEvent& event)
{
   int const result = event.GetInt();
   if ( result < 0){
       destroy_thread();
       wxMessageBox("Can't Open the Joystick!");
   }else{
       wxMessageBox("Joystick Opened!");
    }
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
