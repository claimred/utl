#include "resource_manager.h"

using namespace utl_core;

ResourceManager::ResourceManager( int resourcesnum )
{
   _resources.resize(resourcesnum);
}

void ResourceManager::_initIndices()
{
   int begin = 0;

   _resourceConfig.insert(MapElemInt(mapelem::CHARACTER1, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTER11, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTER12, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTER13, begin++));
   
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTER2, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTER21, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTER22, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTER23, begin++));
   
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTER3, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTER31, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTER32, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTER33, begin++));
   
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTER4, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTER41, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTER42, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTER43, begin++));
   
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTERM10, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTERM11, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTERM12, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTERM13, begin++));
   
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTERM20, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTERM21, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTERM22, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTERM23, begin++));
   
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTERM30, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTERM31, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTERM32, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTERM33, begin++));
   
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTERM40, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTERM41, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTERM42, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::CHARACTERM43, begin++));
   
   _resourceConfig.insert(MapElemInt(mapelem::GRASS, begin++));

   _resourceConfig.insert(MapElemInt(mapelem::WATER, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::WATER1, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::WATER2, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::WATER3, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::WATER4, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::WATER5, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::WATER6, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::WATER7, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::WATER8, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::WATER9, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::WATER10, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::WATER11, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::WATER12, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::WATER13, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::WATER14, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::WATER15, begin++));

   _resourceConfig.insert(MapElemInt(mapelem::GRAVE, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::SAND, begin++));

   for (int i = begin; i < begin + 34; i++)
      for (int j = 0; j < 16; j++)
      {
         mapelem::MapElem el = static_cast<mapelem::MapElem>(mapelem::SAND + (i - (begin)) * 16 + j + 1);

         int ind = begin + (i - begin) * 16 + j;

         _resourceConfig.insert(MapElemInt(el, ind));
      }

   begin += 34 * 16;

   for (int i = begin; i < begin + 34; i++)
   {
      mapelem::MapElem el = static_cast<mapelem::MapElem>(mapelem::GRASS + (i - (begin - 1)));

      _resourceConfig.insert(MapElemInt(el, i));
   }

   begin += 34;

   for (int i = begin; i < begin + 34; i++)
      for (int j = 0; j < 16; j++)
      {
         mapelem::MapElem el = static_cast<mapelem::MapElem>(mapelem::WATER + 15 + (i - (begin)) * 16 + j + 1);

         int ind = begin + (i - begin) * 16 + j;

         _resourceConfig.insert(MapElemInt(el, ind));
      }

   begin += 34 * 16;

   for (int i = begin; i < begin + 34; i++)
   {
      mapelem::MapElem el = static_cast<mapelem::MapElem>(mapelem::GRAVE + (i - (begin - 1)));

      _resourceConfig.insert(MapElemInt(el, i));
   }

   begin += 34;

   _resourceConfig.insert(MapElemInt(mapelem::TREE, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::TREE2, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::TREE3, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::TREE4, begin++));

   _resourceConfig.insert(MapElemInt(mapelem::FLOWERS1, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::FLOWERS2, begin++));

   _resourceConfig.insert(MapElemInt(mapelem::STONE1, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::STONE2, begin++));

   _resourceConfig.insert(MapElemInt(mapelem::BUSH1, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::BUSH2, begin++));
   
   _resourceConfig.insert(MapElemInt(mapelem::ALPHA1, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::ALPHA2, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::ALPHA3, begin++));
   _resourceConfig.insert(MapElemInt(mapelem::ALPHA4, begin++));
}

bool ResourceManager::loadMedia( SDL_Renderer *_renderer )
{
   int counter = 0;

   //Idle up
   if (!_resources[counter++].loadFromFile("res/new/stand_front.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/stand_front.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/stand_front.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/stand_front.png", _renderer))
      return false;


   //Idle down
   if (!_resources[counter++].loadFromFile("res/new/stand_back.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/stand_back.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/stand_back.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/stand_back.png", _renderer))
      return false;

   //Idle right

   if (!_resources[counter++].loadFromFile("res/new/stand_right.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/stand_right.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/stand_right.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/stand_right.png", _renderer))
      return false;
  
   //Idle left

   if (!_resources[counter++].loadFromFile("res/new/stand_left.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/stand_left.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/stand_left.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/stand_left.png", _renderer))
      return false;


   //Walk down

   if (!_resources[counter++].loadFromFile("res/new/walk_front/walk_front_1.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/walk_front/walk_front_2.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/walk_front/walk_front_3.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/walk_front/walk_front_4.png", _renderer))
      return false;



   // Walk up


   if (!_resources[counter++].loadFromFile("res/new/walk_back/walk_back_1.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/walk_back/walk_back_2.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/walk_back/walk_back_3.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/walk_back/walk_back_4.png", _renderer))
      return false;


   //Walk right

   if (!_resources[counter++].loadFromFile("res/new/walk_right/walk_right_1.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/walk_right/walk_right_2.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/walk_right/walk_right_3.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/walk_right/walk_right_4.png", _renderer))
      return false;


   // Walk left

   if (!_resources[counter++].loadFromFile("res/new/walk_left/walk_left_1.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/walk_left/walk_left_2.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/walk_left/walk_left_3.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/new/walk_left/walk_left_4.png", _renderer))
      return false;



   if (!_resources[counter++].loadFromFile("res/grass01@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/sea02_01@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/sea02_02@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/sea02_03@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/sea02_04@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/sea02_05@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/sea02_06@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/sea02_07@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/sea02_08@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/sea02_09@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/sea02_10@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/sea02_11@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/sea02_12@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/sea02_13@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/sea02_14@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/sea02_15@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/sea02_16@pc.png", _renderer))
      return false;


   if (!_resources[counter++].loadFromFile("res/gravel01@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/sand01@pc.png", _renderer))
      return false;

   char Buf[100];

   for (int i = counter; i < 34 + counter; i++)
   {
      for (int j = 0; j < 16; j++)
      {
         sprintf(Buf, "res/s2w%i/%i.png", (j + 1), i - counter + 1);

         int ind = (i - counter) * 16 + j;

         if (!_resources[counter + ind].loadFromFile(Buf, _renderer))
            return false;
      }      
      
   }
   
   counter += 16 * 34;

   for (int i = counter; i < 34 + counter; i++)
   {
      sprintf(Buf, "res/g2s/%i.png", i - counter + 1);
      if (!_resources[i].loadFromFile(Buf, _renderer))
         return false;
   }

   counter += 34;

   for (int i = counter; i < 34 + counter; i++)
   {
      for (int j = 0; j < 16; j++)
      {
         sprintf(Buf, "res/w2g%i/%i.png", (j + 1), i - counter + 1);

         int ind = (i - counter) * 16 + j;

         if (!_resources[counter + ind].loadFromFile(Buf, _renderer))
            return false;
      }
   }

   counter += 16 * 34;

   for (int i = counter; i < 34 + counter; i++)
   {
      sprintf(Buf, "res/g2gr/%i.png", i - counter + 1);
      if (!_resources[i].loadFromFile(Buf, _renderer))
         return false;
   }

   counter += 34;

   if (!_resources[counter++].loadFromFile("res/Tree_01@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/Tree_02@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/Tree_03@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/Tree_04@pc.png", _renderer))
      return false;


   if (!_resources[counter++].loadFromFile("res/Flowers_01@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/Flowers_02@pc.png", _renderer))
      return false;

   
   if (!_resources[counter++].loadFromFile("res/Stone_01@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/Stone_02@pc.png", _renderer))
      return false;


   if (!_resources[counter++].loadFromFile("res/Bush_01@pc.png", _renderer))
      return false;

   if (!_resources[counter++].loadFromFile("res/Bush_02@pc.png", _renderer))
      return false;

   
   if (!_resources[counter++].loadFromFile("res/test1.png", _renderer))
      return false;

   _resources[counter - 1].setBlendMode(SDL_BLENDMODE_BLEND);

   if (!_resources[counter++].loadFromFile("res/test2.png", _renderer))
      return false;

   _resources[counter - 1].setBlendMode(SDL_BLENDMODE_BLEND);

   if (!_resources[counter++].loadFromFile("res/test3.png", _renderer))
      return false;

   _resources[counter - 1].setBlendMode(SDL_BLENDMODE_BLEND);

   if (!_resources[counter++].loadFromFile("res/test4.png", _renderer))
      return false;

   _resources[counter - 1].setBlendMode(SDL_BLENDMODE_BLEND);
   


   _initIndices();
   return true;
}

Texture & ResourceManager::operator[]( mapelem::MapElem el )
{
   return _resources[_resourceConfig[el]];
}

ResourceManager::~ResourceManager()
{
}