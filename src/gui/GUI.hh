#ifndef __GUI_HH__
#define __GUI_HH__

#include "Element.hh"

namespace GUI
{
  class GUI : public Element
  {
   public:
    ~GUI();

    void Render();
  };
};

#endif

