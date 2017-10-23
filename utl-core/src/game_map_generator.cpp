#include <stack>
#include <list>
#include <algorithm>
#include <utility>
#include <time.h>
#include <math.h>
#include <cstdlib>
#include <cmath>

#include "config.h"
#include "game_map.h"
#include "game_map_generator.h"
#include "game_map_preparer.h"
#include "game_map_writer.h"

using namespace utl_core;

GameMapGenerator::GameMapGenerator( GameMap &gm ) : _gm(gm) 
{
  // _dbgfile = fopen("dbgfile.txt", "wt");
   srand((int)time(NULL));
}

void GameMapGenerator::_perturbateContour( std::vector<intpair> &contour )
{

}

bool utl_core::GameMapGenerator::_checkSurround( int x, int y, mapelem::MapElem el, int size )
{
   for (int a = x - size; a <= x + size; a++)
      for (int b = y - size; b <= y + size; b++)
      {
         if (a == x && b == y)
            continue;

         if (_gm.getElem(a, b) != el)
            return false;
      }

   return true;
}

bool GameMapGenerator::generate( GameMapSettings &gms, bool prepare, bool aprepare )
{
   //if (!gms.check())
     // return false;

   if (_gm.isInit())
      //TODO: Exception or friend class maybe? See also comment near "gamemap::
      //create() declaration.
      return false;

   gms.lakes = 15;

   _gm.create(gms.landSizeX, gms.landSizeY);
   _gm.initWalkable();

   int map_size = gms.landSizeX * gms.landSizeY;

   _gm.fill(mapelem::GRASS);

   int waterdepth = 10, sand_offset = 5, sand_points = 19, offset = 0,
       grass_offset = 5, grass_points = 10,
       land_area = std::min(gms.landSizeX, gms.landSizeY) - 2 * waterdepth,
       grass_area = land_area - 2 * grass_offset - 2 * offset;

   //TODO: grass begin dependent on grass_offset? nonnono
   intpair sand_begin(waterdepth, waterdepth),
           grass_begin(sand_begin.first + grass_offset + offset, sand_begin.second + grass_offset + offset);

   //_generateRandomArea(mapelem::SAND, land_area, sand_begin, sand_offset, sand_points, false, false);
   //_generateRandomArea(mapelem::GRASS, grass_area, grass_begin, grass_offset, grass_points, true, false);     

   intpair tmp(grass_begin.first + grass_area, grass_begin.second + grass_area);

   //_generateAreas(gms, grass_begin, tmp);

   if (prepare)
      GameMapPreparer(_gm).prepare(aprepare);
   
   gms.trees = 300;

   //_generateObjects(gms);
   
   return true;
}

void GameMapGenerator::_drawRect( intpair &a, int len, mapelem::MapElem el )
{
   for (int i = 0; i < len; i++)
   {
      _gm.setElem(a.first + i, a.second, el);
      _gm.setElem(a.first + i, a.second + len, el);
      
      _gm.setElem(a.first, a.second + i, el);
      _gm.setElem(a.first+ len, a.second + i, el);
   }
}

void GameMapGenerator::_generateObjects( GameMapSettings &gms )
{
   std::pair<int, int> sizes;

   _gm.getSize(sizes);

   int trees = gms.trees, rocks = 100, flowers = 100, bushes = 100;
   std::vector<intpair> sand_free_points, grass_free_points, grave_free_points;

   for (int j = 0; j < sizes.second; j++)
      for (int i = 0; i < sizes.first; i++)
      {
         mapelem::MapElem el = _gm.getElem(i, j);

         if (el == mapelem::WATER)
            continue;

         if (el >= mapelem::S2W_0101 && el <= mapelem::S2W_3416)
            continue;

         if (el >= mapelem::W2G_0101 && el <= mapelem::W2G_3416)
            continue;

         intpair point(i, j);

         if (el == mapelem::SAND || (el >= mapelem::G2S_1 && el <= mapelem::G2S_34))
            sand_free_points.push_back(point);

         if (el == mapelem::GRASS)
            grass_free_points.push_back(point);

         if (el == mapelem::GRAVE || (el >= mapelem::GR2G_1 && el <= mapelem::GR2G_34))
            grave_free_points.push_back(point);
      }

   for (int k = 0; k < trees / 2; k++)
   {
      std::vector<intpair>::iterator it = grass_free_points.begin() + 
         rand() % grass_free_points.size();

      int i = it->first, j = it->second;
      MapObject mo;   

      int tmp = rand() % 3;
      
      if (tmp == 0)
         mo.el = mapelem::TREE3;

      if (tmp == 1 || tmp == 2)
         mo.el = mapelem::TREE4;

      int i1, j1;

      switch (mo.el)
      {
      
      case mapelem::TREE3:
         i1 = rand() % 3, j1 = 1 + rand() % 4;
         mo.x = (i + 1) * Config::texW;
         mo.y = (j + 1) * Config::texH;
         _gm.setWalkable(i * Config::countX, j * Config::countY + 4, false);
         _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 4, false);
         _gm.setWalkable(i * Config::countX + 2, j * Config::countY + 4, false);
         _gm.setWalkable(i * Config::countX + 3, j * Config::countY + 4, false);
         mo.xoffset = 0;
         mo.yoffset = 2 * Config::regH;
            
         mo.el_sel = mapelem::TREE3_SEL;
         break;
      case mapelem::TREE4:
         i1 = rand() % 2, j1 = rand() % 5;
         mo.x = (i + 1) * Config::texW; 
         mo.y = (j + 1) * Config::texH; 
         _gm.setWalkable(i * Config::countX, j * Config::countY + 3, false);
         _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 3, false);
         _gm.setWalkable(i * Config::countX + 2, j * Config::countY + 3, false);
         _gm.setWalkable(i * Config::countX + 3, j * Config::countY + 3, false);
         mo.xoffset = 0;
         mo.yoffset = -2 * Config::regH;
         mo.el_sel = mapelem::TREE4_SEL;
         break;
     /* case mapelem::TREE5:
         i1 = rand() % 2, j1 = 1 + rand() % 4;
         mo.x = i * Config::texW + i1 * Config::regW;
         mo.y = j * Config::texH + j1 * Config::regH;
         _gm.setWalkable(i * Config::countX + i1, j * Config::countY + j1, false);
         _gm.setWalkable(i * Config::countX + i1 + 1, j * Config::countY + j1, false);
         _gm.setWalkable(i * Config::countX + i1, j * Config::countY + j1 - 1, false);
         _gm.setWalkable(i * Config::countX + i1 + 1, j * Config::countY + j1 - 1, false);
         mo.xoffset = Config::texW - 2.0 * Config::regW;
         mo.yoffset = Config::texH - 14 * Config::regH;
         mo.el_sel = mapelem::TREE5_SEL;
         break;*/
       default:
         break;
      }
      
      _gm.addObject(mo);     

      grass_free_points.erase(it);
   }

   for (int k = 0; k < trees / 2; k++)
   {
      std::vector<intpair>::iterator it = sand_free_points.begin() + 
         rand() % sand_free_points.size();

      int i = it->first, j = it->second;
      int tmp = rand() % 2;
      int i1, j1;

      MapObject mo;

      if (tmp == 0)
         mo.el = mapelem::TREE;

      if (tmp == 1)
         mo.el = mapelem::TREE2;

      switch (mo.el)
      {
         case mapelem::TREE:
            i1 = rand() % 3, j1 = 1 + rand() % 4;
            mo.x = (i + 1) * Config::texW; 
            mo.y = (j + 1) * Config::texH; 
            _gm.setWalkable(i * Config::countX, j * Config::countY + 3, false);
            _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 3, false);
            _gm.setWalkable(i * Config::countX + 2, j * Config::countY + 3, false);
            _gm.setWalkable(i * Config::countX + 3, j * Config::countY + 3, false);
            mo.xoffset = 0;
            mo.yoffset = -2 * Config::regH;
            break;
         case mapelem::TREE2:
            i1 = rand() % 3, j1 = 2 + rand() % 3;
            mo.x = (i + 1) * Config::texW; 
            mo.y = (j + 1) * Config::texH; 
            _gm.setWalkable(i * Config::countX, j * Config::countY + 4, false);
            _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 4, false);
            _gm.setWalkable(i * Config::countX + 2, j * Config::countY + 4, false);
            _gm.setWalkable(i * Config::countX + 3, j * Config::countY + 4, false);
            mo.xoffset = 0;
            mo.yoffset = -2 * Config::regH;
            mo.el_sel = mapelem::TREE2_SEL;
            break;
      }

      _gm.addObject(mo);     

      sand_free_points.erase(it);
   }


   for (int k = 0; k < rocks * 0.5; k++)
   {
      std::vector<intpair>::iterator it = grass_free_points.begin() + 
         rand() % grass_free_points.size();

      int i = it->first, j = it->second;
      mapelem::MapElem el = _gm.getElem(it->first, it->second);

      MapObject mo;   

      int tmp = rand() % 2, j1;

      mo.el = static_cast<mapelem::MapElem>(mapelem::STONE1 + tmp);

      switch (mo.el)
      {
      case mapelem::STONE1:
         j1 = 1 + rand() % 4;
         mo.x = (i + 1) * Config::texW;
         mo.y = (j + 1) * Config::texH;
         _gm.setWalkable(i * Config::countX, j * Config::countY + 4, false);
         _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 4, false);
         _gm.setWalkable(i * Config::countX + 2, j * Config::countY + 4, false);
         _gm.setWalkable(i * Config::countX + 3, j * Config::countY + 4, false);
         mo.xoffset = 0;
         mo.yoffset = 2 * Config::regH;
         break;
      default:
         mo.x = (i + 1) * Config::texW;
         mo.y = (j + 1) * Config::texH;
         _gm.setWalkable(i * Config::countX, j * Config::countY + 4, false);
         _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 4, false);
         _gm.setWalkable(i * Config::countX + 2, j * Config::countY + 4, false);
         _gm.setWalkable(i * Config::countX + 3, j * Config::countY + 4, false);
         mo.xoffset = 0;
         mo.yoffset = 2 * Config::regH;
         break;
      }

      _gm.addObject(mo);     

      grass_free_points.erase(it);
   }

   for (int k = 0; k < rocks * 0.25; k++)
   {
      std::vector<intpair>::iterator it = grave_free_points.begin() + 
         rand() % grave_free_points.size();

      int i = it->first, j = it->second, j1;
      MapObject mo;

      mo.el = static_cast<mapelem::MapElem>(mapelem::STONE1 + rand() % 2);

      switch (mo.el)
      {
      case mapelem::STONE2:
         j1 = 1 + rand() % 4;
         mo.x = (i + 1) * Config::texW;
         mo.y = (j + 1) * Config::texH;
         _gm.setWalkable(i * Config::countX, j * Config::countY + 4, false);
         _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 4, false);
         _gm.setWalkable(i * Config::countX + 2, j * Config::countY + 4, false);
         _gm.setWalkable(i * Config::countX + 3, j * Config::countY + 4, false);
         mo.xoffset = 0;
         mo.yoffset = 2 * Config::regH;
         break;
      default:
         break;
      }

      _gm.addObject(mo);
      grave_free_points.erase(it);
   }

  /* for (int k = 0; k < rocks * 0.25; k++)
   {
      std::vector<intpair>::iterator it = sand_free_points.begin() + 
         rand() % sand_free_points.size();

      int i = it->first, j = it->second, i1, j1;
      MapObject mo;

      mo.el = static_cast<mapelem::MapElem>(mapelem::STONE1 + rand() % 2);

      switch (mo.el)
      {
      case mapelem::STONE1:
         j1 = 1 + rand() % 4;
         mo.x = i * Config::texW;
         mo.y = j * Config::texH + j1 * Config::regH;
         mo.xoffset = Config::texW;
         mo.yoffset = Config::texH - 3 * Config::regH;
         _gm.setWalkable(i * Config::countX, j * Config::countY + j1, false);
         _gm.setWalkable(i * Config::countX + 1, j * Config::countY + j1, false);
         _gm.setWalkable(i * Config::countX + 2, j * Config::countY + j1, false);
         _gm.setWalkable(i * Config::countX + 2, j * Config::countY + j1 - 1, false);
         break;
      default:
         mo.x = i * Config::texW;
         mo.y = j * Config::texH;
         mo.xoffset = Config::texW;
         mo.yoffset = Config::texH;
         break;
      }

      _gm.addObject(mo);
      sand_free_points.erase(it);
   }*/

   for (int k = 0; k < flowers; k++)
   {
      std::vector<intpair>::iterator it = grass_free_points.begin() + 
         rand() % grass_free_points.size();

      MapObject mo;

      int i = it->first, j = it->second;

      mo.el = static_cast<mapelem::MapElem>(mapelem::FLOWERS1 + rand() % 2);

      switch (mo.el)
      {
      case mapelem::FLOWERS1:
         mo.x = (i + 1) * Config::texW;
         mo.y = (j + 1) * Config::texH;
         _gm.setWalkable(i * Config::countX, j * Config::countY + 4, false);
         _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 4, false);
         _gm.setWalkable(i * Config::countX + 2, j * Config::countY + 4, false);
         _gm.setWalkable(i * Config::countX + 3, j * Config::countY + 4, false);
         mo.xoffset = 0;
         mo.yoffset = -2 * Config::regH;
         break;

      case mapelem::FLOWERS2:
         mo.x = (i + 1) * Config::texW;
         mo.y = (j + 1) * Config::texH;
         _gm.setWalkable(i * Config::countX + 0, j * Config::countY + 3, false);
         _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 3,  false);
         _gm.setWalkable(i * Config::countX + 2, j * Config::countY + 3, false);
         _gm.setWalkable(i * Config::countX + 3, j * Config::countY + 3,  false);
         mo.xoffset = 0;
         mo.yoffset = 2 * Config::regH;
         break;
      }

      _gm.addObject(mo);
      grass_free_points.erase(it);
   }

   for (int k = 0; k < bushes; k++)
   {
      std::vector<intpair>::iterator it = grass_free_points.begin() + 
         rand() % grass_free_points.size();

      MapObject mo;

      int i = it->first, j = it->second, j1, i1;

      mo.el = static_cast<mapelem::MapElem>(mapelem::BUSH1 + rand() % 2);
      mo.x = i * Config::texW;
      mo.y = j * Config::texH;
      mo.xoffset = 0;
      mo.yoffset = 0;

      switch (mo.el)
      {
      case mapelem::BUSH1:
         i1 = rand() % 3;
         j1 = 1 + rand() % 4;
         mo.x = (i + 1) * Config::texW;
         mo.y = (j + 1) * Config::texH;
         _gm.setWalkable(i * Config::countX, j * Config::countY + 4, false);
         _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 4, false);
         _gm.setWalkable(i * Config::countX + 2, j * Config::countY + 4, false);
         _gm.setWalkable(i * Config::countX + 3, j * Config::countY + 4, false);
         mo.xoffset = 0;
         mo.yoffset = 2 * Config::regH;
         break;

      case mapelem::BUSH2:
         mo.x = (i + 1) * Config::texW;
         mo.y = (j + 1) * Config::texH;
         _gm.setWalkable(i * Config::countX, j * Config::countY + 3, false);
         _gm.setWalkable(i * Config::countX + 1, j * Config::countY + 3, false);
         _gm.setWalkable(i * Config::countX + 2, j * Config::countY + 3, false);
         _gm.setWalkable(i * Config::countX + 3, j * Config::countY + 3, false);
         mo.xoffset = 0;
         mo.yoffset = -2 * Config::regH;
         
         break;

      
      default:

         break;
      }

      _gm.addObject(mo);
      grass_free_points.erase(it);
   }

    int lightfields = 0;
 
    int x_border = sizes.first * Config::countX,
        y_border = sizes.second * Config::countY;
 
    for (int k = 0; k < lightfields; k++)
    {
       int i = 10 + rand() % (sizes.first - 10),
           j = 10 + rand() % (sizes.second - 10);
 
       MapObject mo;
 
       mo.x = i * Config::texW;
       mo.y = j * Config::texH;
       mo.xoffset = Config::texW;
       mo.yoffset = Config::texH;
       mo.el = rand() % 2 ? mapelem::LIGHTFIELD1 : mapelem::LIGHTFIELD2;
 
       _gm.addObject(mo);
    }
}

void GameMapGenerator::_generateAreas( GameMapSettings &gms, intpair &a, intpair &b )
{
   int count = gms.lakes + gms.graves + gms.forests,
       regnum = (int)ceil(sqrt(count * 1.0)),
       step = (b.first - a.first) / regnum;

   std::vector<std::pair<intpair, intpair> > rects;

   for (int i = 0; i < regnum; i++)
   {
      for (int j = 0; j < regnum; j++)
      {
         intpair tmp1, tmp2;

         tmp1.first = a.first + i * step;
         tmp1.second = a.second + j * step;

         tmp2.first = a.first + (i + 1) * step;
         tmp2.second = a.second + (j + 1) * step;

         std::pair<intpair, intpair> tmp(tmp1, tmp2);

         rects.push_back(tmp);

         //_gm.setElem(tmp1.first, tmp1.second, mapelem::GRAVE);
         //_gm.setElem(tmp2.first, tmp2.second, mapelem::GRAVE);
      }
   }

   //TODO: timecost erase here? diff containter usage

   for (int j = 0; j < gms.lakes; j++)
   {
      int ind = rand() % rects.size(), size = gms.lakeMaxArea;
      intpair point;
      std::vector<std::pair<intpair, intpair> >::iterator rect = rects.begin() + ind;

      intpair a1 = rect->first, b1 = rect->second;

      b1.first -= size;
      b1.second -= size;

      _generatePoint(point, a1, b1);

      _generateRandomArea(mapelem::WATER, size, point, 1, 1, false, false);
      rects.erase(rect);
   }

   for (int j = 0; j < gms.graves; j++)
   {
      int ind = rand() % rects.size(), size = 2 + rand() % (gms.graveMaxArea - 1);
      intpair point;
      std::vector<std::pair<intpair, intpair> >::iterator rect = rects.begin() + ind;

      intpair a1 = rect->first, b1 = rect->second;

      b1.first -= size;
      b1.second -= size;

      _generatePoint(point, a1, b1);

      _generateRandomArea(mapelem::GRAVE, size, point, 1, 1, false, false);
      rects.erase(rect);
   }

   /*for (int j = 0; j < gms.drygrassareas; j++)
   {
      int ind = rand() % rects.size(), size = 2 + rand() % (gms.dryGrassMaxArea - 1);
      intpair point;
      std::vector<std::pair<intpair, intpair> >::iterator rect = rects.begin() + ind;

      intpair a1 = rect->first, b1 = rect->second;

      b1.first -= size;
      b1.second -= size;

      _generatePoint(point, a1, b1);

      _generateRandomArea(mapelem::DRYGRASS, size, point, 1, 1, false, false);
      rects.erase(rect);
   }*/
}

void GameMapGenerator::_drawLine( intpair &a, intpair &b, mapelem::MapElem el )
{
   int x1 = b.first, x0 = a.first, y1 = b.second, y0 = a.second;

   int dx = std::abs(x1-x0), sx = x0<x1 ? 1 : -1;
   int dy = std::abs(y1-y0), sy = y0<y1 ? 1 : -1; 
   int err = (dx>dy ? dx : -dy)/2, e2;

   for(;;){
      _gm.setElem(x0,y0, el);
      if (x0==x1 && y0==y1) break;
      e2 = err;
      if (e2 >-dx) { err -= dy; x0 += sx; }
      if (e2 < dy) { err += dx; y0 += sy; }
   }
}

/* a------
 * |     |
 * |_____b
 */
void GameMapGenerator::_generatePoint( intpair &res, intpair &a, intpair &b )
{
   int dx = b.first - a.first - 1,
       dy = b.second - a.second - 1;

   if (dx <= 0 && dy <= 0)
   {
      res = a;
      return;
   }

   if (dx <= 0)
   {
      int rnd = rand() % 2;
      
      res.first = (rnd == 1) ? a.first : b.first;
      res.second = a.second + 1 + rand() % dy;
      return;
   }

   if (dy <= 0)
   {
      int rnd = rand() % 2;

      res.second = (rnd == 1) ? a.second : b.second;
      res.first = a.first + 1 + rand() % dx;
      return;
   }

   res.first = a.first + 1 + rand() % dx;
   res.second = a.second + 1 + rand() % dy;
}

bool compareX( intpair a, intpair b )
{
   return a.first > b.first;
}

bool compareXinv( intpair a, intpair b )
{
   return a.first < b.first;
}

bool compareY( intpair a, intpair b )
{
   return a.second > b.second;
}

bool compareYinv( intpair a, intpair b )
{
   return a.second < b.second;
}

void GameMapGenerator::_generatePoints( std::vector<intpair> &res, int pointsnum, 
                                              intpair &a, intpair &b, 
                                              bool (*comparator)( intpair, intpair ),
                                              intpair &additional )
{
   std::vector<intpair> tmp;

   for (int i = 0; i < pointsnum; i++)
   {
      intpair point;

      _generatePoint(point, a, b);
      tmp.push_back(point);
   }

   if (additional.first != -1)
      tmp.push_back(additional);

   std::stable_sort(tmp.begin(), tmp.end(), comparator);

//    for (std::vector<intpair>::iterator i = tmp.begin(); i != tmp.end(); i++)
//    {
//       fprintf(_dbgfile, "(%i, %i) ", i->first, i->second);
//    }
// 
//    fprintf(_dbgfile, "\n");

   res.insert(res.end(), tmp.begin(), tmp.end());
}

void GameMapGenerator::_generatePointsCareful( std::vector<intpair> &res, int pointsnum, 
                                       intpair &a, intpair &b, 
                                       bool (*comparator)( intpair, intpair ),
                                       intpair &additional )
{
   
   int dy = b.second - a.second,
       dx = b.first - a.first;

   int step = std::max(dy, dx) / pointsnum;

   std::vector<std::pair<intpair, intpair> > rects;

   if (dx > dy)
   {
      for (int i = 0; i < pointsnum; i++)
      {
         intpair a1, b1;

         a1.first = a.first + i * step, 
         a1.second = a.second, 
         b1.first = a.first + (i + 1) * step, 
         b1.second = b.second;

         std::pair<intpair, intpair> tmp(a1, b1);

         rects.push_back(tmp);
      }

      std::pair<intpair, intpair> endp = *(rects.end() - 1);

      if (endp.second != b)
      {
         intpair tmp1(b.first - step, b.second);
         std::pair<intpair, intpair> tmp;

         tmp.second = b;
         tmp.first = tmp1;

         rects.push_back(tmp);
      }
   }
   else
   {
      for (int i = 0; i < pointsnum; i++)
      {
         intpair a1, b1;

         a1.first = a.first;
         a1.second = a.second + i * step;

         b1.first = b.first;
         b1.second = a.second + (i + 1) * step;

         std::pair<intpair, intpair> tmp(a1, b1);

         rects.push_back(tmp);
      }

      std::pair<intpair, intpair> endp = *(rects.end() - 1);

      if (endp.second != b)
      {
         intpair tmp1(a.first, b.second - step);
         std::pair<intpair, intpair> tmp;

         tmp.first = tmp1;
         tmp.second = b;

         rects.push_back(tmp);
      }
   }

   std::vector<intpair> tmp;

   for (std::vector<std::pair<intpair, intpair> >::iterator i = rects.begin(); i != rects.end(); i++)
   {
      intpair point;

      _generatePoint(point, i->first, i->second);
      tmp.push_back(point);
   }

   if (additional.first != -1)
      tmp.push_back(additional);

   std::stable_sort(tmp.begin(), tmp.end(), comparator);

//    for (std::vector<intpair>::iterator i = tmp.begin(); i != tmp.end(); i++)
//    {
//       fprintf(_dbgfile, "(%i, %i) ", i->first, i->second);
//    }
// 
//    fprintf(_dbgfile, "\n");

   res.insert(res.end(), tmp.begin(), tmp.end());
}

void GameMapGenerator::_fillGeneratedArea( intpair &begin, mapelem::MapElem el )
{
   std::stack<intpair> points;

   points.push(begin);

   while (!points.empty())
   {
      intpair cur = points.top();
      points.pop();

      _gm.setElem(cur.first, cur.second, el);

      intpair tmp(cur.first + 1, cur.second);

//       if (tmp.first < 0 || tmp.second < 0)
//          return;

      if (_gm.getElem(tmp.first, tmp.second) != el)
         points.push(tmp);

      tmp.first = cur.first, tmp.second = cur.second + 1;

//       if (tmp.first < 0 || tmp.second < 0)
//          return;

      if (_gm.getElem(tmp.first, tmp.second) != el)
         points.push(tmp);

      tmp.first = cur.first - 1, tmp.second = cur.second;

//       if (tmp.first < 0 || tmp.second < 0)
//          return;

      if (_gm.getElem(tmp.first, tmp.second) != el)
         points.push(tmp);

      tmp.first = cur.first, tmp.second = cur.second - 1;

//       if (tmp.first < 0 || tmp.second < 0)
//          return;

      if (_gm.getElem(tmp.first, tmp.second) != el)
         points.push(tmp);
   }
}

void GameMapGenerator::_generateRandomArea( mapelem::MapElem el, int area, intpair &start,
                                            int offset, int pointsnum, bool careful, bool useadd )
{
   intpair a, b, additional(-1, -1), a1, b1;
   std::vector<intpair> tmp, points;

   // top
   a.first = start.first;
   a.second = start.second - offset;
   b.first = start.first + area;
   b.second = start.second;

   if (useadd)
   {
      a1.first = a.first - offset;
      a1.second = a.second;
      b1.first = a.first;
      b1.second = b.second;

      _generatePoint(additional, a1, b1);
   }

   if (!careful)
      _generatePoints(points, pointsnum, a, b, compareXinv, additional);
   else
      _generatePointsCareful(points, pointsnum, a, b, compareXinv, additional);

   //right
   a.first = start.first + area;
   a.second = start.second;
   b.first = start.first + area + offset;
   b.second = start.second + area;

   if (useadd)
   {
      a1.first = a.first;
      a1.second = a.second - offset;
      b1.first = a.first + offset;
      b1.second = a.second;

      _generatePoint(additional, a1, b1);
   }

   if (!careful)
      _generatePoints(points, pointsnum, a, b, compareYinv, additional);
   else
      _generatePointsCareful(points, pointsnum, a, b, compareYinv, additional);

   //bottom
   a.first = start.first;
   a.second = start.second + area;
   b.first = start.first + area;
   b.second = start.second + area + offset;

   if (useadd)
   {
      a1.first = b.first;
      a1.second = b.second - offset;
      b1.first = b.first + offset;
      b1.second = b.second;

      _generatePoint(additional, a1, b1);
   }
   
   if (!careful)
      _generatePoints(points, pointsnum, a, b, compareX, additional);
   else
      _generatePointsCareful(points, pointsnum, a, b, compareX, additional);

   //left
   a.first = start.first - offset;
   a.second = start.second;
   b.first = start.first;
   b.second = start.second + area;

   if (useadd)
   {
      a1.first = a.first;
      a1.second = b.second;
      b1.first = b.first;
      b1.second = b.second + offset;

      _generatePoint(additional, a1, b1);
   }

   if (!careful)
      _generatePoints(points, pointsnum, a, b, compareY, additional);
   else
      _generatePointsCareful(points, pointsnum, a, b, compareY, additional);

//    intpair t1(73,62), t2(74,64), t3(73,65), t4(71,63);
// 
//    points.clear();
// 
//    points.push_back(t1);
//    points.push_back(t2);
//    points.push_back(t3);
//    points.push_back(t4);

   for (std::vector<intpair>::iterator i = points.begin(); i != points.end() - 1; i++)
   {
      intpair a = *i, b = *(i + 1);
      _drawLine(*i, b, el);
      _gm.setElem(i->first, i->second, el);
   }
   
   _drawLine(*(points.end() - 1), *points.begin(), el);

   intpair begin;

   begin.first = start.first + area / 2;
   begin.second = start.second + area / 2;

  // begin.first = 72, begin.second = 62;

   while (true)
   {
      if (_gm.getElem(begin.first + 1, begin.second) != el)
      {
         begin.first++;
         break;
      }

//       if (_gm.getElem(begin.first - 1, begin.second) != el)
//       {
//          begin.first--;
//          break;
//       }

      if (_gm.getElem(begin.first, begin.second + 1) != el)
      {
         begin.second++;
         break;
      }

//       if (_gm.getElem(begin.first, begin.second - 1) != el)
//       {
//          begin.second--;
//          break;
//       }

      break;
   }

    if (_gm.getElem(begin.first, begin.second) != el)
       _fillGeneratedArea(begin, el);
}

void GameMapGenerator::_generateArea( mapelem::MapElem el, int area, 
                                      int landsize, int waterdepth )
{
   int x = rand() % landsize + waterdepth, 
       y = rand() % landsize + waterdepth;

   for (int i = x; i < x + area; i++)
      for (int j = y; j < y + area; j++)
         _gm.setElem(i, j, el);
}

bool GameMapGenerator::generate( bool prepare, bool aprepare )
{
   GameMapSettings gms;

   gms.setDefault();

   return generate(gms, prepare, aprepare);
}

GameMapGenerator::~GameMapGenerator()
{
}