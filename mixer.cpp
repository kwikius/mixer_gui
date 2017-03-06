
#include "actuator_drawing.hpp"
#include <mixer_lang.hpp>

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
      apm_mix::input_pair{"Pitch", get_pitch},
      apm_mix::input_pair{"Yaw",  get_yaw},
      apm_mix::input_pair{"Roll", get_roll},
      apm_mix::input_pair{"Throttle", get_throttle},
      apm_mix::input_pair{"Flap", get_flap},
      apm_mix::input_pair{"Airspeed", get_airspeed},
      apm_mix::input_pair{"ControlMode", get_control_mode},
      apm_mix::input_pair{"ARSPD_MIN", static_cast<apm_mix::float_t(*)()>([]()->apm_mix::float_t{return 10.0;})},
      apm_mix::input_pair{"ARSPD_CRUISE",static_cast<apm_mix::float_t(*)()>([]()->apm_mix::float_t{return 12.0;})},
      apm_mix::input_pair{"ARSPD_MAX", static_cast<apm_mix::float_t(*)()>([]()->apm_mix::float_t{return 20.0;})},
      apm_mix::input_pair{"FAILSAFE_ON", failsafe_on},
      apm_mix::input_pair{"DUMMY_INT", static_cast<apm_mix::int_t(*)()>([]()->apm_mix::int_t{return 1000;})}
   };

   template <unsigned  N>
   void output_action(apm_mix::float_t const & v)
   {
      if ( p_drawing && (N < p_drawing->get_num_actuators()) &&  p_drawing->m_actuators[N]){
         p_drawing->m_actuators[N]->set_value(v);
      }
   }

   apm_mix::abc_expr* outputs[] = {
       new apm_mix::output<apm_mix::float_t>{output_action<0>}
     , new apm_mix::output<apm_mix::float_t>{output_action<1>}
     , new apm_mix::output<apm_mix::float_t>{output_action<2>}
     , new apm_mix::output<apm_mix::float_t>{output_action<3>}
     , new apm_mix::output<apm_mix::float_t>{output_action<4>}
     , new apm_mix::output<apm_mix::float_t>{output_action<5>}
     , new apm_mix::output<apm_mix::float_t>{output_action<6>}
   };
   //const char * const mixer_filename = "/home/andy/cpp/projects/mixer_lang/mixers/generic/flyingwing.mix";
   const char * const mixer_filename = "/home/andy/cpp/projects/mixer_lang/mixers/generic/glider.mix";
  // const char * const mixer_filename = "/home/andy/cpp/projects/mixer_lang/mixers/generic/crow.mix";
} // namespace

void actuator_drawing::eval_mixer()
{
   apm_mix::mixer_eval();
}

bool actuator_drawing::setup_mixer()
{
   p_drawing = this;
   if  ( !apm_mix::mixer_create(
          mixer_filename
         ,inputs, sizeof(inputs)/sizeof(inputs[0])
         ,outputs, sizeof(outputs)/sizeof(outputs[0])
      )){
         wxMessageBox("Create mixer failed");
         return false;
      }
   return true;
}

