#ifndef _GUI_MANAGER_H_
#define _GUI_MANAGER_H_

#include "SDL.h"
#include "SDL_image.h"

#ifndef __EMSCRIPTEN__
#include "SDL_ttf.h"
#endif

#include "texture.h"

namespace utl_core
{
   class GuiManager
   {
   public:

      GuiManager();

      void drawCursor( int x, int y, SDL_Renderer *renderer );
      void drawHud( int xoffset, int yoffset, SDL_Renderer *renderer );
      void drawContextMenu( int x, int y, SDL_Renderer *renderer );

      void changeText( bool flag );

      void destroyFont();

      bool loadMedia( SDL_Renderer *renderer );

      ~GuiManager();

      int screenW, screenH;

   private:

      bool _loadFont( SDL_Renderer *renderer );

      

      // Weather toolbar
      Texture _wbg, _wind;

      // History toolbar
      Texture _ltop, _lmiddle, _lbottom;

      // Indicators toolbar
      Texture _ind, _indhealth, _indhunger, _indenergy, _indcold, _indwater;

      // Menu toolbar
      Texture _mbutton, _mman, _mcraft, _mgear, _minventory;

      // Inventory toolbar
      Texture _invbar, _invslot;

      // Context menu
      Texture _cmbottom, _cmtop, _cmmiddle;

      // Cursor
      Texture _cursor;
      
#ifndef __EMSCRIPTEN__
      TTF_Font *_font;
#endif /* __EMSCRIPTEN__ */

      SDL_Rect _textRect;
      SDL_Texture *_text, *_textW;
   };
};

#endif /* _GUI_MANAGER_H_ */