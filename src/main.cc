#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#ifdef __WIN32__
#include <windows.h>
#endif

#include <GL/glut.h>
#include <iostream>
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


// The Listener
Listener listener;


void Render()
{
  glClear( GL_COLOR_BUFFER_BIT );

  glutSwapBuffers();
}



void Idle()
{
  // Listen and record if needed
  Sample *currentSample = listener.Listen();

  if( !currentSample )
    return;

  std::vector<Sample*> *finished = listener.GetFinishedSamples();
  if( finished->size() > 0 )
  {
    finished->clear();
  }
}



int main( int argc, char **argv )
{

  // Init glut
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
  glutInitWindowPosition( 0, 0 );
  glutInitWindowSize( 1200, 800 );
  glutCreateWindow( "Listener" );

  glutDisplayFunc( Render );
  glutIdleFunc( Idle );


  // Open the default recording device
  alGetError();
  ALCdevice *device = alcCaptureOpenDevice( NULL, SRATE, AL_FORMAT_MONO16, SSIZE );
  if( alGetError() != AL_NO_ERROR )
  {
      return 0;
  }

  // Initialize the Listener
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


  // Start the main loop
  glutMainLoop();


  // Stop capturing
  alcCaptureStop( device );
  alcCaptureCloseDevice( device );

  return 0;
}

