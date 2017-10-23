#include <map>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <stdio.h>

#include "config.h"
#include "path_builder.h"

using namespace utl_core;

PathBuilder::PathBuilder( GameMap &gm ) : _gm(gm)
{
   _bulk = new Node[_bulkSize];
   _sqrt2 = sqrt(2.0);
   _d = 1.0;
   _d2 = _sqrt2;
//    _d = 1.0;
//    _d2 = _sqrt2;
}

bool utl_core::operator<( const PathBuilder::PathVertex &a, const PathBuilder::PathVertex &b )
{
    if (a.f_score > b.f_score)
        return true;
    
    return false;
    
}

bool utl_core::operator<( const PathBuilder::NodePtr &a, const PathBuilder::NodePtr &b )
{
   if (a.node->f_score > b.node->f_score)
      return true;

   return false;

}

int PathBuilder::getPathSize()
{
   return _path.size();
}

void PathBuilder::clear()
{
   _path.clear();
   _borders.clear();
   _states.clear();
}

int PathBuilder::_heuristic( PathVertex &node, PathVertex &goal )
{
   int dx = std::abs(node.point.first - goal.point.first),
       dy = std::abs(node.point.second - goal.point.second);

   if (dx > dy)
      return _d2 * dy + _d * (dx - dy);

   return _d2 * dx + _d * (dy - dx);
}

double PathBuilder::_heuristic( Node &a, Node &b )
{
   int dx = std::abs(a.x - b.x),
       dy = std::abs(a.y - b.y);

   if (dx > dy)
      return _d2 * dy + _d * (dx - dy);

   return _d2 * dx + _d * (dy - dx);

}

bool PathBuilder::_isWalkable( int i, int j )
{
   return true;
}

void PathBuilder::_getWalkable( intpair &a, intpair &b )
{
   std::vector<intpair> line;

   Utils::buildLine(b, a, line);

   for (std::vector<intpair>::iterator it = line.begin(); it != line.end(); it++)
   {
      if (_gm.isMicroWalkable(it->first, it->second))
      {
         b = *it;
         break;
      }
   }
}

void PathBuilder::setBorders( int x0, int y0, int x1, int y1 )
{
   _x0 = x0;
   _y0 = y0;

   _x1 = x1;
   _y1 = y1;
}

bool PathBuilder::_checkBorders( int i, int j )
{
   if (i < _x0 || i > _x1)
      return false;

   if (j < _y0 || j > _y1)
      return false;

   return true;
}

void PathBuilder::_transformPoint( intpair &a )
{
   int tmpx = a.first / Config::countX * Config::texW +
              a.first % Config::countX * Config::regW,
       tmpy = a.second / Config::countY * Config::texH +
              a.second % Config::countY * Config::regH;

   a.first = tmpx + Config::regW / 2, a.second = tmpy + Config::regH / 2;
}

void PathBuilder::buildPath( intpair &a, intpair &b, intpair *char_pos )
{
   _charPos = char_pos;
   _openset.clear();
   _closedset.clear();
   _allocCount = 0;

   for (int i = 0; i < _bulkSize; i++)
   {
      _bulk[i].setZero();
   }
//      
//    if (_bulk.empty())
//      _bulk.resize(1000);

   _buildPathJPS(a, b);
   //_buildPath(a, b, _isMacroWalkable);
   //_buildPath(a, b, _isMicroWalkable);
}

bool PathBuilder::_isJumpOver( int x, int y, int dx, int dy )
{
   if (!_gm.isMicroWalkable(x, y)) 
      return true;

   if (x == _goal.x && y == _goal.y) 
      return true;

   if (dx != 0 && dy != 0) 
   {
      if ((_gm.isMicroWalkable(x - dx, y + dy) && !_gm.isMicroWalkable(x - dx, y)) ||
         (_gm.isMicroWalkable(x + dx, y - dy) && !_gm.isMicroWalkable(x, y - dy))) 
      {
         return true;
      }
   }
   else 
   {
      if (dx != 0) 
      { 
         if ((_gm.isMicroWalkable(x + dx, y + 1) && !_gm.isMicroWalkable(x, y + 1)) ||
            (_gm.isMicroWalkable(x + dx, y - 1) && !_gm.isMicroWalkable(x, y - 1))) 
         {
            return true;
         }
      }
      else 
      {
         if ((_gm.isMicroWalkable(x + 1, y + dy) && !_gm.isMicroWalkable(x + 1, y)) ||
            (_gm.isMicroWalkable(x - 1, y + dy) && !_gm.isMicroWalkable(x - 1, y))) 
         {
            return true;
         }
      }
   }

   return false;
}

PathBuilder::Node *PathBuilder::_jumpVertical( int x, int y, int px, int py )
{
   int j = y, dy = y - py;
   Node *res;

//    if (!_gm.isMicroWalkable(x, y))
//       return 0;

   while (true)
   {
      if (_isJumpOver(x, j, 0, dy))
         break;

      j += dy;
   }

   if (!_gm.isMicroWalkable(x, j))
      return 0;
   else
   {
      res = getNode();
      res->x = x, res->y = j;
      return res;
   }
}

PathBuilder::Node *PathBuilder::_jumpHorizontal( int x, int y, int px, int py )
{
   int i = x, dx = x - px;
   Node *res;

//    if (!_gm.isMicroWalkable(x, y))
//       return 0;

   while (true)
   {
      if (_isJumpOver(i, y, dx, 0))
         break;

      i += dx;
   }

   if (!_gm.isMicroWalkable(i, y))
      return 0;
   else
   {
      res = getNode();
      res->x = i, res->y = y;
      return res;
   }
}

PathBuilder::Node *PathBuilder::_jump( int x, int y, int px, int py )
{
   int dx = x - px, dy = y - py;

   if (dy == 0)
      return _jumpHorizontal(x, y, px, py);

   if (dx == 0)
      return _jumpVertical(x, y, px, py);

   int i = x, j = y;

   if (dx != 0 && dy != 0)
   {
      while (true)
      {
         if (_isJumpOver(i, j, dx, dy))
         {
            if (!_gm.isMicroWalkable(i, j))
               return 0;

            Node *res = getNode();

            res->x = i, res->y = j;
            return res;
            
         }
         
//          if (!_gm.isMicroWalkable(i, j))
//             return 0;         

         //TODO: odd allocations, no need to do them
         Node *jx = _jumpHorizontal(i + dx, j, i, j);
         Node *jy = _jumpVertical(i, j + dy, i, j);

         if (jy != 0 || jx != 0)
         {
            Node *res = getNode();

            res->x = i, res->y = j;
            return res;

         }


         if (_gm.isMicroWalkable(i + dx, j) || _gm.isMicroWalkable(i, j + dy)) 
         {
            i += dx, j += dy;
            continue;
         }
         else
            return 0;
      }
   }

   return 0;
}

PathBuilder::Node *PathBuilder::getNode()
{
   //printf("%i\n", _allocCount);

   if (_allocCount >= _bulkSize)
      printf("Memory Overhead\n");
   
   return &_bulk[_allocCount++];
}

void PathBuilder::_findNeighbours( PathBuilder::Node *node )
{
   int px, py, x = node->x, y = node->y, dx, dy;
   Node *parent = node->parent;

   if (parent != 0) 
   {
      px = parent->x;
      py = parent->y;
      
      dx = (x - px) / std::max(abs(x - px), 1);
      dy = (y - py) / std::max(abs(y - py), 1);

      // Diagonal direction
      if (dx != 0 && dy != 0) 
      {
         // Natural neighbors
         if (_gm.isMicroWalkable(x, y + dy)) 
         {
            Node *neighbour = getNode();

            neighbour->parent = node;
            neighbour->x = x, neighbour->y = y + dy;
            node->neighbours.push_back(neighbour);
         }
         
         if (_gm.isMicroWalkable(x + dx, y)) 
         {
            Node *neighbour = getNode();

            neighbour->parent = node;
            neighbour->x = x + dx, neighbour->y = y;
            node->neighbours.push_back(neighbour);
         }
         
         if (_gm.isMicroWalkable(x, y + dy) || _gm.isMicroWalkable(x + dx, y)) 
         {
            Node *neighbour = getNode();

            neighbour->parent = node;
            neighbour->x = x + dx, neighbour->y = y + dy;
            node->neighbours.push_back(neighbour);
         }

         // Forced neighbors
         if (!_gm.isMicroWalkable(x - dx, y) && _gm.isMicroWalkable(x, y + dy)) 
         {
            Node *neighbour = getNode();

            neighbour->parent = node;
            neighbour->x = x - dx, neighbour->y = y + dy;
            node->neighbours.push_back(neighbour);
         }

         if (!_gm.isMicroWalkable(x, y - dy) && _gm.isMicroWalkable(x + dx, y)) 
         {
            Node *neighbour = getNode();

            neighbour->parent = node;
            neighbour->x = x + dx, neighbour->y = y - dy;
            node->neighbours.push_back(neighbour);
         }
      } 
      else 
      {
         // Vertical direction
         if (dx == 0) 
         {
            if (_gm.isMicroWalkable(x, y + dy)) 
            {
               if (_gm.isMicroWalkable(x, y + dy)) 
               {
                  Node *neighbour = getNode();

                  neighbour->parent = node;
                  neighbour->x = x, neighbour->y = y + dy;
                  node->neighbours.push_back(neighbour);
               }
               if (!_gm.isMicroWalkable(x + 1, y)) 
               {
                  Node *neighbour = getNode();

                  neighbour->parent = node;
                  neighbour->x = x + 1, neighbour->y = y + dy;
                  node->neighbours.push_back(neighbour);
               }
               if (!_gm.isMicroWalkable(x - 1, y)) 
               {
                  Node *neighbour = getNode();

                  neighbour->parent = node;
                  neighbour->x = x - 1, neighbour->y = y + dy;
                  node->neighbours.push_back(neighbour);
               }
            }
         }
         // Horizontal direction
         else 
         {
            if (_gm.isMicroWalkable(x + dx, y)) 
            {
               if (_gm.isMicroWalkable(x + dx, y)) 
               {
                  Node *neighbour = getNode();

                  neighbour->parent = node;
                  neighbour->x = x + dx, neighbour->y = y;
                  node->neighbours.push_back(neighbour);
               }
               if (!_gm.isMicroWalkable(x, y + 1)) 
               {
                  Node *neighbour = getNode();

                  neighbour->parent = node;
                  neighbour->x = x + dx, neighbour->y = y + 1;
                  node->neighbours.push_back(neighbour);
               }
               if (!_gm.isMicroWalkable(x, y - 1)) 
               {
                  Node *neighbour = getNode();

                  neighbour->parent = node;
                  neighbour->x = x + dx, neighbour->y = y - 1;
                  node->neighbours.push_back(neighbour);
               }
            }
         }
      }
   }
   else 
   {
      intpair tmp;

      int a[8][2] = { {0, -1}, {1, 0}, {0, 1}, {-1, 0}, {-1, -1}, {1, -1}, {1, 1}, {-1, 1} };

      for (int k = 0; k < 8; k++)
      {
         int i = a[k][0], j = a[k][1];

         if (_isMicroWalkable(_gm, node->x + i, node->y + j, tmp))
         {
            Node *neighbor = getNode();

            neighbor->x = node->x + i, neighbor->y = node->y + j;
            neighbor->parent = node;

            node->neighbours.push_back(neighbor);
         }
      }
   }
}

void PathBuilder::_identifySuccessors( PathBuilder::Node *node )
{
   typedef std::vector<Node *>::iterator NodeIterator;
   typedef std::vector<NodePtr>::iterator NodePtrIterator;

   int x = node->x, y = node->y;

   _findNeighbours(node);

   for (NodeIterator it = node->neighbours.begin(); it != node->neighbours.end(); it++)
   {
      Node *neighbor = *it;

      Node *jumppoint = _jump(neighbor->x, neighbor->y, x, y);

      if (jumppoint != 0)
      {
         bool _continue = false;

         for (NodePtrIterator k = _closedset.begin(); k != _closedset.end(); k++)
         {
            if (k->node->x == jumppoint->x && k->node->y == jumppoint->y)
            {
               _continue = true;
               break;
            }
         }

         if (_continue)
            continue;

         int jx = jumppoint->x, jy = jumppoint->y,
            dx = abs(jx - x), dy = abs(jy - y);

         double d = std::sqrt(1.0 * dx * dx + dy * dy);
         double ng = node->g_score + d;

         Node *tmp;

         bool isinopenset = false;

         //TODO: hash table?
         for (NodePtrIterator k = _openset.begin(); k != _openset.end(); k++)
         {
            if (k->node->x == jumppoint->x && k->node->y == jumppoint->y)
            {
               isinopenset = false;
               tmp = k->node;
               break;
            }
         }


         if (!isinopenset)
         {
            jumppoint->g_score = ng;
            jumppoint->f_score = jumppoint->g_score + _heuristic(*jumppoint, _goal);
            jumppoint->parent = node;

            _openset.push_back(NodePtr(jumppoint));
            std::push_heap(_openset.begin(), _openset.end());
         }
         else
         {
            tmp->g_score = ng;
            tmp->f_score = tmp->g_score + _heuristic(*tmp, _goal);
            tmp->parent = node;

            std::make_heap(_openset.begin(), _openset.end());
         }
      }
   }
}

void PathBuilder::_buildPathJPS( intpair &a, intpair &b )
{
   _goal.x = b.first;
   _goal.y = b.second;

   _start.neighbours.clear();
   _start.parent = 0;
   _start.x = a.first;
   _start.y = a.second;
   _start.g_score = 0;
   _start.f_score = _start.g_score + _heuristic(_start, _goal);

   if (!_gm.isMicroWalkable(_goal.x, _goal.y))
   {
      intpair tmp1(_start.x, _start.y), tmp2(_goal.x, _goal.y);

      _getWalkable(tmp1, tmp2);
      _goal.x = tmp2.first;
      _goal.y = tmp2.second;
   }

   if (_start.x == _goal.x && _start.y == _goal.y)
      return;

   _openset.push_back(&_start);
   std::push_heap(_openset.begin(), _openset.end());

   Node *res = 0;

   //printf("\n_aCb: %i\n", _allocCount);

   while (!_openset.empty())
   {
      Node *current = _openset.front().node;

      if (current->x == _goal.x && current->y == _goal.y)
      {
         res = current;
         break;
      }

      std::pop_heap(_openset.begin(), _openset.end());
      _openset.pop_back();

      _closedset.push_back(NodePtr(current));

      _identifySuccessors(current);
   }

   //printf("_aCe: %i\n", _allocCount);
   
   if (res == 0)
   {
      printf("no path found!");
   }

   std::vector<intpair> tmppath;

   while (res != 0)
   {
      tmppath.push_back(intpair(res->x, res->y));
      res = res->parent;
   }

   std::reverse(tmppath.begin(), tmppath.end());

   std::vector<intpair>::iterator it;

    _borders.push_back(0);

   int it1 = 0;

   if (_charPos != 0)
   {
      it1 = 1;
      intpair a = *(tmppath.begin() + 1), b = *_charPos;

      _transformPoint(a);

      Utils::buildLine(b, a, _path);
      _borders.push_back(_path.size() - 1);
   }

   for (it = tmppath.begin() + it1; it != tmppath.end() - 1; it++)
   {
      intpair a = *it, b = *(it + 1);

      _transformPoint(a);
      _transformPoint(b);

      Utils::buildLine(a, b, _path);

      _borders.push_back(_path.size() - 1);
   }

   for (int i = 0; i != _borders.size() - 1; i++)
      _states.push_back(_getState(_path[_borders[i]], _path[_borders[i + 1]]));
}

bool PathBuilder::_isMacroWalkable( GameMap &gm, int i, int j, const intpair &parent )
{
   return gm.isMacroWalkable(i, j);
}

bool PathBuilder::_isMicroWalkable( GameMap &gm, int i, int j, const intpair &parent )
{
   return gm.isMicroWalkable(i, j);
}

void PathBuilder::_buildPath( intpair &a, intpair &b, 
                             bool (*isWalkable)( GameMap &gm, int i, int j, const intpair &parent) )
{
   typedef std::vector<PathVertex>::iterator VertexIterator;

   intpair a1(a), b1(b);
   PathVertex goal(b1, -1, -1), start(a1, 0, 0);
   std::vector<PathVertex> openset, closedset;
   
   std::map<intpair, intpair> came_from;

   start.f_score = start.g_score + _heuristic(start, goal);

   openset.push_back(start);
   std::push_heap(openset.begin(), openset.end());

   int counter = 0;
   bool error = false;

   while (!openset.empty())
   {
      counter++;

      if (counter > 1000)
      {
         error = true;
         break;
      }

      PathVertex current = openset.front();

      if (current.point == goal.point) 
      {
         //printf("oset: %i\ncset: %i\n", openset.size(), closedset.size());
         break;
      }

      std::pop_heap(openset.begin(), openset.end());
      openset.pop_back();

      closedset.push_back(current);
      std::push_heap(closedset.begin(), closedset.end());

      intpair curp = current.point;
      bool _continue = false;

      for (int j = -1; j <= 1; j++)
         for (int i = -1; i <= 1; i++)
         {
            _continue = false;

            if (i == 0 && j == 0)
               continue;

            if (isWalkable(_gm, curp.first + i, curp.second + j, current.point))
            {
               intpair intp(curp.first + i, curp.second + j);
               PathVertex neighbour(intp, -1, -1);

               //Skip already closed neighbours
                
               for (VertexIterator k = closedset.begin(); k != closedset.end(); k++)
               {
                  if (k->point == neighbour.point)
                  {
                     _continue = true;
                     break;
                  }
               }

               if (_continue)
                  continue;
                
               int tentative_g_score = current.g_score;
                
               if (i + j == 1 || i + j == -1)
                  tentative_g_score += _d;
               else
                  tentative_g_score += _d2;
               
               bool isinopenset = false;
               VertexIterator tmp;


               for (VertexIterator k = openset.begin(); k != openset.end(); k++)
               {
                  if (k->point == neighbour.point)
                  {
                     isinopenset = true;
                     tmp = k;
                     break;
                  }
               }

               if (!isinopenset)
               {
                  neighbour.g_score = tentative_g_score;
                  neighbour.f_score = neighbour.g_score + _heuristic(neighbour, goal);
                  came_from.insert(std::pair<intpair, intpair>(neighbour.point, current.point));
                  openset.push_back(neighbour);
                  std::push_heap(openset.begin(), openset.end());
               }
               else
               {
                  if (tentative_g_score < tmp->g_score)
                  {
                     tmp->g_score = tentative_g_score;
                     tmp->f_score = tmp->g_score + _heuristic(*tmp, goal);

                     std::map<intpair, intpair>::iterator it = came_from.find(tmp->point);

                     if (it == came_from.end())
                        came_from.insert(std::pair<intpair, intpair>(tmp->point, current.point));
                     else
                        it->second = current.point;

                     std::make_heap(openset.begin(), openset.end());
                  }
               }
            }
         }
   }

   if (error)
   {
      printf("woah. no path found\n");
      return;
   }

   
   intpair tmp = goal.point;
   std::vector<intpair> tmppath;

   while (true)
   {
      if (tmp == start.point)
      {
         tmppath.push_back(tmp);
         break;
      }

      tmppath.push_back(tmp);
      tmp = came_from[tmp];
   }

   std::reverse(tmppath.begin(), tmppath.end());

   std::vector<intpair>::iterator it;

   for (it = tmppath.begin(); it != tmppath.end() - 1; it++)
   {
      intpair a = *it, b = *(it + 1);

      _transformPoint(a);
      _transformPoint(b);

      Utils::buildLine(a, b, _path);
   }

//    for (std::vector<intpair>::iterator it = _path.begin(); it != _path.end(); it++)
//    {
//       it->first = it->first * Config::texW + Config::regW;
//       it->second = it->second * Config::texH + 2 * Config::regH;
//    }

//    for (std::vector<intpair>::iterator it = _path.begin(); it != _path.end(); it++)
//       _transformPoint(*it);
}

mapelem::MapElem PathBuilder::getCurrentState( int counter )
{
   for (int i = 0; i != _borders.size() - 1; i++)
   {
      if (counter >= _borders[i] && counter <= _borders[i + 1])
      {
         return _states[i];
      }
   }

   return mapelem::GRASS;
}

mapelem::MapElem PathBuilder::_getState( intpair &a, intpair &b )
{
   int deltax = b.first - a.first, deltay = b.second - a.second;

   //down, up, right, left

   if (deltax >= 0)
      if (deltay >= 0)
      {
         if (deltay > deltax)
            return mapelem::CHARACTERM10;
         else if (deltay <= deltax)
            return mapelem::CHARACTERM30;
      }

   if (deltax >= 0)
      if (deltay < 0)
      {
         if (deltax > -deltay)
            return mapelem::CHARACTERM30;
         else if (deltax <= -deltay)
            return mapelem::CHARACTERM20;
      }

   if (deltax < 0)
      if (deltay < 0)
      {
         if (-deltax < -deltay)
            return mapelem::CHARACTERM20;
         else if (-deltax >= -deltay)
            return mapelem::CHARACTERM40;
      }

   if (deltax < 0)
      if (deltay >= 0)
      {
         if (-deltax < deltay)
            return mapelem::CHARACTERM10;
         else if (-deltax >= deltay)
            return mapelem::CHARACTERM40;
      }

   return mapelem::CHARACTERM10;
}

intpair &PathBuilder::operator[]( int index )
{
   
   return _path[index];
}

PathBuilder::~PathBuilder()
{
   delete[] _bulk;
}