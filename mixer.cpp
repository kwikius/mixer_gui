
#include "actuator_drawing.hpp"
#include <mixer_lang.hpp>
#include <mixer_lang_filestream.hpp>
#include <cstdio>
#include <cstring>

bool apm_mix::yyerror(const char* str )
{
   char buf[200];
   sprintf(buf,"line %i , error : %s", apm_lexer::get_line_number(),str);
   wxMessageBox(buf);
   return false;
}

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

   apm_mix::float_t get_roll(){ return get_joystick_value(0);}
   apm_mix::float_t get_pitch() { return get_joystick_value(1);}
   apm_mix::float_t get_throttle() { return get_joystick_value(2);}
   apm_mix::float_t get_yaw()  { return get_joystick_value(3);}
   apm_mix::float_t get_flap() { return get_joystick_value(4);}
   apm_mix::float_t get_control_mode() { return get_joystick_value(5);}

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
      {"Pitch", get_pitch},
      {"Yaw",  get_yaw},
      {"Roll", get_roll},
      {"Throttle", get_throttle},
      {"Flap", get_flap},
      {"Airspeed", get_airspeed},
      {"ControlMode", get_control_mode},
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

} // namespace

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
      wxMessageBox("Create mixer failed");
      close_mixer();
   }else{
      result = true;
   }
   return result;
   
}

