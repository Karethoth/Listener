#ifndef __SAMPLEELEMENT_HH__
#define __SAMPLEELEMENT_HH__

#include "Element.hh"
#include "../Sample.hh"


namespace GUI
{
  class SampleElement : public Element
  {
   private:
    Sample *sample;


   public:
    SampleElement();
    SampleElement( Sample* );
    ~SampleElement();

    void Render();

    void SetSample( Sample* );
  };
};

#endif

