#include "config.h"
#include "gui_manager.h"

using namespace utl_core;

GuiManager::GuiManager()
{
   _text = 0;
}

void GuiManager::drawCursor( int x, int y, SDL_Renderer *renderer )
{
   _cursor.render(x, y, 0, renderer);
}

bool GuiManager::loadMedia( SDL_Renderer *renderer )
{
   // Cursor
  
   if (!_cursor.loadFromFile("res/arrow02@pc.png", renderer))
   {
      printf("error loading cursor\n");
      return false;
   }   

   // Weather

   // History

   if (!_ltop.loadFromFile("res/hud/log/history_bg_top.png", renderer))
      return false;

   if (!_lmiddle.loadFromFile("res/hud/log/history_bg_middle.png", renderer))
      return false;

   if (!_lbottom.loadFromFile("res/hud/log/history_bg_bottom.png", renderer))
      return false;

   // Indicators

   if (!_ind.loadFromFile("res/hud/indic_tb/indicator_bg.png", renderer))
      return false;

   if (!_indcold.loadFromFile("res/hud/indic_tb/indiator_icon_freeze.png", renderer))
      return false;

   if (!_indhunger.loadFromFile("res/hud/indic_tb/indiator_icon_starving.png", renderer))
      return false;

   if (!_indenergy.loadFromFile("res/hud/indic_tb/indiator_icon_energy.png", renderer))
      return false;

   if (!_indwater.loadFromFile("res/hud/indic_tb/indiator_icon_thirst.png", renderer))
      return false;

   if (!_indhealth.loadFromFile("res/hud/indic_tb/indicator_life.png", renderer))
      return false;

   // Menu

   if (!_mbutton.loadFromFile("res/hud/menu_tb/button_sm_normal.png", renderer))
      return false;

   if (!_mman.loadFromFile("res/hud/menu_tb/button_icon_man.png", renderer))
      return false;

   /*if (!_mgear.loadFromFile("res/hud/menu_tb/button_icon_gear.png", renderer))
      return false;

   if (!_mcraft.loadFromFile("res/hud/menu_tb/button_icon_craft.png", renderer))
      return false;

   if (!_minventory.loadFromFile("res/hud/menu_tb/button_icon_inventory.png", renderer))
      return false;*/

   // Inventory

   
   if (!_invslot.loadFromFile("res/hud/res_tb/inventory_slot.png", renderer))
      return false;

   
#ifndef __EMSCRIPTEN__
   _loadFont(renderer);
#endif /* __EMSCRIPTEN__ */

   return true;
}

bool GuiManager::_loadFont( SDL_Renderer *renderer )
{
#ifndef __EMSCRIPTEN__
   _font = TTF_OpenFont("consolab.ttf", 12);

   if (_font == 0)
   {
      printf("error loading font: %s\n", TTF_GetError());
      return false;
   }

   TTF_SetFontOutline(_font, 0);

   SDL_Color color = {255, 255, 0};

   if (_text != 0)
      SDL_DestroyTexture(_text);

   TTF_SizeText(_font, "You are staying", &_textRect.w, &_textRect.h);
   SDL_Surface *text_surface = TTF_RenderText_Blended(_font, "You are staying", color);
   _text = SDL_CreateTextureFromSurface(renderer, text_surface);

   color.r = 0;
   
   TTF_SizeText(_font, "You are staying", &_textRect.w, &_textRect.h);
   text_surface = TTF_RenderText_Blended(_font, "You are walking", color);
   _textW = SDL_CreateTextureFromSurface(renderer, text_surface);

   SDL_SetTextureAlphaMod(_textW, SDL_ALPHA_TRANSPARENT);
#endif /* __EMSCRIPTEN__ */

   return true;
}

void GuiManager::destroyFont()
{
#ifndef __EMSCRIPTEN__
   if (_text != 0)
      SDL_DestroyTexture(_text);

   TTF_CloseFont(_font);
#endif /* __EMSCRIPTEN__ */
}

void GuiManager::drawHud( int xoffset, int yoffset, SDL_Renderer *renderer )
{
   /* Weather toolbar */

   xoffset += Config::texW, yoffset += Config::texH;

   SDL_Rect clip;

   clip.x = 0, clip.y = 0;
   clip.w = 126, clip.h = 126 / 2;

   //_wbg.render(screenW - 138 + xoffset, 6 + yoffset, &clip, renderer);
   //_wind.render(screenW - 138 - 6 + xoffset, 0 + yoffset, 0, renderer);

   /* History */

   int x = 6 + xoffset, y = screenH - 16 - 6 + yoffset;

   _lbottom.render(x, y, 0, renderer);
   _lmiddle.render(x, y - 16, 0, renderer);

   _textRect.x = x + 15, _textRect.y = y - 16;

   if (_text != 0 && _textW != 0)
   {
      SDL_RenderCopy(renderer, _text, 0, &_textRect);
      SDL_RenderCopy(renderer, _textW, 0, &_textRect);
   }

   _ltop.render(x, y - 2 * 16, 0,  renderer);

   /* Indicators */

   int intern_xoffset = 9, intern_yoffset = 8;
   x = 6 + xoffset, y = 6 + yoffset;

   _ind.render(x, y, 0, renderer);
   _indhealth.render(x + intern_xoffset, y + intern_yoffset, 0, renderer);

   x += 6 + 57;

   _ind.render(x, y, 0, renderer);
   _indwater.render(x + intern_xoffset, y + intern_yoffset, 0, renderer);

   x += 6 + 57;

   _ind.render(x, y, 0, renderer);
   _indhunger.render(x + intern_xoffset, y + intern_yoffset, 0, renderer);

   x += 6 + 57;

   _ind.render(x, y, 0, renderer);
   _indcold.render(x + intern_xoffset, y + intern_yoffset, 0, renderer);

   x += 6 + 57;

   _ind.render(x, y, 0, renderer);
   _indenergy.render(x + intern_xoffset, y + intern_yoffset, 0, renderer);

   // Menu toolbar

   x = screenW + xoffset - 6 - 51,
   y = screenH + yoffset - 6 - 50;

   _mbutton.render(x, y, 0, renderer);
   //_mgear.render(x + 2, y, 0, renderer);

   y -= 6 + 50;

   _mbutton.render(x, y, 0, renderer);
   //_mcraft.render(x + 2, y, 0, renderer);

   y -= 6 + 50;

   _mbutton.render(x, y, 0, renderer);
   //_minventory.render(x + 2, y, 0, renderer);

   y -= 6 + 50;

   _mbutton.render(x, y, 0, renderer);
   _mman.render(x + 2, y, 0, renderer);

   y -= 6 + 50;

   // Inventory toolbar

   x = 213 + 6 + 96 + xoffset;
   y = screenH - 68 + yoffset;

   _invbar.render(x, y, 0, renderer);

   x += 22;
   y += 7;

   _invslot.render(x, y, 0, renderer);

   for (int i = 0; i < 9; i++)
   {
      x += 20 + 29 + 6; 

      _invslot.render(x, y, 0, renderer);
   }   
}

void GuiManager::drawContextMenu( int x, int y, SDL_Renderer *renderer )
{
   _cmtop.render(x, y, 0, renderer);
   _cmmiddle.render(x, y + 35, 0, renderer);
   _cmbottom.render(x + 1, y + 35 + 29, 0, renderer);
}

void GuiManager::changeText( bool flag )
{
#ifndef __EMSCRIPTEN__
   if (flag)
   {
      SDL_SetTextureAlphaMod(_text, SDL_ALPHA_TRANSPARENT);
      SDL_SetTextureAlphaMod(_textW, SDL_ALPHA_OPAQUE);
   }
   else
   {
      SDL_SetTextureAlphaMod(_text, SDL_ALPHA_OPAQUE);
      SDL_SetTextureAlphaMod(_textW, SDL_ALPHA_TRANSPARENT);
   }
#endif /* __EMSCRIPTEN__ */
}

GuiManager::~GuiManager()
{
}