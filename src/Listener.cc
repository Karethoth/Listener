#include "Listener.hh"
#include <iostream>
#include <cstdio>

using std::vector;


Listener::Listener()
{
  device = NULL;
  buffer = NULL;
  currentSample = NULL;
  
  isRecording = false;
}



Listener::~Listener()
{
  if( buffer )
    delete[] buffer;
}



void Listener::Init( ALCdevice *device,
                     ALint      sampleRate,
                     ALenum     format,
                     ALint      bufferSize,
                     int        threshold,
                     int        thresholdCount )
{
  // Set some member variables:
  this->device         = device;
  this->sampleRate     = sampleRate;
  this->format         = format;
  this->bufferSize     = bufferSize;
  this->threshold      = threshold;
  this->thresholdCount = thresholdCount;


  // Check if parameters are valid:
  if( !device )
    throw( ListenerException( "Listener::Init(..) - No device given!", __FILE__, __LINE__ ) );

  if( bufferSize <= 0 )
    throw( ListenerException( "Listener::Init(..) - Buffer size must be larger than 0!", __FILE__, __LINE__ ) );

  if( format != AL_FORMAT_MONO16 )
    throw( ListenerException( "Listener::Init(..) - Wrong format, currently only MONO16 is supported!", __FILE__, __LINE__ ) );


  // Allocate the buffer:
  try
  {
    switch( format )
    {
      case AL_FORMAT_MONO16:
        buffer = new ALbyte[bufferSize*2];
        break;
    }
  }
  catch( std::exception &e )
  {
    std::string message = "Listener::Init(..) - Allocating buffer failed!(";
    message.append( e.what() );
    message.append( ")" );

    throw( ListenerException( message, __FILE__, __LINE__ ) );
  }

  currentSample = new Sample();
}



Sample* Listener::Listen()
{
  // Get the count of the audio samples available
  alcGetIntegerv( device, ALC_CAPTURE_SAMPLES, (ALCsizei)sizeof(ALint), &samplesAvailable );

  // Check if we don't have enough audio samples to be captured
  if( samplesAvailable < sampleRate/100 )
  {
    return currentSample;
  }

  // Fill the buffer with audio samples
  alcCaptureSamples( device, (ALCvoid *)buffer, bufferSize );

  HandleAudioSamples();

  return currentSample;
}



vector<Sample*>* Listener::GetFinishedSamples()
{
  return &finishedSamples;
}



void Listener::HandleAudioSamples()
{
  /*
   * HOX!
   * Currently assumes that format is
   * AL_FORMAT_MONO16
   */

  short *p = (short*)buffer;      // Pointer to the buffer

  ShortVector audioSampleVector;
  ShortVectorIterator it;
  ShortVectorIterator sectionStart;

  // Push audio samples to the vector
  for( int i=0; i < bufferSize; ++i )
  {
    audioSampleVector.push_back( *p );
  }


  // Set sectionStart to the first audio sample
  // by default
  sectionStart = audioSampleVector.begin();

  // Loop through audio samples in the vector
  for( it = audioSampleVector.begin();
       it != audioSampleVector.end();
       ++it )
  {
    // If we are recording
    if( isRecording )
    {
      // If current audio sample is below the treshold
      if( *it < threshold && *it > 0-threshold )
      {
        // If we haven't reached the maximum count
        // of sequental non-triggering audio samples
        if( sinceTriggeredThreshold < thresholdCount )
        {
          ++sinceTriggeredThreshold;
          continue;
        }

        // Maximum has been reached, stopping recording

        printf( "Stopping recording!\r\n" );
        isRecording = false;
        sinceTriggeredThreshold = 0;

        // Finalize the current sample
        currentSample->Append( sectionStart, it );

        // Push currentSample to the vector
        finishedSamples.push_back( currentSample );
        currentSample = new Sample();

      }
      // If we get an audio sample that's over
      // the threshold, while we're recording
      else if( *it >= threshold || *it <= 0-threshold )
      {
        sinceTriggeredThreshold = 0;
      }
    }
    // If we're not recording, but treshold gets triggered
    else if( *it >= threshold || *it <= 0-threshold )
    {
      printf( "Starting recording!\r\n" );
      isRecording = true;
      sinceTriggeredThreshold = 0;

      // Set the current section start
      sectionStart = it;

      if( currentSample )
        delete currentSample;

      currentSample = new Sample();
    }
    
    //tmp = (short)(tmp * hRatio);

    //PutPixel32_nolock( screen, (int)(i*wRatio), 400-tmp, 0xff00ff00 );
  }


  // If we have reached end of the buffer,
  // and we are still recording, we need
  // to push the audio samples to the
  // current sample
  if( isRecording && currentSample )
  {
    currentSample->Append( sectionStart, audioSampleVector.end() );
  }
}

