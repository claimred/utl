#include <iterator>

#include "camera.h"
#include "config.h"

using namespace utl_core;

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::_updateObjIndices()
{
   int j1 = _j0 * Config::texH,
       j2 = (_j0 + screenElemH  + 5) * Config::texH;

   for (MapObjIterator it = _objsRef->begin(); it != _objsRef->end(); it++)
   {
      if ((*it)->y > j1)
      {
         _objBegin = it;
         break;
      }
   }

   typedef std::vector<MapObject *>::reverse_iterator MapObjRevIterator;

   for (MapObjRevIterator it = _objsRef->rbegin(); it != _objsRef->rend(); it++)
   {
      int y = (*it)->y;

      if ((*it)->y < j2)
      {
         
         
         int dist = -std::distance(it, _objsRef->rbegin()); 

         it.base();

         if (_objsRef->size() > 1)
         {
            _objEnd = it.base();
            //_objEnd = _objsRef->begin() + dist + 1;
         }
         else
            _objEnd = _objsRef->begin();

         break;
      }
   }
}

void Camera::setObjsRef( std::vector<MapObject *> *objsref )
{
   _objsRef = objsref;
}

void Camera::setBorderX( int border )
{
   _borderX = border;
}

void Camera::setBorderY( int border )
{
   _borderY = border;
}

void Camera::setY( int y )
{
   if (y < 0)
      y = 0;

   if (y > _borderY)
      y = _borderY;

   _y = y;

   _j0 = _y / Config::texH - 1;
   _yoffset = _y % Config::texH;

   _updateObjIndices();
}

void Camera::setX( int x )
{
   if (x < 0)
      x = 0;

   if (x > _borderX)
      x = _borderX;

   _x = x;

   _i0 = _x / Config::texW - 1;
   _xoffset = _x % Config::texW;

   //_updateObjIndices();
}

void Camera::setStepX( int step )
{
   _stepX = step;
}

void Camera::setStepY( int step )
{
   _stepY = step;
}

void Camera::moveX( int dir )
{
   int newx = _x + dir * _stepY;

   if (newx < 0)
      setX(0);

   if (newx > _borderX)
      setX(_borderX - 1);

   setX(newx);
}

void Camera::moveY( int dir )
{   
   int newy = _y + dir * _stepY;

   if (newy < 0)
      setY(0);

   if (newy > _borderY)
      setY(_borderY - 1);

   setY(newy);
}

int Camera::getX()
{
   return _x;
}

int Camera::getY()
{
   return _y;
}

int Camera::geti0()
{
   return _i0;
}

int Camera::getj0()
{
   return _j0;
}

int Camera::getxOffset()
{
   return _xoffset;
}

int Camera::getyOffset()
{
   return _yoffset;
}

Camera::MapObjIterator Camera::getObjBegin()
{
   return _objBegin;
}

Camera::MapObjIterator Camera::getObjEnd()
{
   return _objEnd;
}