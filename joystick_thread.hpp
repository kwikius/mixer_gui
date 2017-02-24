#ifndef JOYSTICK_THREAD_HPP_INCLUDED
#define JOYSTICK_THREAD_HPP_INCLUDED

#include <wx/thread.h>


// reqd for js_event
#include <linux/joystick.h>
#include <cstdint>


//#include "actuator_drawing.hpp"

wxDECLARE_EVENT(wxJOYSTICK_TRY_OPEN, wxThreadEvent);
wxDECLARE_EVENT(wxJOYSTICK_STATUS, wxThreadEvent);
wxDECLARE_EVENT(wxJOYSTICK_CLOSED, wxThreadEvent);

struct main_frame;
struct joystick_thread : wxThread{
   // for try open   
   joystick_thread(main_frame* handler,const char* device_name)
   : wxThread{wxTHREAD_DETACHED}
   ,m_handler{handler}
   ,m_device_name{device_name}
   {}

   ~joystick_thread();
   void * Entry() override;
private:
   main_frame* m_handler;
   const char* const m_device_name;

};

#endif // JOYSTICK_THREAD_HPP_INCLUDED
