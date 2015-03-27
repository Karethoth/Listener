#ifndef __LISTENER_HH__
#define __LISTENER_HH__

#include <AL/al.h>
#include <AL/alc.h>
#include <vector>

#include "sample.hh"
#include "listenerException.hh"


class Listener
{
 private:
  /* OpenAL relevant member variables: */
  ALCdevice *device;            // Capture device.
  ALbyte    *buffer;            // Pointer to buffer.
  ALenum     format;            // Format of audio samples.
  ALint      samplesAvailable;  // Count of audio samples available.
  ALint      sampleRate;        // Count of audio samples per second.
  ALint      bufferSize;        // How many audio samples fit in to the buffer.


  /* Rest of the member variables: */
  int threshold;                // The threshold to start recording.
  int thresholdCount;           // Maximum count of audio samples between
                                // triggering thresholds without splitting
                                // samples.

  int sinceTriggeredThreshold;  // Count of audio samples since last time
                                // triggering the threshold.
  
  Sample *currentSample;        // The current sample.
  bool    isRecording;          // True if we are recording audio samples now.
  bool    recordAlways;

  // And to keep record of finished samples:
  std::vector<Sample*> finishedSamples;



 public:
  Listener();
  ~Listener();

  // Initializes the Listener.
  void Init( ALCdevice *device,
             ALint      sampleRate,
             ALenum     format,
             ALint      bufferSize,
             int        threshold,
             int        thresholdCount );

  // Handles listening and recording samples.
  // Returns pointer to current sample.
  Sample* Listen();

  // Returns reference to the finished samples.
  std::vector<Sample*>* GetFinishedSamples();

  void SetThreshold( int );


 protected:
  void HandleAudioSamples();
};

#endif

