#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <iostream>
#include <SDL/SDL.h>
using namespace std;

#include "Sample.hh"
#include "Listener.hh"

const int TRESHOLD = 200;
const int TRESHOLD_COUNT = 500;

const int SRATE = 44100;
const int SSIZE = 882;

ALbyte buffer[22050];
ALint samplesAvailable;

bool isSampling = false;
int tresholdCounter = 0;


const float hRatio = 400.0 / 32768.5;
const float wRatio = SSIZE / SSIZE;


void PutPixel32_nolock(SDL_Surface * surface, int x, int y, Uint32 color)
{
  Uint8 * pixel = (Uint8*)surface->pixels;
  pixel += (y * surface->pitch) + (x * sizeof(Uint32));
  *((Uint32*)pixel) = color;
}



void Draw( SDL_Surface *screen )
{
  SDL_FillRect( screen, NULL, 0x000000 );

  SDL_Flip( screen );
}



int main( int argc, char *argv[] )
{

  // Init SDL
  SDL_Surface *screen = NULL;
  SDL_Init( SDL_INIT_EVERYTHING );
  screen = SDL_SetVideoMode( 1200, 800, 32, SDL_SWSURFACE );


  alGetError();
  ALCdevice *device = alcCaptureOpenDevice(NULL, SRATE, AL_FORMAT_MONO16, SSIZE);
  if (alGetError() != AL_NO_ERROR)
  {
      return 0;
  }

  // Instancize the Listener
  Listener listener = Listener();

  // Try to initialize it
  try
  {
    listener.Init( device,
                   44100,
                   AL_FORMAT_MONO16,
                   400,
                   150,
                   4000 );
  }
  catch( ListenerException &e )
  {
    std::cout << "ERROR - " << e.what() << "\n";
    return 1;
  }


  // Start capturing
  alcCaptureStart( device );


  // The main loop
  while( true )
  {
    // Listen and record if needed
    Sample *currentSample = listener.Listen();

    if( !currentSample )
      continue;

    std::vector<Sample*> *finished = listener.GetFinishedSamples();
    if( finished->size() > 0 )
    {
      finished->clear();
    }

    // Draw the GUI
    Draw( screen );

    // Delay to free up some processor time
    SDL_Delay( 1 );
  }

  // Stop capturing
  alcCaptureStop( device );
  alcCaptureCloseDevice( device );

  return 0;
}

