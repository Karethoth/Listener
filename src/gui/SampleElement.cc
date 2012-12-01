#include "SampleElement.hh"
#include <GL/gl.h>
#include <cstdio>
#include <vector>
#include <climits>

using std::map;
using std::vector;
using std::string;

using GUI::Box;
using GUI::Element;
using GUI::SampleElement;


SampleElement::SampleElement()
{
  SampleElement( NULL );
}



SampleElement::SampleElement( Sample *sample )
{
  SetSample( sample );
}



SampleElement::~SampleElement()
{
  map<string, Element*>::iterator it;
  for( it = children.begin(); it != children.end(); ++it )
  {
    delete it->second;
  }
  children.clear();
}



void SampleElement::SetSample( Sample *sample )
{
  this->sample = sample;
}



void SampleElement::Render()
{
  glColor3f( 1.0, 1.0, 1.0 );

  int ax = area.x;
  int ay = area.y;

  int bx = area.x+area.width;
  int by = area.y;

  int cx = area.x+area.width;
  int cy = area.y+area.height;

  int dx = area.x;
  int dy = area.y+area.height;

  // Render border
  glBegin( GL_LINE_STRIP );
    glVertex2i( ax, ay );
    glVertex2i( bx, by );
    glVertex2i( cx, cy );
    glVertex2i( dx, dy );
    glVertex2i( ax, ay );
  glEnd();

  if( !sample )
    return;

  // Render the data
  ShortVector data = sample->GetData();
  ShortVectorIterator it;

  int dataLength = data.size();

  if( dataLength <= 0 )
    return;

  float xRatio = (area.width-2) / (float)dataLength;
  float yRatio = (area.height/2-2) / (float)SHRT_MAX;
  float yMiddle = area.y + area.height/2;

  float xTmp, yTmp;

  glColor3f( 0.0, 1.0, 0.0 );

  glBegin( GL_LINE_STRIP );
  for( it = data.begin(); it != data.end(); ++it )
  {
    xTmp = ax + 1 + (it - data.begin()) * xRatio;
    yTmp = ay + yMiddle - (*it) * yRatio;

    glVertex2f( xTmp, yTmp );
  }
  glEnd();
}

