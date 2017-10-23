#include "utils.h"

using namespace utl_core;

namespace utl_core
{
    //TODO: write other abs function and use them. strange behaviour with cmath and math.h
    int abs( int a )
    {
        if (a < 0)
            return -a;
        
        return a;
    }
}

void Utils::buildLine( const intpair &a, const intpair &b, std::vector<intpair> &line )
{
   int x1 = b.first, x0 = a.first, y1 = b.second, y0 = a.second;

    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;

   int err = (dx > dy ? dx : -dy) / 2, e2;

   while (true)
   {
      line.push_back(intpair(x0, y0));

      if (x0 == x1 && y0 == y1) 
         break;

      e2 = err;

      if (e2 > -dx) 
      {
         err -= dy; 
         x0 += sx; 
      }

      if (e2 < dy) 
      {
         err += dx; 
         y0 += sy; 
      }
   }
}

void Utils::buildLine( const intpair &a, const intpair &b, void (*setpixel)( int x, int y ) )
{
   int x1 = b.first, x0 = a.first, y1 = b.second, y0 = a.second;

   int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
   int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 

   int err = (dx > dy ? dx : -dy) / 2, e2;

   if (setpixel == 0)
      throw "Wrong function passed to buildLine";

   while (true)
   {
      setpixel(x0, y0);
      
      if (x0 == x1 && y0 == y1) 
         break;

      e2 = err;

      if (e2 > -dx) 
      {
         err -= dy; 
         x0 += sx; 
      }

      if (e2 < dy) 
      {
         err += dx; 
         y0 += sy; 
      }
   }  
}

int Utils::clamp( int a, int a1, int a2 )
{
   if (a < a1)
      return a1;

   if (a > a2)
      return a2;

   return a;
}