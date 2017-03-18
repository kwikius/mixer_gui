/***************************************************************
 * Name:      mixer_guiApp.h
 * Purpose:   Defines Application Class
 * Author:    Andy (kwikius@yahoo.com)
 * Created:   2017-02-20
 * Copyright: Andy (www.zoomworks.org)
 * License:
 **************************************************************/

#ifndef MIXER_GUIAPP_H
#define MIXER_GUIAPP_H

#include <wx/app.h>
#include <wx/config.h>

struct document;

struct mixer_guiApp : public wxApp
{
public:
   mixer_guiApp();
   ~mixer_guiApp();
   virtual bool OnInit();
   document* get_document()const {assert(m_document);return m_document;}

   wxConfig* get_config()const {return m_app_config;}
 
private:
   document* m_document;
   wxConfig* m_app_config;

};

DECLARE_APP(mixer_guiApp)

#endif // MIXER_GUIAPP_H
