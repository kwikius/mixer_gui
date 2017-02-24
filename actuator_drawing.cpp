
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
//#include "joystick_thread.hpp"

namespace {
   QUAN_ANGLE_LITERAL(rad)
   QUAN_ANGLE_LITERAL(deg)
   QUAN_QUANTITY_LITERAL(length,mm)
}

BEGIN_EVENT_TABLE(actuator_drawing,wxScrolledWindow)

   EVT_SIZE(actuator_drawing::OnSize)
   EVT_PAINT(actuator_drawing::OnPaint)
   EVT_SCROLLWIN(actuator_drawing::OnScroll)
   EVT_LEFT_DOWN(actuator_drawing::OnMouseLeftDown)
   EVT_LEFT_UP(actuator_drawing::OnMouseLeftUp)
   EVT_MOTION(actuator_drawing::OnMouseMove)
 
END_EVENT_TABLE()

actuator_drawing::~actuator_drawing()
{
    for ( auto & p : m_actuators) {
       if (p) {p->destroy();}
    }
}

actuator_drawing::actuator_drawing(wxWindow* parent)
: wxScrolledWindow{parent}
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

    m_actuators[0] = new servo{"V_Tail.Port",{-50_mm,-50_mm}, 0.1};
    m_actuators[1] = new servo{"V_Tail.Stbd",{50_mm,-50_mm}, -0.1};
    m_actuators[2] = new servo{"Throttle",{0_mm,75_mm}, 0.0};
    m_actuators[3] = new servo{"Ail.Port.Inner",{-75_mm,0_mm}, 0.25};
    m_actuators[4] = new servo{"Ail.Port.Outer",{-75_mm,50_mm}, -0.5};
    m_actuators[6] = new servo{"Ail.Stbd.Inner",{75_mm,0_mm}, 0};
    m_actuators[5] = new servo{"Ail.Stbd.Outer",{75_mm,50_mm}, 1};
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

   for (uint8_t i = 0u ; i < num_actuators; ++i){
      auto p = m_actuators[i];
      if ( p != nullptr){
         p->set_value(m_actuator_channel[i]/ 32767.0);;
         p->draw(wc);
      }
   }

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


