#include "servo.hpp"
#include <quan/gx/primitives/box.hpp>
#include <quan/gx/primitives/circle.hpp>
#include <quan/gx/primitives/polyline.hpp>
#include <quan/gx/primitives/simple_text.hpp>
#include <quan/two_d/rotation.hpp>
#include <quan/angle.hpp>
#include <quan/length.hpp>

namespace {
   QUAN_ANGLE_LITERAL(rad)
   QUAN_ANGLE_LITERAL(deg)
   QUAN_QUANTITY_LITERAL(length,mm)
}

quan::angle::rad servo::value_to_angle() const
{
   return this->get_value() * 45_deg;
}

void servo::draw(quan::gx::wxwidgets::graphics_context const & wc) const
{
    typedef quan::length::mm mm;
    typedef quan::two_d::vect<mm> vect_mm;
    auto const pos = this->get_position();
    quan::gx::primitives::circle<mm> c{pos,3_mm};
    auto ctrl_len = 20_mm;
    quan::two_d::box<mm> box{-c.get_radius(),c.get_radius(),0_mm,-c.get_radius()};
    std::vector<vect_mm> vec{
         top_right(box)
         ,top_left(box)
         ,{ctrl_len * -2, 0_mm}
         ,bottom_left(box)
         ,bottom_right(box)
    };

    quan::two_d::rotation rotate{this->value_to_angle()};
    for ( auto & v: vec) {
       v = rotate(v) + pos;
    }

    quan::gx::primitives::polyline<mm> poly{vec};

    quan::gx::primitives::simple_text<mm> name{
      this->get_name()
      ,pos + vect_mm{5_mm,0_mm}
      ,5_mm
    };
        
    wc.draw_circle(c);
    wc.draw_polyline(poly);
    wc.draw_text(name);
}
