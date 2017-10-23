#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>

#include "SDL.h"

namespace utl_core
{
   class Texture
   {
   public:

      Texture();
      ~Texture();

      bool loadFromFile( std::string path, SDL_Renderer *renderer );
      void free();
      void setColor( Uint8 red, Uint8 green, Uint8 blue );
      void setBlendMode( SDL_BlendMode blending );
      void setAlpha( Uint8 alpha );
      void render( int x, int y, SDL_Rect *clip, SDL_Renderer *renderer );
      int getWidth();
      int getHeight();

   private:

      int _width;
      int _height;
      char *_data;

      SDL_Texture *_texture;
   };
}

#endif /* _TEXTURE_H_ */