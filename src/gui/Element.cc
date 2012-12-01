#include "Element.hh"

using std::map;
using std::pair;
using std::string;

using GUI::Box;
using GUI::Element;



Element::~Element()
{
  map<string, Element*>::iterator it;
  for( it = children.begin(); it != children.end(); ++it )
  {
    delete it->second;
  }
  children.clear();
}



void Element::RenderChildren()
{
  map<string, Element*>::iterator it;
  for( it = children.begin(); it != children.end(); ++it )
  {
    it->second->Render();
  }
}



void Element::Render()
{
  if( children.size() > 0 )
    RenderChildren();
}



bool Element::Add( string key, Element *value )
{
  value->SetArea( area.x, area.y, area.width, area.height );

  pair<map<string, Element*>::iterator, bool> ret;
  ret = children.insert( pair<string, Element*>( key, value ) );

  return ret.second;
}



bool Element::Remove( string key )
{
  if( children.erase( key ) )
    return true;

  return false;
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

