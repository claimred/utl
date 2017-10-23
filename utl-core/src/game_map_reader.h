#ifndef _GAME_MAP_READER_H_
#define _GAME_MAP_READER_H_

#include "game_map.h"

namespace utl_core
{
   class GameMapReader 
   {
   public:

      GameMapReader();

      static bool read( GameMap &gm, const char *filename );
      
      ~GameMapReader();
   
   private:

      GameMapReader( const GameMapReader &gmw );

   };
}
#endif /* _GAME_MAP_READER_H_ */