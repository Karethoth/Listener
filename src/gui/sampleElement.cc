#include "../global.hh"
#include "sampleElement.hh"

#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <climits>

using std::map;
using std::vector;
using std::string;

using GUI::Box;
using GUI::Element;
using GUI::SampleElement;

using namespace std;

SampleElement::SampleElement( Sample *sample )
{
  SetSample( sample );

  threshold     = 10;
  showThreshold = false;
}



SampleElement::~SampleElement()
{
  for( auto& child : children )
  {
    delete child.second;
    child.second = nullptr;
  }
}



void SampleElement::SetSample( Sample *sample )
{
  this->sample = sample;
}



void SampleElement::SetThreshold( short threshold )
{
  this->threshold = threshold;
}



void SampleElement::ShowThreshold( bool value )
{
  showThreshold = value;
}



void SampleElement::Render()
{
  //glColor3f( 1.0, 1.0, 1.0 );
  SDL_SetRenderDrawColor( global.renderer, 255, 255, 255, 255 );

  int ax = area.x;
  int ay = area.y;

  int bx = area.x+area.width;
  int by = area.y;

  int cx = area.x+area.width;
  int cy = area.y+area.height;

  int dx = area.x;
  int dy = area.y+area.height;

  // Render border
  SDL_RenderDrawLine( global.renderer, ax, ay, bx, by );
  SDL_RenderDrawLine( global.renderer, bx, by, cx, cy );
  SDL_RenderDrawLine( global.renderer, cx, cy, dx, dy );
  SDL_RenderDrawLine( global.renderer, dx, dy, ax, ay );

  if( !sample )
  {
    return;
  }

  // Render the data
  ShortVector data = sample->GetData();
  ShortVectorIterator it;

  int dataLength = data.size();

  if( dataLength <= 0 )
  {
    return;
  }

  // Find the largest absolute value
  int largestVal = 1;
  for( auto& val : data )
  {
    auto currentVal = abs( val );
    if( currentVal > largestVal )
    {
      largestVal = currentVal;
    }
  }

  float xRatio = (area.width-2) / (float)dataLength;
  float yRatio = (area.height/2-2) / (float)largestVal;
  float yMiddle = area.y + area.height/2;

  float xTmp, yTmp;

  SDL_SetRenderDrawColor( global.renderer, 0, 255, 0, 255 );

  float xPrev = 0.f;
  float yPrev = 0.f;

  int stepSize = xRatio >= 1 ? 1 : 1 / xRatio;

  for( it = data.begin(); it != data.end(); )
  {
    xTmp = ax + 1 + (it - data.begin()) * xRatio;
    yTmp = yMiddle - (*it) * yRatio;

    if( xPrev != 0.f )
    {
      SDL_RenderDrawLine( global.renderer, xPrev, yPrev, xTmp, yTmp );
    }
    xPrev = xTmp;
    yPrev = yTmp;

    it += stepSize;
    if( it-data.begin() > (unsigned int)data.size() )
    {
      break;
    }
  }

  // If requested, render the threshold lines
  if( showThreshold )
  {
    SDL_SetRenderDrawColor( global.renderer, 127, 127, 255, 127 );
    yTmp = threshold * yRatio;

    
    SDL_RenderDrawLine(
        global.renderer,
        ax, yMiddle+yTmp,
        bx, yMiddle+yTmp
    );
    SDL_RenderDrawLine(
        global.renderer,
        ax, yMiddle-yTmp,
        bx, yMiddle-yTmp
    );
  }

  RenderChildren();
}

