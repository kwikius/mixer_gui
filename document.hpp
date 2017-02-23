#ifndef QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED
#define QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED

#include <quan/two_d/vect.hpp>
#include <quan/three_d/vect.hpp>
#include <quan/length.hpp>

struct document{
   document();
   quan::two_d::vect<quan::length::mm> const & 
   get_map_size() const {return m_map_size;}
   void set_map_size( quan::two_d::vect<quan::length::mm> const & size)
   { m_map_size = size;}

   quan::length::mm get_click_bucket()const { return m_click_bucket;}

   private:
   quan::two_d::vect<quan::length::mm> m_map_size;
   quan::length::mm  m_click_bucket;
};

#endif // QUANTRACKER_SIM_DOCUMENT_HPP_INCLUDED
