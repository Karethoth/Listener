#pragma once

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
   public:
    virtual ~Element();

    virtual void Render();

    bool Add( std::string, Element* );
    bool Remove( std::string );

    Element* GetChild( std::string );

    void SetArea( struct Box );
    void SetArea( int, int, int, int );
    struct Box GetArea();

   protected:
    std::map<std::string, Element*> children;
    struct Box area;

    void RenderChildren();

  };
};

