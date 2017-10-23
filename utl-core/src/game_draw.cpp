#include "game.h"
#include "config.h"

using namespace utl_core;

void Game::_initMinimap()
{
   std::pair<int, int> sizes;

   _gm.getSize(sizes);

   int minimap_w = Config::minimapW, minimap_h = Config::minimapH,
       offset_x = _camera.getxOffset() + Config::texW + 20, offset_y = _camera.getyOffset() + Config::texH + 20,
       elem_w = minimap_w / sizes.first, elem_h = minimap_h / sizes.second;
   
   SDL_Rect rect = { offset_x, offset_y, minimap_w, minimap_h };
   int r = 0, g = 0, b = 0;

   
   SDL_SetRenderTarget(_renderer, _minimapTexture);
   SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
   SDL_RenderClear(_renderer);
   SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);
   SDL_RenderDrawRect(_renderer, &rect);

   rect.w = elem_w;
   rect.h = elem_h;

   for (int j = 0; j < sizes.second; j++)
   {
      for (int i = 0; i < sizes.first; i++)
      {
         mapelem::MapElem el = _gm.getElem(i, j);

         g = 255, r = 255, b = 255;

         if (el == mapelem::GRASS || (el >= mapelem::G2S_1 && el <= mapelem::G2S_34))
            r = 79, g = 137, b = 62;

         if ((el >= mapelem::WATER && el <= mapelem::WATER15) ||
            (el >= mapelem::W2G_0101 && el <= mapelem::W2G_3416))
            r = 71, g = 101, b = 161;         

         if (el == mapelem::SAND || (el >= mapelem::S2W_0101 && el <= mapelem::S2W_3416))
            r = 208, g = 192, b = 146; 

         if (el == mapelem::GRAVE || (el >= mapelem::GR2G_1 && mapelem::GR2G_34))
            r = 89, g = 66, b = 39; 
            
         rect.x = i * elem_w;
         rect.y = j * elem_h;

         SDL_SetRenderDrawColor(_renderer, r, g, b, 0);
         SDL_RenderFillRect(_renderer, &rect);
         
      }
   }


}

void Game::_drawMinimap()
{
   std::pair<int, int> sizes;

   _gm.getSize(sizes);

   int elem_w = Config::minimapW / sizes.first, elem_h = Config::minimapH / sizes.second;
   SDL_Rect rect = { 0, 0, elem_w, elem_h };

   rect.x = _char->x / Config::texW * elem_w;
   rect.y = _char->y / Config::texH * elem_h;

   SDL_SetRenderDrawColor(_renderer, 0xFF, 0, 0, 0);
   SDL_RenderFillRect(_renderer, &rect);
}

void Game::_drawHud()
{
   _guiManager.drawHud(_camera.getxOffset(), _camera.getyOffset(), _renderer);
}

void Game::_drawCursor()
{
   int xoffset = _camera.getxOffset(), yoffset = _camera.getyOffset();

   _guiManager.drawCursor(mousex + Config::texW + xoffset, mousey + Config::texH + yoffset, _renderer);
}

void Game::_drawContextMenu()
{
   int xoffset = _camera.getxOffset(), yoffset = _camera.getyOffset();

   _guiManager.drawContextMenu(mousex + xoffset, mousey + yoffset, _renderer);
}

void Game::_drawObjects()
{
   int xoffset = _camera.getxOffset(), yoffset = _camera.getyOffset(),
      i0 = _camera.geti0(), j0 = _camera.getj0();

   std::vector<MapObject *>::iterator it;
   std::vector<MapObject *> *objects = _gm.getObjects();

   int count = 0;

   for (it = _camera.getObjBegin(); it != _camera.getObjEnd(); it++)
   {
      SDL_Rect rect;

      rect.x = (*it)->x + (*it)->xoffset - _camera.getX() + xoffset;
      rect.y = (*it)->y + (*it)->yoffset - _camera.getY() + yoffset;

      if (rect.x + _camera.getX() > i0 * Config::texW &&
          rect.x + _camera.getX() < (i0 + screenElemW + 4) * Config::texW)
      {
         count++;
         rect.w = _resources[(*it)->el].getWidth();
         rect.h = _resources[(*it)->el].getHeight();

         mapelem::MapElem ind;

         ind = (*it)->el;

         if (_objSelected && it == _selIt && !_gathering)
            ind = (*it)->el_sel;

         if ((*it)->el >= mapelem::CHARACTER1 && (*it)->el <= mapelem::CHARACTERM43)
         {
            if (_walking)
               ind = static_cast<mapelem::MapElem>(_char->el + (int)_charMMarker);
            else
               ind = static_cast<mapelem::MapElem>(_char->el + (int)_charMarker);
         }

         _resources[ind].render(rect.x, rect.y, 0, _renderer);
      }

      if (_isWireframe)
      {
        SDL_SetRenderDrawColor(_renderer, 255, 0, 255, 0);
        SDL_RenderDrawRect(_renderer, &rect);
      }
      
   }
}

void Game::_drawMap()
{
   int i0 = _camera.geti0(), j0 = _camera.getj0();  

   std::pair<int, int> sizes;

   _gm.getSize(sizes);

   for (int i = i0; i <= screenElemW + 1 + i0; i++)
      for (int j = j0; j <= screenElemH + 1 + j0; j++)
      {
         mapelem::MapElem el; 

         if (i < 0 || j < 0 || i >= sizes.first || j >= sizes.second)
            el = mapelem::WATER;
         else
            el = _gm.getElem(i, j);

         if (_checkSpecial(el))         
         {
            el = static_cast<mapelem::MapElem>(el + (int)_waterMarker);       


         }

         _resources[el].render((i - i0) * Config::texW, (j - j0) * Config::texH, 0, _renderer);
      }

      if (_isWireframe)
      {
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);

        SDL_Rect a;

        a.x = 0, a.y = 0;
        a.w = Config::texW, a.h = Config::texH;

        for (int i = i0; i <= screenElemW + 1 + i0; i++)
           for (int j = j0; j <= screenElemH + 1 + j0; j++)
           {
              //NB: End of debug grid = end of map
              if (i < 0 || j < 0 || i >= sizes.first || j >= sizes.second)
                 continue;

              SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 0);

              a.w = Config::regW, a.h = Config::regH;

              for (int p = 0; p < Config::countX; p++)
                 for (int q = 0; q < Config::countY; q++)
                 {
                    if (_gm.isMicroWalkable(i * Config::countX + p, j * Config::countY + q))
                       SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 0);
                    else
                       SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 0);

                    a.x = (i - i0) * Config::texW + p * Config::regW;
                    a.y = (j - j0) * Config::texH + q * Config::regH;

                    SDL_RenderDrawRect(_renderer, &a);
                 }

              if (_gm.isMacroWalkable(i, j))
                 SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 0);
              else
                 SDL_SetRenderDrawColor(_renderer, 255, 255, 0, 0);


              a.x = (i - i0) * Config::texW;
              a.y = (j - j0) * Config::texH;
              a.w = Config::texW, a.h = Config::texH;
            
              SDL_RenderDrawRect(_renderer, &a);
           }

           SDL_Rect rect;

           rect.w = 10, rect.h = 10;

           for (int i = 0; i < _pathBuilder.getPathSize(); i++)
           {
              SDL_SetRenderDrawColor(_renderer, 240, 34, 123, 0);

              rect.x = _pathBuilder[i].first - i0 * Config::texW;
              rect.y = _pathBuilder[i].second - j0 * Config::texH;

              SDL_RenderDrawRect(_renderer, &rect);
           }
      }
}

void Game::_drawAlphaBorderEffect()
{
   int xoffset = _camera.getxOffset(), yoffset = _camera.getyOffset(),
      i0 = _camera.geti0(), j0 = _camera.getj0();

   for (int i = 0; i < screenElemW; i++)
   {
      _resources[mapelem::ALPHA1].render(Config::texW + i * Config::texW + _camera.getxOffset(),
         Config::texH + _camera.getyOffset(), 0, _renderer);
      _resources[mapelem::ALPHA2].render(Config::texW + i * Config::texW + _camera.getxOffset(),
         Config::texH + (screenElemH - 1) * Config::texH + _camera.getyOffset(), 0, _renderer);
   }

   for (int j = 0; j < screenElemH; j++)
   {
      _resources[mapelem::ALPHA3].render(Config::texW + _camera.getxOffset(),
         Config::texH + j * Config::texH + _camera.getyOffset(), 0, _renderer);
      _resources[mapelem::ALPHA4].render(Config::texW + (screenElemW - 1) * Config::texW + _camera.getxOffset(),
         Config::texH + j * Config::texH + _camera.getyOffset(), 0, _renderer);
   }
}

void Game::_drawWholeMap( const char *filename )
{
   /* DOESN'T WORK FOR SOME REASON
   std::pair<int, int> size;

   _gm.getSize(size);

   int w = size.first, h = size.second,
       //width = w * Config::texW, height = h * Config::texH;
       width = Config::texW, height = Config::texH;

   SDL_Surface *surf = SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

   SDL_Texture *tex = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
      width, height);
   
   int res = SDL_SetRenderTarget(_renderer, tex);      
   SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
   SDL_RenderClear(_renderer);

   SDL_SetRenderDrawColor(_renderer, 0x00, 0xFF, 0x00, 0xFF);
   
   SDL_Rect rect;

   rect.x = 0;
   rect.y = 0;
   rect.w = 50;
   rect.h = 50;

   SDL_RenderFillRect(_renderer, &rect);

   res = SDL_RenderReadPixels(_renderer, NULL, 0, surf->pixels, surf->pitch);


   //_drawMap();   
   
   

   res = SDL_SaveBMP(surf, filename);


   
   SDL_FreeSurface(surf);
   
   
   SDL_DestroyTexture(tex);*/
}

/************************************************************************/