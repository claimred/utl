#include "lhcore_c.h"

#include "game_map.h"
#include "game_map_generator.h"
#include "game_map_writer.h"
#include "game_map_settings.h"

using namespace utl;

CEXPORT void lhcoreGenerateMap( int *map, int size, int coastwidth, int trees,
                               int lakes, int grave_fields, int drygrass_fields,
                               int trees_in_forest, int lake_area, int gravel_area,
                               int drygrass_area )
{
   GameMapSettings gms;
   gms.landSize = size;

   gms.coastWidth = coastwidth;
   gms.trees = trees;
   gms.lakes = lakes;
   gms.graves = grave_fields;
   gms.drygrassareas = drygrass_fields;
   gms.treesInForest = trees_in_forest;
   gms.lakeMaxArea = lake_area;
   gms.graveMaxArea = gravel_area;
   gms.dryGrassMaxArea = drygrass_area;

   GameMap gm;
   GameMapGenerator gmg(gm);

   gmg.generate(gms, false);

   //map[23] = -1;

   GameMapWriter::writeBuffer(gm, map);

   //GameMapWriter::write(gm, "test.map");
}

CEXPORT void lhcoreSaveMapToFile( int *map, int size, const char *filename )
{
   FILE *F = fopen(filename, "wt");

   if (F == 0)
      return;

   for (int j = 0; j < size; j++)
   {
      for (int i = 0; i < size; i++)
         fprintf(F, "%i", map[j * size + i]);

      fprintf(F, "\n");
   }

   fclose(F);
}

CEXPORT void lhcoreFreeMap( int **map )
{
   delete[] *map;
}