#include "Grid.hh"

using std::map;
using std::string;

using GUI::Box;
using GUI::Element;
using GUI::Grid;


Grid::Grid( int columns, int rows )
{
  this->columns = columns;
  this->rows    = rows;
}



Grid::~Grid()
{
  map<string, Element*>::iterator it;
  for( it = children.begin(); it != children.end(); ++it )
  {
    delete it->second;
  }
  children.clear();
}



void Grid::Render()
{
  /* Loop through every grid node and give their area to
   * corresponding child element and render it. */
  Box childArea;

  childArea.x = area.x;
  childArea.y = area.y;
  childArea.width  = area.width / columns;
  childArea.height = area.height / rows;
  
  map<string, Element*>::iterator it;
  it = children.begin();

  for( int j=0; j < rows; ++j )
  {
    for( int i=0; i < columns; ++i )
    {
      if( it == children.end() )
        return;

      it->second->SetArea( childArea );
      it->second->Render();
      ++it;

      childArea.x += childArea.width;
    }

    childArea.y += childArea.height;
    childArea.x  = area.x;
  }
}

