#ifndef SERVO_HPP_INCLUDED
#define SERVO_HPP_INCLUDED

#include "actuator.hpp"
#include <quan/length.hpp>

struct servo : actuator{
    servo( std::string const& name_in, 
         quan::two_d::vect<quan::length::mm> const & pos_in, 
         value_type const & value_in): actuator{name_in,pos_in,value_in}{}
    quan::two_d::vect<quan::length::mm> get_size() const 
    {return {quan::length::mm{20.0},quan::length::mm{20.0}};}

    quan::angle::rad value_to_angle() const;
    void draw(quan::gx::wxwidgets::graphics_context const & wc) const ;

};

#endif // SERVO_HPP_INCLUDED
