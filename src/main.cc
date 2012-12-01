#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#ifdef __WIN32__
#include <windows.h>
#endif

#include <GL/glut.h>
#include <iostream>

#include "gui/GUI.hh"
#include "gui/SampleElement.hh"
#include "Sample.hh"
#include "Listener.hh"


using namespace std;

using GUI::SampleElement;


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


// The GUI
GUI::GUI gui;

// The Listener
Listener listener;



void Render()
{
  glClear( GL_COLOR_BUFFER_BIT );

  gui.Render();

  glutSwapBuffers();
}



void Idle()
{
  SampleElement *se = static_cast<SampleElement*>( gui.GetChild( "SAMPLE" ) );

  // Listen and record if needed
  Sample *currentSample = listener.Listen();

  if( currentSample && se )
  {
    if( currentSample->GetData().size() > 50 )
    {
      se->SetSample( currentSample );
      glutPostRedisplay();
      return;
    }
  }

  std::vector<Sample*> *finished = listener.GetFinishedSamples();
  if( finished->size() <= 0 )
    return;


  if( se )
  {
    Sample *sample = *(finished->begin());
    finished->erase( finished->begin() );
    se->SetSample( sample );
    glutPostRedisplay();
  }

  finished->clear();
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


  // Set the GUI up
  SampleElement *se = new SampleElement();
  gui.SetArea( 0, 0, 1200, 800 );
  gui.Add( "SAMPLE", se );


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
                   200,
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

