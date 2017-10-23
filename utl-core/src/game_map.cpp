#include <algorithm>
#include <string.h>
#include <stdio.h>

#include "config.h"
#include "game_map.h"

using namespace utl_core;

GameMap::GameMap() : _bulkSize(3000)
{
   _isInit = false;
   _map = 0;
   _sizeX = -1;
   _sizeY = -1;
   _bulk = new MapObject[_bulkSize];
   _allocCount = 0;
}

bool GameMap::create( int sizex, int sizey )
{
   if (_map != 0)
      return false;

   _sizeX = sizex;
   _sizeY = sizey;

   _map = new mapelem::MapElem[sizex * sizey];
   
   _isInit = true;

   return true;
}

void GameMap::setElem( int i, int j, mapelem::MapElem el )
{
   if (!_isInit)
   {
      //throw Exception here
      return;
   }

   if (i >= _sizeX || j >= _sizeY || i < 0 || j < 0)
      throw "Invalid indices";

   _map[j * _sizeX + i] = el;
}

mapelem::MapElem GameMap::getElem( int i, int j )
{
   if (i >= _sizeX || j >= _sizeY || i < 0 || j < 0)
      throw "Invalid indices";

   return _map[j * _sizeX + i];
}

void GameMap::initWalkable()
{  
   _width = Config::countX * _sizeX;
   _height = Config::countY * _sizeY;

   _walkable = new bool*[_height];

   for (int i = 0; i < _height; i++)
   {
      _walkable[i] = new bool[_width];

      for (int j = 0; j < _width; j++)
         _walkable[i][j] = true;
   }

 //  _walkable.resize(_width * _height);

  // std::fill(_walkable.begin(), _walkable.end(), true);
}

bool GameMap::isMicroWalkable( int p, int q )
{
   //TODO: it`s wrong!!1
    if (p < 0 || q < 0 || p >= _width || q >= _height)
       return false;

   //return _walkable[q * _width + p];
   return _walkable[q][p];
}

bool GameMap::isMacroWalkable( int i, int j )
{
   mapelem::MapElem el = getElem(i, j);

   if (el == mapelem::WATER)
      return false;

   if (el >= mapelem::W2G_0101 && el <= mapelem::W2G_3401)
      return false;

   return true;
}

void GameMap::setWalkable( int p, int q, bool val )
{
   _walkable[q][p] = val;
}

std::vector<MapObject *> *GameMap::getObjects()
{
   return &_objects;
}

MapObject *GameMap::addObject( const MapObject &mo )
{
   MapObject *tmp = &_bulk[_allocCount++];

   if (_allocCount > _bulkSize)
      printf("Objects: memory overhead!\n");

   //printf("objalloccount: %i\n", _allocCount);

   *tmp = mo;

   _objects.push_back(tmp);

   return tmp;
}

bool GameMap::isInit()
{
   return _isInit;
}

void GameMap::clear()
{
   if (_map != 0)
      delete[] _map;

   if (_bulk != 0)
      delete[] _bulk;

   _sizeX = -1;
   _sizeY = -1;
   _map = 0;
   _isInit = false;
}

void GameMap::fill( mapelem::MapElem el )
{
   if (!isInit())
      //TODO: Exceptions
      return;

   int totalsize = _sizeX * _sizeY;

   for (int i = 0; i < totalsize; i++)
      _map[i] = el;

   //memset(_map, el, _size * _size); // TODO: Wut?
}

void GameMap::getSize( std::pair<int, int> &sizes )
{
   sizes.first = _sizeX;
   sizes.second = _sizeY;
}

void GameMap::updateObjects()
{
   std::sort(_objects.begin(), _objects.end(), MapObject::compareMapObjects);
}

GameMap::~GameMap()
{
   clear();
}