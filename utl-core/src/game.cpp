
#include <cmath>
#include <cstdio>

#include "game.h"
#include "game_map.h"
#include "game_map_generator.h"
#include "game_map_preparer.h"
#include "game_map_writer.h"
#include "camera.h"
#include "config.h"

using namespace utl_core;

//TODO:
Game::Game() : _pathBuilder(_gm), _resources(1500)
/*Game::Game() : _pathBuilder(_gm), _resources(6 + 34 + 34 + 34 + 34 + 34 + 11 + 
                                             374 + 374 + 24 + 48 + 4 + 3 + 15 + 2 + 4 + 5 + 5 + 4)*/
{
   _isInit = false;
   _window = 0;
   _renderer = 0;
   _quit = false;
}

bool Game::init()
{
   _alpha = 0.0;
   _isDayTime = true;
   _showMinimap = false;
   _objSelected = false;   
   _walking = false;
   _cameraMove = false;
   _showContextMenu = false;
   _gathering = false;
   _startGathering = false;

   _isWireframe = false;
   
   _charVelocity = 340.6;
   _stateVel = 11.0;

   _charMMarker = 0.0;   
   _waterMarker = 0.0;
   _charMarker = 0.0;

   if (SDL_Init(SDL_INIT_VIDEO) < 0)
   {
      printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
      return false;
   }
   else
   {
      SDL_DisplayMode dm;

      SDL_GetDesktopDisplayMode(0, &dm);

      screenElemW = dm.w / Config::texW - 1;
      screenElemH = dm.h / Config::texH - 1;

      screenW = screenElemW * Config::texW;
      screenH = screenElemH * Config::texH;
      

      

      if (!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1"))
         printf( "Warning: VSync not enabled!" );

      if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
         printf( "Warning: Linear texture filtering not enabled!" );

      _window = SDL_CreateWindow("utl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        screenW, screenH, SDL_WINDOW_SHOWN);
      
      if (_window == NULL)
      {
         printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
         return false;
      }
      else
      {
         _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

         if (_renderer == NULL)
         {
            printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            return false;
         }
         else
         {
            SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

            int imgFlags = IMG_INIT_PNG;


            //Emscripten bug?
#ifndef __EMSCRIPTEN__
            if (!(IMG_Init(imgFlags) & imgFlags))
            {
               printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
               return false;
            }
#endif /* __EMSCRIPTEN__ */

            _frameBuffer = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 
               (screenElemW + 2) * Config::texW, 
               (screenElemH + 2) * Config::texH);

            SDL_SetTextureBlendMode(_frameBuffer, SDL_BLENDMODE_NONE);

            if (_frameBuffer == NULL)
            {
               printf("Frame buffer could not be created! SDL Error: %s\n", SDL_GetError());
               return false;
            }

            _background = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
               screenW, screenH);
            
            SDL_SetTextureBlendMode(_background, SDL_BLENDMODE_BLEND);
            SDL_SetTextureAlphaMod(_background, SDL_ALPHA_TRANSPARENT);

            SDL_SetRenderTarget(_renderer, _background);
            SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);
            SDL_RenderClear(_renderer);

            if (_background == NULL)
            {
               printf("Minimap buffer could not be created! SDL Error: %s\n", SDL_GetError());
               return false;
            }

            _minimapTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
               Config::minimapW, Config::minimapH);

            _minimapBuffer = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
               Config::minimapW, Config::minimapH);

            

         }
      }
   }

   SDL_ShowCursor(0);
   SDL_DisableScreenSaver();   
   SDL_GetMouseState(&mousex, &mousey);

   //TODO: Compile SDL2_ttf to use with em++
#ifndef __EMSCRIPTEN__
   if (!TTF_WasInit() && TTF_Init() == -1) 
   {
      printf("TTF_Init: %s\n", TTF_GetError());
   }
#endif /* __EMSCRIPTEN__ */

   if (!_loadMedia())
   {
      printf("Problem in resources loading \n");
      return false;
   }

   GameMapGenerator gmg(_gm);

   gmg.generate(true, false);

/*   GameMapReader gmr;

   gmr.read(_gm, "testmap2.map");
   

   GameMapWriter gmw;

   gmw.write(_gm, "testmap.map");*/

   GameMapPreparer gmp(_gm);

   gmp.prepare(true);




   std::pair<int, int> sizes;

   _gm.getSize(sizes);


   int i = 50, j = sizes.second - 5;
   bool found = false;

   for (j = 0; j < sizes.second; j++)
   {
      for (i = 0; i < sizes.first; i++)
      {
         mapelem::MapElem el = _gm.getElem(i, j);
         
         if (el == mapelem::GRASS)
         {
            found = true;
            break;
         }    
      }

      if (found)
         break;
   }

   i = 80, j = 40;

   MapObject tmp;

   tmp.x = i * Config::texW + 0 * Config::regW + Config::regW / 2;
   tmp.y = j * Config::texH + 0 * Config::regH + Config::regH / 2;
   tmp.xoffset = Config::texW / 2; Config::texW - 1 * Config::regW - Config::regW / 2;
   tmp.yoffset = -Config::regH;
   tmp.el = mapelem::CHARACTER1;   
   _char = _gm.addObject(tmp);

   i = 21;
   j = 21;

   tmp.x = i * Config::texW + 0 * Config::regW + Config::regW / 2;
   tmp.y = j * Config::texH + 0 * Config::regH + Config::regH / 2;
   tmp.xoffset = Config::texW / 2; Config::texW - 1 * Config::regW - Config::regW / 2;
   tmp.yoffset = -Config::regH;
   tmp.el = mapelem::STONE1;   
   _gm.addObject(tmp);

//    tmp.x = i * Config::texW + 1 * Config::regW;
//    tmp.y = j * Config::texH + 2 * Config::regH;
//    _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 2, false);
//    _gm.setWalkable(i * Config::countX + 2, j * Config::countY + 2, false);
//    _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 1, false);
//    _gm.setWalkable(i * Config::countX + 2, j * Config::countY + 1, false);
//    tmp.xoffset = Config::texW - 2.0 * Config::regW;
//    tmp.yoffset = Config::texH - 14 * Config::regH;
//    tmp.el = mapelem::TREE5;


//    tmp.x = i * Config::texW + 1 * Config::regW;
//    tmp.y = j * Config::texH + 2 * Config::regH;
//    _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 2, false);
//    _gm.setWalkable(i * Config::countX + 2, j * Config::countY + 2, false);
//    tmp.xoffset = Config::texW - 1.5 * Config::regW;
//    tmp.yoffset = Config::texH - 13 * Config::regH;
//    tmp.el = mapelem::TREE4;
// 
// 
//    tmp.x = i * Config::texW + 1 * Config::regW;
//    tmp.y = j * Config::texH + 2 * Config::regH;
//    _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 2, false);
//    _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 1, false);
//    tmp.xoffset = Config::texW - 1 * Config::regW;
//    tmp.yoffset = Config::texH - 9 * Config::regH;
//    tmp.el = mapelem::TREE3;
// 
//  
/*   tmp.x = i * Config::texW + 1 * Config::regW;
   tmp.y = j * Config::texH + 2 * Config::regH;
   _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 2, false);
   _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 1, false);
   tmp.xoffset = Config::texW - 1 * Config::regW;
   tmp.yoffset = Config::texH - 9 * Config::regH;
   tmp.el = mapelem::TREE;
   tmp.el_sel = mapelem::TREE1_SEL;
   _gm.addObject(tmp);*/

// 
//    tmp.x = i * Config::texW + 1 * Config::regW;
//    tmp.y = j * Config::texH + 2 * Config::regH;
//    _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 2, false);
//    _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 1, false);
//    _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 0, false);
//    tmp.xoffset = Config::texW - 1.5 * Config::regW;
//    tmp.yoffset = Config::texH - 10 * Config::regH;
//    tmp.el = mapelem::TREE2;

//   _gm.addObject(tmp);

   _gm.updateObjects();

   int a = 2 + 1;   


   

//    int offset = 12, i1 = i * Config::countX + 1, j1 = j * Config::countY + 2;
//    int tmpx = i * Config::countX - offset, tmpy = j * Config::countY - offset;
// 
//    for (int k = tmpx; k < tmpx + 2 * offset; k++)
//    {
//       _gm.setWalkable(k, tmpy, false);
//       _gm.setWalkable(k, tmpy + 2 * offset, false);
//    }
// 
//    for (int k = tmpy; k < tmpy + 2 * offset; k++)
//    {
//       _gm.setWalkable(tmpx, k, false);
//       _gm.setWalkable(tmpx + 2 * offset - 1, k, false);
//    }
// 
//    _gm.setWalkable(i1, j1 - 1, false);
//    _gm.setWalkable(i1, j1 - 2, false);
//    _gm.setWalkable(i1 - 1, j1 - 2, false);
//    _gm.setWalkable(i1 - 1, j1 - 1, false);
//    _gm.setWalkable(i1 - 1, j1, false);
//    _gm.setWalkable(i1 - 1, j1 + 1, false);
//    _gm.setWalkable(i1 - 1, j1 + 2, false);
//    _gm.setWalkable(i1 - 1, j1 + 3, false);
//    _gm.setWalkable(i1 - 1, j1 + 4, false);
//    _gm.setWalkable(i1 - 2, j1 + 3, false);
//    _gm.setWalkable(i1 - 3, j1 + 3, false);
//    _gm.setWalkable(i1 - 4, j1 + 3, false);

   _camera.screenElemH = screenElemH;
   _camera.setObjsRef(_gm.getObjects());
 
   _camera.setBorderX((sizes.first - 1) * Config::texW - screenElemW * Config::texW);
   _camera.setBorderY((sizes.second - 1) * Config::texH - screenElemH * Config::texH);

   _camera.setX(_char->x - screenElemW / 2 * Config::texW);
   _camera.setY(_char->y - screenElemH / 2 * Config::texH);

   _camera.setStepX(10);
   _camera.setStepY(10);   

   _isInit = true;

   _selIt = _gm.getObjects()->end();
   _gatherIt = _gm.getObjects()->end();

   _gatherCounter = 0;
   _gatherTime = 3.0;

   _initMinimap();

   _drawWholeMap("world.bmp");

   return true;
}

bool Game::_loadMedia()
{
   if (!_resources.loadMedia(_renderer))
      return false;

   if (!_guiManager.loadMedia(_renderer))
      return false;  

   _guiManager.screenW = screenW;
   _guiManager.screenH = screenH;

   return true;
}

bool Game::_checkSpecial( mapelem::MapElem el )
{
   if (el == mapelem::WATER)
      return true;

   if (el >= mapelem::S2W_0101 && el <= mapelem::S2W_3416)
      return true;

   if (el >= mapelem::W2G_0101 && el <= mapelem::W2G_3416)
      return true;

   return false;
}

void Game::_moveChar( double delta )
{
   _pathCounter += _charVelocity * delta;

   if (_pathCounter > _pathBuilder.getPathSize() - 1)
   {
      _pathCounter = _pathBuilder.getPathSize() - 1;

      //TODO: How is this possible?
      if (_pathCounter < 0)
         return;

      _char->x = _pathBuilder[_pathCounter].first;
      _char->y = _pathBuilder[_pathCounter].second;
      _walking = false;
      _guiManager.changeText(false);

      if (_char->el >= mapelem::CHARACTERM10 && _char->el <= mapelem::CHARACTERM13)
         _char->el = mapelem::CHARACTER1;

      if (_char->el >= mapelem::CHARACTERM20 && _char->el <= mapelem::CHARACTERM23)
         _char->el = mapelem::CHARACTER2;

      if (_char->el >= mapelem::CHARACTERM30 && _char->el <= mapelem::CHARACTERM33)
         _char->el = mapelem::CHARACTER3;

      if (_char->el >= mapelem::CHARACTERM40 && _char->el <= mapelem::CHARACTERM43)
         _char->el = mapelem::CHARACTER4;  

      if (_gathering)
      {
         _startGathering = true;
      }

      return;
   }

   _char->x = _pathBuilder[_pathCounter].first;
   _char->y = _pathBuilder[_pathCounter].second;

   _char->el = _pathBuilder.getCurrentState(_pathCounter);
}

void Game::_moveCamera( double delta )
{
   if (_cameraCounter >= 1)
   {
      _cameraCounter = 0;
      _cameraMove = false;
   }

   if (_cameraMove)
   {      
      _camera.setX(_oldCameraX * (1 - _cameraCounter) + 
         (_oldCharX - screenElemW / 2 * Config::texW) * _cameraCounter);
      _camera.setY(_oldCameraY * (1 - _cameraCounter) + 
         (_oldCharY - screenElemH / 2 * Config::texH) * _cameraCounter);
      _cameraCounter += delta;      
   }

   if (!_cameraMove)
   {
      if (_char->x >= (screenElemW - 1) * Config::texW + _camera.getX() ||
         _char->x <= 1 * Config::texW + _camera.getX()  ||
         _char->y >= (screenElemH - 1) * Config::texH + _camera.getY() ||
         _char->y <= 1 * Config::texH + _camera.getY())
      {
         _cameraMove = true;
         _cameraCounter = 0;
         _oldCameraX = _camera.getX();
         _oldCameraY = _camera.getY(); 
         _oldCharX = _char->x;
         _oldCharY = _char->y;
      }
   }
}

void Game::_getCoords( int x, int y, intpair &res )
{
   int tmpx = x + _camera.getX(),
       tmpy = y + _camera.getY();

   int i_map = tmpx / Config::texW, j_map = tmpy / Config::texH;

   int offx = tmpx - i_map * Config::texW,
       offy = tmpy - j_map * Config::texH;

   res.first = i_map * Config::countX + (int)(offx / Config::regW);
   res.second = j_map * Config::countY + (int)(offy / Config::regH);
}

void Game::_gatherResource( double delta )
{
   _gatherCounter += delta;

   if (_gatherCounter >= _gatherTime)
   {
      _gathering = false, _startGathering = false;
      _gatherCounter = 0.0;
      _objSelected = false;
      printf("Gathering complete\n");
      
      if ((*_gatherIt)->el == mapelem::TREE)
         (*_gatherIt)->el = mapelem::STOMP1;

      if ((*_gatherIt)->el == mapelem::TREE2)
         (*_gatherIt)->el = mapelem::STOMP2;

      if ((*_gatherIt)->el == mapelem::TREE3)
         (*_gatherIt)->el = mapelem::STOMP3;

      if ((*_gatherIt)->el == mapelem::TREE4)
         (*_gatherIt)->el = mapelem::STOMP4;

      if ((*_gatherIt)->el == mapelem::TREE5)
         (*_gatherIt)->el = mapelem::STOMP5;
      
      return;
   }

   printf("Gathering (%i, %i)... %.2lf \n", (*_selIt)->x, (*_selIt)->y, 
      _gatherCounter / _gatherTime * 100);
}

#define UTL_HANDLE_EVENT(e, name, f) if (e.type == name) { f(e); }

void Game::_handleEvents()
{
   int i0 = _camera.geti0(), j0 = _camera.getj0(), 
      xoffset = _camera.getxOffset(), yoffset = _camera.getyOffset();
   SDL_Event e;

   while (SDL_PollEvent(&e) != 0)
   {
      UTL_HANDLE_EVENT(e, SDL_QUIT, _handleQuit);
      UTL_HANDLE_EVENT(e, SDL_MOUSEMOTION, _handleMouseMotion);
      UTL_HANDLE_EVENT(e, SDL_MOUSEBUTTONUP, _handleMouseButtonUp);
      UTL_HANDLE_EVENT(e, SDL_MOUSEBUTTONDOWN, _handleMouseButtonDown);
      UTL_HANDLE_EVENT(e, SDL_KEYDOWN, _handleKeyDown);
   }
}

void Game::_changeState()
{
   double delta = _delta.getTicks() / 1000.f;
  
   _waterMarker += _stateVel * delta;

   if (_waterMarker > 15.999)
      _waterMarker = 0;

   if (!_walking)
   {
      _charMarker += (4.0) * delta;
      
      if (_charMarker > 4)
         _charMarker = 0;
   }

   if (_walking)
   {
      _charMMarker += 12.0 * delta;

      if (_charMMarker > 3.999)
         _charMMarker = 0;

      _moveChar(delta);
      _gm.updateObjects();
   }

   _moveCamera(delta);
   
   if (_startGathering)
   {
      _gatherResource(delta);
   }

   if (!_isDayTime)
   {
      if (_alpha < 180.0)
      {
         _alpha += 120.0 * delta;
         SDL_SetTextureAlphaMod(_background, _alpha);
      }
   }
   else
   {
      if (_alpha > 0.0)
      {
         _alpha -= 120.0 * delta;

         if (_alpha < 0)
            _alpha = 0.0;

         SDL_SetTextureAlphaMod(_background, _alpha);

         
      }
   }

   
   //SDL_SetTextureAlphaMod(_background, _alpha);

   _delta.start();
}

//TODO: rework logic
void Game::_render()
{
   SDL_Rect rect;

   if (_showMinimap)
   {
      SDL_SetRenderTarget(_renderer, _minimapBuffer);
      SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
      SDL_RenderClear(_renderer);

      rect.x = 0;
      rect.y = 0;
      rect.w = Config::minimapW;
      rect.h = Config::minimapH;

      SDL_RenderCopy(_renderer, _minimapTexture, &rect, NULL);

      _drawMinimap();
   }

   SDL_SetRenderTarget(_renderer, _frameBuffer);
   SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
   SDL_RenderClear(_renderer);

   _drawMap();
   _drawObjects();
   _drawAlphaBorderEffect();  
   _drawHud();

   if (_showContextMenu)
      _drawContextMenu();

   _drawCursor();

   SDL_SetRenderTarget(_renderer, NULL);
   SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
   SDL_RenderClear(_renderer);   

   

   rect.x = Config::texW + _camera.getxOffset();
   rect.y = Config::texH + _camera.getyOffset();
   rect.w = screenW;
   rect.h = screenH;
   
   SDL_RenderCopy(_renderer, _frameBuffer, &rect, NULL);

   if (_showMinimap)
   {
      rect.x = 8;
      rect.y = 78;
      rect.w = Config::minimapW;
      rect.h = Config::minimapH;

      SDL_RenderCopy(_renderer, _minimapBuffer, NULL, &rect);
   }

   SDL_RenderCopy(_renderer, _background, NULL, NULL);
   

   SDL_RenderPresent(_renderer);   
}

void Game::loopIter()
{
   //Events
   _handleEvents();

   //Logic
   _changeState();

   //Render
   _render();
}

bool Game::loop()
{
   if (!_isInit)
      return false;

   _delta.start();

   while (!_quit)
      loopIter();

   return true;
}

void Game::stop()
{
   SDL_DestroyWindow(_window);
   SDL_DestroyTexture(_frameBuffer);
   SDL_DestroyTexture(_background);
   SDL_DestroyTexture(_minimapTexture);
   SDL_DestroyTexture(_minimapBuffer);
   SDL_DestroyRenderer(_renderer);

   _guiManager.destroyFont();
   
   _window = 0;
   _renderer = 0;

#ifndef __EMSCRIPTEN__
   TTF_Quit();
#endif /* __EMSCRIPTEN__ */

   IMG_Quit();
   SDL_Quit();
   _isInit = false;
}

Game::~Game()
{
   if (_isInit)
      stop();
}