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

#include "mixer_guiApp.h"
#include "mixer_guiMain.h"

IMPLEMENT_APP(mixer_guiApp);

mixer_guiApp::mixer_guiApp()
:m_document{new document}
{
}

bool mixer_guiApp::OnInit()
{
   try {
      mixer_guiFrame* frame = new mixer_guiFrame(0L, _("mixer lang gui"));
      frame->Show();
      return true;
   }catch (std::exception & e){
     wxASSERT(false);
     return false;
   }
}
