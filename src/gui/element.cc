#include "element.hh"

using std::map;
using std::pair;
using std::string;

using GUI::Box;
using GUI::Element;



Element::~Element()
{
  for( auto& child : children )
  {
    if( child.second )
    {
      delete child.second;
      child.second = nullptr;
    }
  }
}



void Element::RenderChildren()
{
  for( auto& child : children )
  {
    if( child.second )
    {
      child.second->Render();
    }
  }
}



void Element::Render()
{
  RenderChildren();
}



bool Element::Add( string key, Element *value )
{
  value->SetArea( area.x, area.y, area.width, area.height );

  pair<map<string, Element*>::iterator, bool> ret;
  ret = children.insert( pair<string, Element*>{ key, value } );

  return ret.second;
}



bool Element::Remove( string key )
{
  return children.erase( key );
}



Element* Element::GetChild( string key )
{
  return children[key];
}



void Element::SetArea( struct Box area )
{
  SetArea( area.x, area.y, area.width, area.height );
}



void Element::SetArea( int x, int y,
                       int width, int height )
{
  area.x = x+1;
  area.y = y;

  area.width  = width-2;
  area.height = height-1;
}


struct Box Element::GetArea()
{
  return area;
}

