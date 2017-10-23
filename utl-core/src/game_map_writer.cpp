#include <cstdio>

#include "game_map_writer.h"

using namespace utl_core;

GameMapWriter::GameMapWriter()
{
}

bool GameMapWriter::writeBuffer( GameMap &gm, int *buf )
{
   std::pair<int, int> sizes;

   gm.getSize(sizes);


   for (int j = 0; j < sizes.second; j++)
   {
      for (int i = 0; i < sizes.first; i++)
      {
         buf[i * sizes.second + j] = gm.getElem(i, j);
      }
   }

   return true;
}

bool GameMapWriter::write( GameMap &gm, const char *filename )
{
   FILE *F = fopen(filename, "wt");

   if (F == 0)
      return false;

   std::pair<int, int> sizes;

   gm.getSize(sizes);

   fprintf(F, "%i %i\n", sizes.first, sizes.second);

   for (int j = 0; j < sizes.second; j++)
   {
      for (int i = 0; i < sizes.first; i++)
      {
         fprintf(F, "%3i ", gm.getElem(i, j));
      }

      fprintf(F, "\n");
   }

   fclose(F);
   
   return true;
}

bool GameMapWriter::writeBinary( GameMap &gm, const char *filename )
{
      return true;
}

GameMapWriter::~GameMapWriter()
{

}