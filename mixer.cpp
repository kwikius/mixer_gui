
#include "actuator_drawing.hpp"
#include <mixer_lang.hpp>
#include <mixer_lang_filestream.hpp>
#include <cstdio>
#include <cstring>
#include <quan/gx/wxwidgets/to_wxString.hpp>


char * apm_mix::duplicate_string(const char *s)
{
    return ::strdup(s);
}

void apm_mix::delete_string(const char* s)
{
    ::free(const_cast<char*>(s));
}

namespace {

   actuator_drawing* p_drawing = nullptr;

   apm_mix::float_t get_joystick_value(uint32_t i)
   {
      if (p_drawing && p_drawing->m_joystick){
         return p_drawing->m_joystick->GetPosition(i)/ 32767.0;
      }else{
         return 0;
      }
   }  
   // This value simulates the airspeed sensor reading
   // TODO make a way to vary it when running for sim ... prob
   // more suitable for a GUI version
   apm_mix::float_t airspeed_m_per_s = 0.0; //

   // true simulates a possible sensor failure 
   // which may mean (for example) airspeed reading is no good
   bool in_failsafe = false;  //
   
   // The mixer uses functions pointers  to get its inputs
   // The inputs can be of type Bool, Integer or Float 
   apm_mix::float_t get_airspeed(){ return airspeed_m_per_s;}
   bool failsafe_on() { return in_failsafe;}

   apm_mix::input_pair inputs[] = { 
      {"Pitch",static_cast<apm_mix::float_t(*)()>([]()->apm_mix::float_t{return get_joystick_value(1);})},
      {"Yaw",  static_cast<apm_mix::float_t(*)()>([]()->apm_mix::float_t{return get_joystick_value(3);})},
      {"Roll", static_cast<apm_mix::float_t(*)()>([]()->apm_mix::float_t{return get_joystick_value(0);})},
      {"Throttle", static_cast<apm_mix::float_t(*)()>([]()->apm_mix::float_t{return get_joystick_value(2);})},
      {"Flap", static_cast<apm_mix::float_t(*)()>([]()->apm_mix::float_t{return get_joystick_value(4);})},
      {"Airspeed", get_airspeed},
      {"ControlMode", static_cast<apm_mix::float_t(*)()>([]()->apm_mix::float_t{return get_joystick_value(5);})},
      {"ARSPD_MIN", static_cast<apm_mix::float_t(*)()>([]()->apm_mix::float_t{return 10.0;})},
      {"ARSPD_CRUISE",static_cast<apm_mix::float_t(*)()>([]()->apm_mix::float_t{return 12.0;})},
      {"ARSPD_MAX", static_cast<apm_mix::float_t(*)()>([]()->apm_mix::float_t{return 20.0;})},
      {"FAILSAFE_ON", failsafe_on},
      {"DUMMY_INT", static_cast<apm_mix::int_t(*)()>([]()->apm_mix::int_t{return 1000;})}
   };

   template <unsigned  N>
   void output_action(apm_mix::float_t const & v)
   {
      if ( p_drawing && (N < p_drawing->get_num_actuators()) &&  p_drawing->m_actuators[N]){
         p_drawing->m_actuators[N]->set_value(v);
      }
   }

   apm_mix::output<apm_mix::float_t> outputs[] = {
       {output_action<0>}
     , {output_action<1>}
     , {output_action<2>}
     , {output_action<3>}
     , {output_action<4>}
     , {output_action<5>}
     , {output_action<6>}
   };

   char error_message[200];

} // namespace

bool apm_mix::yyerror(const char* str )
{

   snprintf(error_message,200,"line %i , error : %s", apm_lexer::get_line_number(),str);

   return false;
}

void actuator_drawing::eval_mixer()
{
   if ( p_drawing != nullptr){
      apm_mix::mixer_eval();
   }
}
void actuator_drawing::close_mixer()
{
   if ( p_drawing){
      apm_mix::close_mixer();
      p_drawing = nullptr;
   }
}

bool actuator_drawing::create_mixer_from_file(const char* filename)
{
   apm_lexer::filestream_t stream{filename};
   p_drawing = this;
   bool result = false;
   if  ( !apm_mix::mixer_create(
          &stream
         ,inputs, sizeof(inputs)/sizeof(inputs[0])
         ,outputs, sizeof(outputs)/sizeof(outputs[0])
      )){
      close_mixer();
      wxMessageBox(quan::gx::wxwidgets::to_wxString(error_message));
   }else{
      result = true;
   }
   return result;
   
}

