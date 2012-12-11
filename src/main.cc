#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#ifdef __WIN32__
#include <windows.h>
#endif

#include <GL/glut.h>
#include <iostream>
#include <climits>

#include "gui/GUI.hh"
#include "gui/SampleElement.hh"
#include "gui/Grid.hh"
#include "Sample.hh"
#include "Listener.hh"


using namespace std;

using GUI::Grid;
using GUI::SampleElement;


// Consts
const int SRATE = 44100;
const int SSIZE = 882;

// The GUI
GUI::GUI gui;

// The Listener
Listener listener;

// Threshold
int threshold = 200;



void Render()
{
  glClear( GL_COLOR_BUFFER_BIT );

  gui.Render();

  glutSwapBuffers();
}



void Idle()
{
  SampleElement *se = static_cast<SampleElement*>( gui.GetChild( "GRID" )->GetChild( "SAMPLE" ) );

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

  // Memory leak:
  finished->clear();
}



void ProcessKeyInput( unsigned char key, int x, int y )
{
  bool thresholdChanged = false;

  switch( key )
  {
    // If +
    case 43:
      threshold += 20;
      thresholdChanged = true;
      break;

    // If -
    case 45:
      threshold -= 20;
      thresholdChanged = true;
      break;

    // If ESC
    case 27:
      exit( 0 );
      break;
  }

  if( thresholdChanged )
  {
    if( threshold < 0 )
      threshold = 0;
    else if( threshold >= SHRT_MAX )
      threshold = SHRT_MAX;

    listener.SetThreshold( threshold );

    SampleElement *se = static_cast<SampleElement*>( gui.GetChild( "GRID" )->GetChild( "SAMPLE" ) );
    if( se )
    {
      se->SetThreshold( threshold );
      glutPostRedisplay();
    }
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
  glutKeyboardFunc( ProcessKeyInput );


  // Set the GUI up
  SampleElement *se = new SampleElement();
  se->SetThreshold( 200 );
  se->ShowThreshold( true );

  Grid *grid = new Grid( 2, 8 );
  grid->Add( "SAMPLE", se );

  gui.SetArea( 0, 0, 1200, 800 );
  gui.Add( "GRID", grid );


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
                   threshold,
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

