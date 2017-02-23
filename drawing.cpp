

#include "mixer_guiApp.h"
#include "drawing.hpp"

drawing::vect drawing::get_size() const
{
  return wxGetApp().get_document()->get_map_size();
}
