#pragma once

#include "element.hh"


namespace GUI
{
  /*
   * GUI::Grid class
   * - Renders its children in a grid.
   */
  class Grid : public Element
  {
   public:
    Grid( int, int );
    ~Grid();

    void Render();

   private:
    int columns, rows;
  };
};

