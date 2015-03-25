#include "sample.hh"


void Sample::Append( ShortVectorIterator &first,
                     const ShortVectorIterator &last )
{
  data.insert( data.end(), first, last );
}


ShortVector& Sample::GetData()
{
  return data;
}

