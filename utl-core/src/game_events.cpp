#include "game.h"
#include "config.h"

using namespace utl_core;

void Game::_handleQuit( SDL_Event &e )
{
   _quit = true;
}

void Game::_handleKeyDown( SDL_Event &e )
{
   switch (e.key.keysym.sym)
   {
   case SDLK_HOME:
      _isDayTime = true;
      break;
   case SDLK_END:
      _isDayTime = !_isDayTime;
      break;
   case SDLK_ESCAPE:
      _quit = true;
      break;
   case SDLK_RIGHT:
      _camera.moveX(1);
      break;
   case SDLK_LEFT:
      _camera.moveX(-1);
      break;
   case SDLK_UP:
      _camera.moveY(-1);
      break;
   case SDLK_DOWN:
      _camera.moveY(1);
      break;
   case SDLK_INSERT:
      _isWireframe = !_isWireframe;
      break;
   case SDLK_TAB:
      _showMinimap = !_showMinimap;
      break;
   case SDLK_PAGEDOWN:
      _camera.moveY(1);
      _camera.moveY(1);
      _camera.moveY(1);
      _camera.moveY(1);
      _camera.moveY(1);
      _camera.moveY(1);
      break;
   case SDLK_PAGEUP:
      _camera.moveY(-1);
      _camera.moveY(-1);
      _camera.moveY(-1);
      _camera.moveY(-1);
      _camera.moveY(-1);
      _camera.moveY(-1);
      break;
   }
}

void Game::_handleMouseMotion( SDL_Event &e )
{
   mousex = e.motion.x;
   mousey = e.motion.y;

   intpair tmp;

   _getCoords(mousex, mousey, tmp);

   bool found = false;
   double coordx = mousex + _camera.getX(), coordy = mousey + _camera.getY();

   std::vector<MapObject *>::iterator it;

   for (it = _camera.getObjBegin(); it != _camera.getObjEnd(); it++)
   {
      double dx = fabs(((*it)->x + Config::regW / 2) - coordx),
               dy = fabs(((*it)->y + Config::regH / 2) - coordy);

      if ((*it)->el == mapelem::TREE || (*it)->el == mapelem::TREE2 ||
         (*it)->el == mapelem::TREE3 || (*it)->el == mapelem::TREE4 ||
         (*it)->el == mapelem::TREE5)
         if (dx < 20.0 && dy < 20.0)
         {
            //_objSelected = true;
            //_selIt = it;
            //found = true;
         }
   }

   if (!found)
      _objSelected = false;
            
   //printf("(%i, %i)\n", tmp.first, tmp.second);
   //printf("(%i, %i)\n", (mousex + _camera.getX()) / Config::texW, (mousey + _camera.getY()) / Config::texH);
}

void Game::_handleMouseButtonDown( SDL_Event &e )
{
   if (e.button.button == SDL_BUTTON_RIGHT)
   {
      _gatherCounter = 0.0;
      _gathering = false, _startGathering = false; 


      SDL_Color color = {0, 255, 0};

   /*      if (_text != 0)
            SDL_DestroyTexture(_text);
 
         TTF_SizeText(_font, "You are walking", &_textRect.w, &_textRect.h);
         SDL_Surface *text_surface = TTF_RenderText_Blended(_font, "You are walking", color);
         _text = SDL_CreateTextureFromSurface(_renderer, text_surface);*/

      int startx = _char->x, starty = _char->y,
            endx = e.motion.x, endy = e.motion.y;

      if (_objSelected)
         if (abs(endx + _camera.getX() - (*_selIt)->x) < Config::regW &&
               abs(endy + _camera.getY() - (*_selIt)->y) < Config::regH)
         {
            _gathering = true;
            _gatherIt = _selIt;
         }

      _pathCounter = 0.0;
      _pathBuilder.clear();

      intpair startpoint, endpoint;

      //micro
      startx -= _camera.getX();
      starty -= _camera.getY();

      _getCoords(startx, starty, startpoint);
      _getCoords(endx, endy, endpoint);

      if (_walking)
      {
         intpair charpos(_char->x, _char->y);
         _pathBuilder.buildPath(startpoint, endpoint, &charpos);
      }
      else
         _pathBuilder.buildPath(startpoint, endpoint);

      if (_pathBuilder.getPathSize() > 0)
      {
         _walking = true; 
         _guiManager.changeText(true);
      }
   }
}

void Game::_handleMouseButtonUp( SDL_Event &e )
{
   if (e.button.button | SDL_BUTTON(3))
   {
      _showContextMenu = false;
      return;
   }
}