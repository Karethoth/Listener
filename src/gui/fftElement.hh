#pragma once

#include "element.hh"
#include "../sample.hh"

#include <complex>
#include <kiss_fft.hh>

const int fftElementCount = 2048;

namespace GUI
{
  class FFTElement : public Element
  {
   public:
    FFTElement();
    ~FFTElement();

    void Render();

    void Generate( Sample *sample );

    std::complex<float>* GetFrequencyMemory();

   private:
    kissfft<float> fft;
    std::complex<float> *timeMem;
    std::complex<float> *freqMem;
  };
};

