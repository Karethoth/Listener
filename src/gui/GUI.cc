#include "GUI.hh"

using std::map;
using std::pair;
using std::string;


GUI::GUI::~GUI()
{
  map<string, Element*>::iterator it;
  for( it = children.begin(); it != children.end(); ++it )
  {
    delete it->second;
  }
  children.clear();
}



void GUI::GUI::Render()
{
  if( children.size() )
    RenderChildren();
}

