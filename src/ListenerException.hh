#ifndef __LISTENEREXCEPTION_HH__
#define __LISTENEREXCEPTION_HH__

#include <exception>
#include <string>


class ListenerException : public std::exception
{
 private:
  std::string message;
  char *file;
  int line;

 public:
  ListenerException( std::string message, char *file, int line );
  ~ListenerException() throw();

  virtual const char* what() const throw();
};


#endif

