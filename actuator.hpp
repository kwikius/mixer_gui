#ifndef ACTUATOR_HPP_INCLUDED
#define ACTUATOR_HPP_INCLUDED

/*

 actuator is a servo, a motor, a control surface (e.g aileron flap etc"
  It is drawn somehow
  It has an asynchronous input within a min max range.
  When the input changes it is visually repreented in the control

*/

#include <quan/min.hpp>
#include <quan/max.hpp>
#include <quan/length.hpp>
#include <quan/two_d/vect.hpp>
#include <quan/constrain.hpp>
#include <quan/gx/wxwidgets/graphics_context.hpp>
#include <string>

struct actuator{


    virtual quan::two_d::vect<quan::length::mm> get_size() const = 0;
    virtual void draw(quan::gx::wxwidgets::graphics_context const & wc) const = 0;
    virtual void destroy(){ delete this;}

    typedef double value_type;
    value_type get_value()const { return m_value;}
    void set_value(value_type const & v)
    {
        m_value = quan::constrain(v,m_min_value, m_max_value);
    }
    quan::two_d::vect<quan::length::mm> get_position()const { return m_position;}
    // centre of nominal drawing for mous clicks
    void set_position(quan::two_d::vect<quan::length::mm> const & in) { m_position = in;}
    std::string get_name() const { return m_name;}
  protected:
    virtual ~actuator(){}
    actuator(
         std::string const& name_in, 
         quan::two_d::vect<quan::length::mm> const & pos_in, 
         value_type const & value_in)
        :m_name{name_in}
        ,m_position{pos_in}
        ,m_max_value{1.0}
        ,m_min_value{-1.0}
        {
            this->set_value(value_in);
        }
  private:
   
    std::string m_name;
    quan::two_d::vect<quan::length::mm> m_position;
    value_type const m_max_value;
    value_type const m_min_value;
    value_type m_value;
    
    
};

#endif // ACTUATOR_HPP_INCLUDED
