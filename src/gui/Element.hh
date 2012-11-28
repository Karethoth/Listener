#ifndef __ELEMENT_HH__
#define __ELEMENT_HH__

#include <string>
#include <map>


namespace GUI
{
  class Element
  {
   private:
    std::map<std::string, Element*> children;

    void RenderChildren();


   public:
    virtual ~Element();

    virtual void Render();

    bool Add( std::string, Element* );
    bool Remove( std::string );
  };
};

#endif

