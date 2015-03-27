#include "sample.hh"

const int maxSampleLength = 1024*100;

void Sample::Append( ShortVectorIterator &first,
                     const ShortVectorIterator &last )
{
  data.insert( data.end(), first, last );

  if( data.size() > maxSampleLength )
  {
    ShortVector( data.begin()+(data.size()-maxSampleLength), data.end() ).swap( data );
  }
}


ShortVector& Sample::GetData()
{
  return data;
}

