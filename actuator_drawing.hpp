#ifndef ACTUATOR_WIDGET_HPP_INCLUDED
#define ACTUATOR_WIDGET_HPP_INCLUDED

#include <wx/scrolwin.h>

#include <quan/two_d/vect.hpp>
#include <quan/three_d/vect.hpp>
#include <quan/length.hpp>
#include <quan/reciprocal_length.hpp>
#include <quan/gx/simple_drawing_view.hpp>
#include <quan/gx/simple_device_window.hpp>

#include "actuator.hpp"
#include "drawing.hpp"

#include <vector>

struct joystick_thread;

struct actuator_drawing : wxScrolledWindow{

   void OnSize(wxSizeEvent & event);
   void OnScroll(wxScrollWinEvent & event);
   void OnHScroll(wxScrollWinEvent & event);
   void OnVScroll(wxScrollWinEvent & event);
   void OnPaint(wxPaintEvent & event);
   void OnMouseLeftDown(wxMouseEvent & event);
   void OnMouseLeftUp(wxMouseEvent & event);
   void OnMouseMove(wxMouseEvent & event);

   actuator_drawing(wxWindow* parent);

   DECLARE_EVENT_TABLE()

   typedef quan::two_d::vect<int>               vect2_i;
   typedef quan::two_d::vect<double>            vect2_d;
   typedef quan::length::mm                          mm;
   typedef quan::two_d::vect<mm>               vect2_mm;
   typedef quan::reciprocal_length::per_mm       per_mm;
   typedef quan::two_d::vect<per_mm>       vect2_per_mm;

   typedef quan::three_d::vect<mm>             vect3_mm;
~actuator_drawing();
private:

   drawing  m_drawing;
   quan::gx::simple_drawing_view m_drawing_view;
   quan::gx::simple_device_window m_device_window;

   static constexpr uint8_t num_actuators = 16;

   actuator* m_actuators[num_actuators];
   /*
      These are updated by the joystick thread
      Kept separate to the actual actuators
      then they can be updated quickly while maybe the actuators are being drawn for example
      The actuators should use TryEnter to check for new values
   */
   int32_t   m_actuator_channel[num_actuators];

   friend class joystick_thread;
   friend class main_frame;
};

#endif // ACTUATOR_WIDGET_HPP_INCLUDED
