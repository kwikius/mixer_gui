
#include <quan/two_d/vect.hpp>
#include <quan/three_d/vect.hpp>
#include <quan/length.hpp>
#include <quan/reciprocal_length.hpp>
#include <quan/gx/simple_drawing_view.hpp>
#include <quan/gx/simple_device_window.hpp>
#include "actuator_drawing.hpp"

#include <quan/gx/primitives/simple_line.hpp>
#include <quan/gx/wxwidgets/graphics_context.hpp>
#include "servo.hpp"

namespace {
   QUAN_ANGLE_LITERAL(rad)
   QUAN_ANGLE_LITERAL(deg)
   QUAN_QUANTITY_LITERAL(length,mm)
}

BEGIN_EVENT_TABLE(actuator_drawing,wxScrolledWindow)
   EVT_CLOSE(actuator_drawing::OnClose)
   EVT_SIZE(actuator_drawing::OnSize)
   EVT_PAINT(actuator_drawing::OnPaint)
   EVT_SCROLLWIN(actuator_drawing::OnScroll)
   EVT_LEFT_DOWN(actuator_drawing::OnMouseLeftDown)
   EVT_LEFT_UP(actuator_drawing::OnMouseLeftUp)
   EVT_MOTION(actuator_drawing::OnMouseMove)
   EVT_JOYSTICK_EVENTS(actuator_drawing::OnJoystickEvent)
 
END_EVENT_TABLE()

actuator_drawing::~actuator_drawing()
{
    for ( auto & p : m_actuators) {
       if (p) {p->destroy();}
    }

    if (m_joystick){
      delete m_joystick;
      m_joystick = nullptr;
    }
}

actuator_drawing::actuator_drawing(wxWindow* parent)
:wxScrolledWindow{parent}
,m_joystick{nullptr}
{
    // pixels
    for ( auto & p : m_actuators) {
       p = nullptr;
    }
    this->SetVirtualSize(10000,10000);
    // rate in pixels per scrollunit
    this->SetScrollRate(100,100);
    // in scrollunits
    this->Scroll(50,50);
    this->ShowScrollbars (wxSHOW_SB_ALWAYS,wxSHOW_SB_ALWAYS);
    this->m_drawing_view.set_scale(1);

    constexpr auto outer_crow = 0_deg;
    constexpr auto inner_crow = 0_deg;
    m_actuators[0] = new servo{"STBD.AIL.OUTR",{80_mm,20_mm},180_deg - outer_crow };
    m_actuators[1] = new servo{"STBD.AIL.INNR",{30_mm,20_mm},180_deg - inner_crow};
    m_actuators[2] = new servo{"PORT.AIL.INNR",{-30_mm,20_mm}, inner_crow};
    m_actuators[3] = new servo{"PORT.AIL.OUTR",{-80_mm,20_mm}, outer_crow};
    m_actuators[4] = new servo{"VTAIL.PORT",{-20_mm,-20_mm}};
    m_actuators[5] = new servo{"VTAIL.STBD",{20_mm,-20_mm},180_deg};
    m_actuators[6] = new servo{"THROTTLE",{0_mm,70_mm},180_deg, -1.0}; // 

    m_joystick = new wxJoystick{wxJOYSTICK1};
    m_joystick->SetCapture(this,10);
   
   // create_mixer_from_file();
}

void actuator_drawing::OnSize(wxSizeEvent & event)
{
   this->GetClientSize(
         &this->m_device_window.m_size_px.x,
         &this->m_device_window.m_size_px.y
   );
   
   vect2_i display_size_in_px;
   wxDisplaySize(&display_size_in_px.x,&display_size_in_px.y);

   wxSize detail_display_size_mm = wxGetDisplaySizeMM();
   vect2_mm display_size_in_mm{mm{detail_display_size_mm.x},mm{detail_display_size_mm.y}};
   
   this->m_device_window.m_size_mm.x 
   = (this->m_device_window.m_size_px.x * display_size_in_mm.x) / display_size_in_px.x;
   this->m_device_window.m_size_mm.y 
   = (this->m_device_window.m_size_px.y * display_size_in_mm.y) / display_size_in_px.y;
  
   this->Refresh();

}

void actuator_drawing::OnPaint(wxPaintEvent & event)
{
   wxPaintDC dc(this);
   dc.SetBackground(* wxWHITE_BRUSH); // sets background brush but doesnt clear
   dc.Clear(); //       need to invoke to clear using current background brush

   quan::gx::wxwidgets::graphics_context wc{
      &dc, 
      &this->m_drawing,
      &this->m_drawing_view,
      &this->m_device_window
   };

   eval_mixer();

   for (uint8_t i = 0u ; i < num_actuators; ++i){
      auto p = m_actuators[i];
      if ( p != nullptr){
         if ( m_joystick && (i < m_joystick->GetNumberAxes())){
            p->draw(wc);
         }
      }
   }
}

void actuator_drawing::OnClose(wxCloseEvent& event)
{
   // Though same as wxWidgets joystick examples
   // coredumps atm. 
   if (m_joystick){
      m_joystick->ReleaseCapture();
   }
}

void actuator_drawing::OnJoystickEvent(wxJoystickEvent& event)
{
   this->Refresh();
}

void actuator_drawing::OnScroll(wxScrollWinEvent & event)
{
  if (event.GetOrientation() == wxHORIZONTAL){
     this->OnHScroll(event);
  }else{
     this->OnVScroll(event);
  }
  this->Refresh();
}

void actuator_drawing::OnHScroll(wxScrollWinEvent & event)
{
   this->m_drawing_view.set_x_scroll_ratio((event.GetPosition() - 50 )/100.0); 
}

void actuator_drawing::OnVScroll(wxScrollWinEvent & event)
{
   this->m_drawing_view.set_y_scroll_ratio( -(event.GetPosition() - 50 )/100.0);
}

void actuator_drawing::OnMouseLeftDown(wxMouseEvent & event)
{
   
}

void actuator_drawing::OnMouseLeftUp(wxMouseEvent & event)
{

}

void actuator_drawing::OnMouseMove(wxMouseEvent & event)
{

}


