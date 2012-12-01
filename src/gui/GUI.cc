#include "GUI.hh"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

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
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho( 0, area.width, area.height, 0, 0, 1 );
  glMatrixMode( GL_MODELVIEW );

  if( children.size() )
    RenderChildren();
}

