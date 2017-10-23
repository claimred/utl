#ifndef _GAME_MAP_WRITER_H_
#define _GAME_MAP_WRITER_H_

#include "game_map.h"

namespace utl_core
{
   //TODO: implement abstract writer class

   class GameMapWriter
   {
   public:

      GameMapWriter();

      static bool write( GameMap &gm, const char *filename );
      static bool writeBuffer( GameMap &gm, int *buf );
      static bool writeBinary( GameMap &gm, const char *filename );

      ~GameMapWriter();
   
   private:

      GameMapWriter( const GameMapWriter &gmw );
   };
}

#endif /* _GAME_MAP_WRITER_H_ */