#ifndef DUMMY_ACTUATOR_HPP_INCLUDED
#define DUMMY_ACTUATOR_HPP_INCLUDED

#include "actuator.hpp"


struct dunmmy_actuator : actuator{

    quan::two_d::vect<quan::length::mm> get_size() const {return {0,0};}
    void draw(quan::gx::wxwidgets::graphics_context const & wc) const{};
    void destroy(){;}
};

#endif // DUMMY_ACTUATOR_HPP_INCLUDED
