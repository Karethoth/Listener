#include "Element.hh"

using std::map;
using std::pair;
using std::string;

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

