#ifndef _GAME_MAP_PREPARER_H_
#define _GAME_MAP_PREPARER_H_

#include <vector>

#include "game_map.h"

namespace utl_core
{
   class GameMapPreparer
   {
   public:
      
      GameMapPreparer( GameMap &gm );

      void prepare( bool aprepare = true );

      ~GameMapPreparer();

   private:

      int _count( int i, int j, mapelem::MapElem el );
      void _prepare( mapelem::MapElem el1, mapelem::MapElem el2, bool switcher = true, 
                     int offset = 0, bool special = false );
      void _initConf();
      void _markWalkable();
      void _markCell( bool *conf, int i, int j, bool inv );
      
      mapelem::MapElem _conf[256];
      GameMap &_gm;
   };
}

#endif /* _GAME_MAP_PREPARER_H_ */