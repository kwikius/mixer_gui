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

#include "mixer_guiApp.h"


#include "actuator_drawing.hpp"

class mixer_guiFrame: public wxFrame
{
    public:
        mixer_guiFrame(wxFrame *frame, const wxString& title);
        ~mixer_guiFrame();
    private:
        enum
        {
            idMenuQuit = 1000,
            idMenuAbout
        };
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);

        actuator_drawing * m_actuator_drawing;
        DECLARE_EVENT_TABLE()

};


#endif // MIXER_GUIMAIN_H
