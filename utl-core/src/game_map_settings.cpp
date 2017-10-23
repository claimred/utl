#include "game_map_settings.h"

using namespace utl_core;

GameMapSettings::GameMapSettings()
{
   // Default config
   setDefault();
}

GameMapSettings::~GameMapSettings()
{
}

void GameMapSettings::setDefault()
{
   // Sizes
   landSizeX = 100;
   landSizeY = 50;
   coastWidth = 1;

   // Counters
   trees = 50;
   forests = 10;
   lakes = 20;
   graves = 10;
   treesInForest = 5;
   drygrassareas = 5;


   // TODO: Maybe max trees in forest. 
   treesToForests = trees / (treesInForest * forests);

   // Percentage
   
   dryGrassToGrass = 0.1;
   graveToGrass = 0.1;

   // Areas
   dryGrassMaxArea = 5;
   lakeMaxArea = 5;
   graveMaxArea = 5;
}

bool GameMapSettings::check()
{
   // TODO: Get rid of hardcode.

   if (landSizeX > 300 || landSizeX < 100)
      return false;

   if (landSizeY > 300 || landSizeY < 100)
      return false;

   if (coastWidth < 1 || coastWidth > 5)
      return false;

   if (trees < 30 || trees > 300)
      return false;

   if (forests < 10 || forests > 20)
      return false;

   if (lakes < 10 || lakes > 20)
      return false;

   if (graves < 5 || graves > 15)
      return false;

   if (drygrassareas < 5 || drygrassareas > 10)
      return false;

   if (treesInForest < 3 || treesInForest > 10)
      return false;

   if (dryGrassMaxArea < 3 || dryGrassMaxArea > 9)
      return false;

   if (lakeMaxArea < 3 || lakeMaxArea > 9)
      return false;

   if (graveMaxArea < 3 || graveMaxArea > 9)
      return false;

   return true;
}