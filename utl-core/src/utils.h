#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
#include <utility>

namespace utl_core
{
   typedef std::pair<int, int> intpair;

   class Utils
   {   
   public:
      
      static void buildLine( const intpair &a, const intpair &b, std::vector<intpair> &line );
      
      static void buildLine( const intpair &a, const intpair &b, void (*setpixel)( int x, int y ) );

      static int clamp( int a, int a1, int a2 );

   private:

      static void _addToVector( int x, int y );

   };
}

#endif /* _UTILS_H_ */