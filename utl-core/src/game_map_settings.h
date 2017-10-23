#ifndef _GAME_MAP_SETTINGS_H_
#define _GAME_MAP_SETTINGS_H_

namespace utl_core
{
   struct GameMapSettings
   {
   public:

      // Sizes
      int landSizeX, landSizeY;
      int coastWidth;
      
      // Counters
      int trees;
      int forests;
      int lakes;
      int graves;
      int drygrassareas;
      int treesInForest;

      // Percentage
      double treesToForests;
      double dryGrassToGrass;
      double graveToGrass;


      // Areas
      int dryGrassMaxArea;
      int lakeMaxArea;
      int graveMaxArea;

      GameMapSettings();

      void setDefault();
      bool check();

      ~GameMapSettings();
      
   };
}

#endif /* _GAME_MAP_SETTINGS_H_ */