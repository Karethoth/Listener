#pragma once

#include "element.hh"

namespace GUI
{
  class GUI : public Element
  {
   public:
    ~GUI();

    void Render();
  };
};

