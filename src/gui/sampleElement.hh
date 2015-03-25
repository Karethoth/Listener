#pragma once

#include "element.hh"
#include "../sample.hh"


namespace GUI
{
  class SampleElement : public Element
  {
   private:
    Sample *sample;
    short   threshold;
    bool    showThreshold;


   public:
    SampleElement( Sample *sample=nullptr );
    ~SampleElement();

    void Render();

    void SetSample( Sample* );

    void SetThreshold( short );
    void ShowThreshold( bool );
  };
};

