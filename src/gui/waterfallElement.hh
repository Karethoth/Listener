#pragma once

#include "element.hh"
#include "../sample.hh"

#include <complex>

const int waterfallRowCount = 50;
const int waterfallRowSize  = 512;


namespace GUI
{
  class WaterfallElement : public Element
  {
   public:
    WaterfallElement();
    ~WaterfallElement();

    void Render();

    void Update( std::complex<float> *ptr, size_t size );

   private:
    float *data;
	int startRow;
	float multiplier;
  };
};

