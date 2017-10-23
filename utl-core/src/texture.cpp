#include "SDL_image.h"

#ifdef UTLRELEASE
   #include "physfs.h"
#endif

#ifdef __APPLE__

#include "bundle_helper.h"

#endif /* __APPLE __ */

#include "texture.h"

using namespace utl_core;

Texture::Texture()
{
   _texture = NULL;
   _width = 0;
   _height = 0;
   _data = 0;
}

Texture::~Texture()
{
   free();
}

bool Texture::loadFromFile( std::string path, SDL_Renderer *renderer )
{
   free();

#ifdef UTLRELEASE
   PHYSFS_openRead(path.c_str());
   PHYSFS_file *R_File = PHYSFS_openRead(path.c_str());
   PHYSFS_sint64 Size = PHYSFS_fileLength(R_File);
   _data = new char[PHYSFS_fileLength(R_File)];
   PHYSFS_read(R_File, _data, 1, Size);

   
   SDL_RWops *RW = SDL_RWFromMem(_data, Size);
   SDL_Surface *TempSurface = IMG_Load_RW(RW,0);
   SDL_SetColorKey(TempSurface, SDL_TRUE, SDL_MapRGB(TempSurface->format, 0, 0xFF, 0xFF ));
   SDL_Texture *OptTexture = SDL_CreateTextureFromSurface(renderer, TempSurface);
   _width = TempSurface->w;
   _height = TempSurface->h;
   SDL_FreeSurface(TempSurface);
   _texture = OptTexture;

   return OptTexture != NULL;
#else


   
   
   SDL_Texture *newTexture = NULL;
    
#ifdef __APPLE__    
   char *filename = BundleHelper::getInstance().getPath(path.c_str());
   SDL_Surface *loadedSurface = IMG_Load(filename);
   delete[] filename;
#else
   SDL_Surface *loadedSurface = IMG_Load(path.c_str());
#endif /* __APPLE__ */

   
   
   if (loadedSurface == NULL)
   {
      printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
   }
   else
   {
      SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF ));
      newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

      if (newTexture == NULL)
      {
         printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
      }
      else
      {
         _width = loadedSurface->w;
         _height = loadedSurface->h;
      }

      SDL_FreeSurface(loadedSurface);
   }

   _texture = newTexture;
   return _texture != NULL;

#endif
}

void Texture::free()
{
   if (_texture != NULL)
   {
      SDL_DestroyTexture(_texture);
      _texture = NULL;
      _width = 0;
      _height = 0;
   }
}

void Texture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
   SDL_SetTextureColorMod(_texture, red, green, blue);
}

void Texture::setBlendMode( SDL_BlendMode blending )
{
   SDL_SetTextureBlendMode(_texture, blending);
}

void Texture::setAlpha( Uint8 alpha )
{
   int res = SDL_SetTextureAlphaMod(_texture, alpha);
}

void Texture::render( int x, int y, SDL_Rect* clip, SDL_Renderer *renderer )
{
   SDL_Rect renderQuad = { x, y, _width, _height };

   if (clip != NULL)
   {
      renderQuad.w = clip->w;
      renderQuad.h = clip->h;
   }

   SDL_RenderCopy(renderer, _texture, clip, &renderQuad);
}

int Texture::getWidth()
{
   return _width;
}

int Texture::getHeight()
{
   return _height;
}