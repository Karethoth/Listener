#include "listenerException.hh"
#include <sstream>

ListenerException::ListenerException( std::string message, std::string file, int line )
{
  this->message = message;
  this->file    = file;
  this->line    = line;
}

ListenerException::~ListenerException() throw()
{
}


const char* ListenerException::what() const throw()
{
  std::ostringstream oss;
  oss << "(" << file << ":" << line << ") - ListenerException: " << message;
  return oss.str().c_str();
}

