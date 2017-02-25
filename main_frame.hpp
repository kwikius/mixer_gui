/***************************************************************
 * Name:      mixer_guiMain.h
 * Purpose:   Defines Application Frame
 * Author:    Andy (kwikius@yahoo.com)
 * Created:   2017-02-20
 * Copyright: Andy (www.zoomworks.org)
 * License:
 **************************************************************/

#ifndef MIXER_GUIMAIN_H
#define MIXER_GUIMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

struct actuator_drawing;

class main_frame: public wxFrame{
public:
   main_frame(wxFrame *frame, const wxString& title);
   ~main_frame();
private:
   enum{
      idMenuQuit = 1000,
      idMenuAbout,
      idMenuConnect
   };
   void OnClose(wxCloseEvent& event);
   void OnQuit(wxCommandEvent& event);
   void OnAbout(wxCommandEvent& event);
   void OnConnect(wxCommandEvent& event);


   bool Destroy() override;

   actuator_drawing * m_actuator_drawing;

   DECLARE_EVENT_TABLE()
};


#endif // MIXER_GUIMAIN_H
