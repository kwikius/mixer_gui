

#include "app.hpp"
#include "joystick_thread.hpp"
#include "actuator_drawing.hpp"
#include "main_frame.hpp"
#include <stdexcept>

namespace {
  bool fired = false;
}
void * joystick_thread::Entry() 
{
  // wxMessageBox("In Thread !!!!");
   // try to open the port
   int const fd = ::open(m_device_name, O_RDONLY | O_NONBLOCK);
   {
      wxThreadEvent event{wxJOYSTICK_TRY_OPEN};
      event.SetInt(fd); // >= 0 indicates success
      wxQueueEvent(m_handler, event.Clone());
   }

   if ( fd < 0){
     // throw std::runtime_error("whatever");
   }
   while ( !this->TestDestroy()){
  

      if ( fd >=0 ){
         js_event    js_event_in;
         if(read (fd, &js_event_in, sizeof(js_event_in)) == sizeof(js_event_in)) {
            if ((js_event_in.type & JS_EVENT_AXIS)==JS_EVENT_AXIS){
               bool updated = false;
               {
                  wxCriticalSectionLocker lock(m_handler->m_joystick_CS);
                  if ( js_event_in.number < m_handler->m_actuator_drawing->num_actuators){
                     m_handler->m_actuator_drawing->m_actuator_channel[js_event_in.number] = js_event_in.value;
                     updated = true;
                  }
               }
               if (updated){
                  // wxASSERT(false);
                  wxThreadEvent event{wxJOYSTICK_STATUS};
                  event.SetInt(js_event_in.number);
                  wxQueueEvent(m_handler,event.Clone());
               }
            }
         }
         if ( errno != EAGAIN){
            wxThreadEvent event{wxJOYSTICK_STATUS};
            event.SetInt(-1);
            wxQueueEvent(m_handler, event.Clone());
            break;
         }
      }

   }
   if (fd >= 0) { 
      ::close(fd);
   }
   wxThreadEvent event{wxJOYSTICK_CLOSED};
   wxQueueEvent(m_handler, event.Clone());
   return nullptr;
}

joystick_thread::~joystick_thread()
{
   wxCriticalSectionLocker lock(m_handler->m_joystick_CS);
   m_handler->m_joystick_thread = nullptr;
}

wxDEFINE_EVENT(wxJOYSTICK_TRY_OPEN, wxThreadEvent);
wxDEFINE_EVENT(wxJOYSTICK_STATUS,wxThreadEvent);
wxDEFINE_EVENT(wxJOYSTICK_CLOSED, wxThreadEvent);
