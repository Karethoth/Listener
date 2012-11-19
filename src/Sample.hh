#ifndef __SAMPLE_HH__
#define __SAMPLE_HH__

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
  std::vector<short> data;


 public:
  Sample();
  ~Sample();

  // Append data to the sample
  void Append( ShortVectorIterator &first,
               const ShortVectorIterator &last );

  ShortVector& GetData();
};

#endif

