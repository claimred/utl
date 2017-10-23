#ifndef _GAME_H_
#define _GAME_H_

#include <vector>
#include <map>

#include "SDL.h"
#include "SDL_image.h"

#include "path_builder.h"
#include "timer.h"
#include "camera.h"
#include "texture.h"
#include "game_map.h"
#include "utils.h"
#include "resource_manager.h"
#include "gui_manager.h"

namespace utl_core
{
   class Game
   {
   public:

      Game();

      bool init();
      void loopIter();
      bool loop();
      void stop();

      ~Game();

   private:

      

      void _moveCamera( double delta );
      void _moveChar( double delta );
      void _gatherResource( double delta );

      void _getCoords( int x, int y, intpair &res );

      bool _checkSpecial( mapelem::MapElem el );

      void _changeState();
      void _handleEvents();
      void _render();
      
      bool _loadMedia();

      //Draw functions
      void _drawMap();
      void _drawChar();
      void _drawHud();
      void _drawCursor();
      void _drawContextMenu();
      void _drawObjects();
      void _drawAlphaBorderEffect();
      void _drawMinimap();
      void _initMinimap();

      void _drawWholeMap( const char *filename );

      //Event handle functions
      void _handleQuit( SDL_Event &e );
      void _handleKeyDown( SDL_Event &e );
      void _handleMouseMotion( SDL_Event &e );
      void _handleMouseButtonDown( SDL_Event &e );
      void _handleMouseButtonUp( SDL_Event &e );

      //TODO: cleanup                 

      double _charVelocity;
      double _charMarker,
             _charMMarker,
             _waterMarker; 

      double _gatherCounter, _gatherTime;
      double _pathCounter;
      double _pathStep, _oldCameraX, _oldCameraY, _oldCharX, _oldCharY;

      double _cameraCounter;

      bool _walking, _cameraMove, _objSelected, _gathering, _startGathering, _isWireframe, _isDayTime;

      int screenElemW, screenElemH;
         
      int screenW,
          screenH;


      std::vector<MapObject *>::iterator _selIt, _gatherIt;

      int mousex, mousey;

      double _stateVel, _alpha;

      bool _isInit, _quit, _showContextMenu, _showMinimap;

      MapObject *_char;
      GameMap _gm;
      Camera _camera;
      Timer _delta;
      ResourceManager _resources;      
      PathBuilder _pathBuilder;
      GuiManager _guiManager;      

      SDL_Texture *_frameBuffer, *_background, *_minimapTexture, *_minimapBuffer;
      SDL_Window *_window;
      SDL_Renderer *_renderer;
   };
}\

#endif /* _GAME_H_ */