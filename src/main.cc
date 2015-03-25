#include <SDL2/SDL.h>
#ifdef __WIN32__
#include <windows.h>
#endif

#ifdef main
#undef main
#endif

#include <AL/alut.h>

#include <iostream>
#include <iomanip>
#include <climits>

#include "global.hh"
#include "gui/gui.hh"
#include "gui/sampleElement.hh"
#include "gui/grid.hh"
#include "sample.hh"
#include "listener.hh"


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
int threshold = 1000;


Global global{};

void Render()
{
  SDL_SetRenderDrawColor( global.renderer, 0, 0, 0, 255 );
  SDL_RenderFillRect( global.renderer, nullptr );

  gui.Render();


  SDL_RenderPresent( global.renderer );
}



void Update()
{
  SampleElement *se = static_cast<SampleElement*>( gui.GetChild( "GRID" )->GetChild( "SAMPLE" ) );

  // Listen and record if needed
  Sample *currentSample = listener.Listen();

  if( currentSample && se )
  {
    if( currentSample->GetData().size() > 50 )
    {
      se->SetSample( currentSample );
      //glutPostRedisplay();
      return;
    }
  }

  std::vector<Sample*> *finished = listener.GetFinishedSamples();
  if( finished->size() <= 0 )
  {
    return;
  }

  if( se )
  {
    Sample *sample = *(finished->begin());
    finished->erase( finished->begin() );
    se->SetSample( sample );
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
      //exit( 0 );
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
      //glutPostRedisplay();
    }
  }
}


void HandleSDLEvents()
{
  SDL_Event e;
  while( SDL_PollEvent( &e ) )
  {
    switch( e.type )
    {
      case SDL_QUIT:
        global.shouldQuit = true;
        break;
    }
  }
}


int main( int argc, char **argv )
{

  // Init glut
  //glutInit( &argc, argv );
  //glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
  //glutInitWindowPosition( 0, 0 );
  //glutInitWindowSize( 1200, 800 );
  //glutCreateWindow( "Listener" );

  //glutDisplayFunc( Render );
  //glutIdleFunc( Idle );
  //glutKeyboardFunc( ProcessKeyInput );
  //
  if( SDL_Init(SDL_INIT_VIDEO) != 0 )
  {
    cout << "SDL_Init failed!" << endl;
    return -1;
  }

  global.window = SDL_CreateWindow(
    "Listener",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    global.windowWidth,
    global.windowHeight,
    SDL_WINDOW_SHOWN
  );


  global.renderer = SDL_CreateRenderer( global.window, -1, 0 );

  // Set the GUI up
  Grid *grid = new Grid( 1, 8 );

  SampleElement *se = new SampleElement();
  se->SetThreshold( 200 );
  se->ShowThreshold( true );

  grid->Add( "SAMPLE", se );

  gui.SetArea( 0, 0, 1200, 800 );
  gui.Add( "GRID", grid );

  alutInit( 0, nullptr );

  // Open the default recording device
  alGetError();
  ALCdevice *device = alcCaptureOpenDevice( nullptr, SRATE, AL_FORMAT_MONO16, SSIZE );
  auto alError = alGetError();
  if( alError != AL_NO_ERROR )
  {
    cout << "alcCaptureOpenDevice failed: " << hex << alError << endl;
    alutExit();
    return 1;
  }

  // Initialize the Listener
  try
  {
    listener.Init(
      device,
      44100,
      AL_FORMAT_MONO16,
      400,
      threshold,
      14000
    );
  }
  catch( ListenerException &e )
  {
    std::cout << "ERROR - " << e.what() << "\n";
    alutExit();
    return 1;
  }


  // Start capturing
  alcCaptureStart( device );


  // Start the main loop
  while( !global.shouldQuit )
  {
    HandleSDLEvents();
    Update();
    Render();
  }


  // Stop capturing
  alcCaptureStop( device );
  alcCaptureCloseDevice( device );

  alutExit();
  return 0;
}

