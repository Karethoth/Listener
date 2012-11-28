#ifndef __ELEMENT_HH__
#define __ELEMENT_HH__

#include <string>
#include <map>


namespace GUI
{
  struct Box
  {
    int x, y;
    int width, height;
  };



  class Element
  {
   protected:
    std::map<std::string, Element*> children;
    struct Box area;

    void RenderChildren();


   public:
    virtual ~Element();

    virtual void Render();

    bool Add( std::string, Element* );
    bool Remove( std::string );

    void SetArea( struct Box );
    void SetArea( int, int, int, int );
    struct Box GetArea();
  };
};

#endif

