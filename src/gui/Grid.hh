#ifndef __GRID_HH__
#define __GRID_HH__

#include "Element.hh"


namespace GUI
{
  /*
   * GUI::Grid class
   * - Renders its children in a grid.
   */
  class Grid : public Element
  {
   private:
    int columns, rows;


   public:
    Grid( int, int );
    ~Grid();

    void Render();
  };
};

#endif

