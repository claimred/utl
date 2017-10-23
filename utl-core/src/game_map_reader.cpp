#include "game_map_reader.h"

using namespace utl_core;

GameMapReader::GameMapReader()
{
}

bool GameMapReader::read( GameMap &gm, const char *filename )
{
   FILE *F = fopen(filename, "rt");

   if (F == 0)
      return false;

   std::pair<int, int> sizes;

   fscanf(F, "%i%i", &sizes.first, &sizes.second);

   gm.create(sizes.first, sizes.second);
   gm.initWalkable();

   char Buf[4096];

   fgets(Buf, 4096, F);

   for (int j = 0; j < sizes.second; j++)
   {
      fgets(Buf, 4096, F);

      int offset = 0, i = 0;
      
      while (true)
      {
         int elem = 0, k = 0;
                  
         int res = sscanf(Buf + offset, "%d%n", &elem, &k);          

        

         if (res != 1)
            break;

         offset += k;

         gm.setElem(i++, j, static_cast<mapelem::MapElem>(elem));
      }
      
   
   }
      

   fclose(F);
      

   return true;
}

GameMapReader::~GameMapReader()
{
}