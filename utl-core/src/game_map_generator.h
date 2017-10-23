#ifndef _GAME_MAP_GENERATOR_H_
#define _GAME_MAP_GENERATOR_H_

#include <vector>

#include "game_map.h"
#include "game_map_settings.h"

namespace utl_core
{
   typedef std::pair<int, int> intpair;

   class GameMapGenerator
   {
   public:

      GameMapGenerator( GameMap &gm );

      bool generate( bool prepare = true, bool aprepare = true );
      bool generate( GameMapSettings &gms, bool prepare = true, bool aprepare = true );

      ~GameMapGenerator();
   
   private:

      void _generateAreas( GameMapSettings &gms, intpair &a, intpair &b );
      void _fillGeneratedArea( intpair &begin, mapelem::MapElem el );
      void _generateObjects( GameMapSettings &gms );
      void _drawRect( intpair &a, int len, mapelem::MapElem el );
      void _drawLine( intpair &a, intpair &b, mapelem::MapElem el );
      void _generatePoint( intpair &res, intpair &a, intpair &b );
      void _generateRandomArea( mapelem::MapElem el, int area, intpair &start,
            int offset, int pointsnum, bool careful, bool useadd );
      void _generatePoints( std::vector<intpair> &res, int pointsnum, 
         intpair &a, intpair &b, 
         bool (*comparator)( intpair, intpair ), intpair &additional );
      void _generatePointsCareful( std::vector<intpair> &res, int pointsnum, 
                            intpair &a, intpair &b, 
                            bool (*comparator)( intpair, intpair ), 
                            intpair &additional );
      void _generateArea( mapelem::MapElem el, int area, 
         int landsize, int waterdepth );

      bool _checkSurround( int x, int y, mapelem::MapElem el, int size = 1 );
      void _perturbateContour( std::vector<intpair> &contour );

      FILE *_dbgfile;
      GameMap &_gm;
   };
}

#endif /* _GAME_MAP_H_ */