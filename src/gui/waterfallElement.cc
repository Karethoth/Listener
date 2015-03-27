#include "../global.hh"
#include "waterfallElement.hh"
#include <SDL2/SDL.h>
#include <iostream>
#include <cstring>

using GUI::WaterfallElement;

using namespace std;

WaterfallElement::WaterfallElement()
{
  multiplier = 15.f;
  startRow = 0;
  data = new float[waterfallRowCount*waterfallRowSize];
  memset( data, 0, waterfallRowCount*waterfallRowSize );
}


WaterfallElement::~WaterfallElement()
{
  if( data )
  {
    delete[] data;
  }
}


void WaterfallElement::Update( std::complex<float> *ptr, size_t size )
{
  // Only use the first third
  size /= 3;

  // Move the start row "upwards"
  startRow--;
  if( startRow < 0 )
  {
	  startRow = waterfallRowCount-1;
  }

  // Add new row
  auto ratio = (float)size / waterfallRowSize;
  for( int x = 0; x < waterfallRowSize; x++ )
  {
	float total = 0.f;
    for( int i=0; i < (int)ratio; i++ )
    {
      total += ptr[size_t(x*ratio)].imag();
    }

    data[(startRow*waterfallRowSize)+x] = total;
  }
}


void WaterfallElement::Render()
{
  SDL_Rect tgt;
  tgt.w = (float)area.width/waterfallRowSize+1;
  tgt.h = (float)area.height/waterfallRowCount+1;

  int endRow = startRow-1;
  if( endRow < 0 )
  {
	  endRow = waterfallRowCount-1;
  }

  static auto counter = 0;
  int rowOffset = 0;
  for( int y = startRow; y != endRow; )
  {
	tgt.y = area.y + rowOffset*((float)area.height/waterfallRowCount);
    for( int x = 0; x < waterfallRowSize; x++ )
    {
	  tgt.x = area.x + x*((float)area.width/waterfallRowSize);
      auto value = data[y*waterfallRowSize+x]/200.f;
	  if( value < 0.f )
	  {
		  value *= -1.f;
	  }
	  int intensity = value*multiplier*128.f;
	  SDL_SetRenderDrawColor( global.renderer, intensity, intensity*2, intensity*3, 255 );
	  SDL_RenderFillRect( global.renderer, &tgt );
    }

	counter++;
	rowOffset++;
    y++;
    if( y >= waterfallRowCount )
    {
      y = 0;
    }
  }
}

