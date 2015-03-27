#include "../global.hh"
#include "fftElement.hh"

#include <SDL2/SDL.h>
#include <iostream>
#include <climits>

using namespace std;
using GUI::FFTElement;


FFTElement::FFTElement() : fft( fftElementCount, false )
{
  timeMem = new complex<float>[fftElementCount];
  freqMem = new complex<float>[fftElementCount];

  if( timeMem )
  {
    memset( timeMem, 0, sizeof( float ) * fftElementCount );
  }

  if( freqMem )
  {
    memset( freqMem, 0, sizeof( float ) * fftElementCount );
  }
}



FFTElement::~FFTElement()
{
  if( timeMem )
  {
    delete[] timeMem;
  }

  if( freqMem )
  {
    delete[] freqMem;
  }
}


complex<float>* FFTElement::GetFrequencyMemory()
{
  return freqMem;
}


void FFTElement::Render()
{
  if( !freqMem )
  {
    return;
  }

  auto xCurr = 0;
  auto yCurr = 0;
  auto xPrev = 0;
  auto yPrev = 0;
  auto valPrev = 0.f;

  const auto finalElementCount = fftElementCount/3;


  for( auto i=0; i<finalElementCount; i += 2 )
  {
    auto val = abs( freqMem[i].real() ) / 10.f;
    if( val > 1.f )
    {
      val = 1.f;
    }

    auto colorVal = (valPrev + val) / 2.f;

    xCurr = area.x + (float)i / finalElementCount * area.width;
    yCurr = (area.y + area.height) - (val * area.height);

    if( i != 0 )
    {
      SDL_SetRenderDrawColor( global.renderer, 255, 255-colorVal*255, 255-colorVal*255, 255 );
      SDL_RenderDrawLine( global.renderer, xPrev, yPrev, xCurr, yCurr );
    }
    
    xPrev = xCurr;
    yPrev = yCurr;
    valPrev = val;
  }

}


void FFTElement::Generate( Sample *sample )
{
  if( !sample )
  {
    return;
  }

  const auto data = sample->GetData();

  if( data.size() < fftElementCount )
  {
    return;
  }

  // Find the largest absolute value
  float largestVal = USHRT_MAX;

  auto startInd = data.size() - fftElementCount;

  for( auto i=0; i < fftElementCount; i++ )
  {
    auto val = (float)data[startInd + i] / largestVal;
    timeMem[i] = complex<float>( val, val );
  }

  fft.transform( timeMem, freqMem );
}

