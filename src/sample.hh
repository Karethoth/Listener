#pragma once

#include <vector>

typedef std::vector<short>           ShortVector;
typedef std::vector<short>::iterator ShortVectorIterator;

/*
 * Class Sample
 * ------------
 * Container for audio sample.
 */
class Sample
{
 private:
  ShortVector data;

 public:
  // Append data to the sample
  void Append( ShortVectorIterator &first,
               const ShortVectorIterator &last );

  ShortVector& GetData();
};

