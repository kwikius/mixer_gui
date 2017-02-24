

#include "app.hpp"
#include "drawing.hpp"
#include "document.hpp"

drawing::vect drawing::get_size() const
{
  return wxGetApp().get_document()->get_map_size();
}
