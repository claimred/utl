#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <vector>

#include "game_map.h"

namespace utl_core
{
   class Camera
   {
   public:

      typedef std::vector<MapObject *>::iterator MapObjIterator;

      Camera();

      void setObjsRef( std::vector<MapObject *> *objsref );
      
      void setBorderX( int border );
      void setBorderY( int border );
      void setStepX( int step );
      void setStepY( int step );
      void setX( int x );
      void setY( int y );

      int geti0();
      int getj0();
      int getxOffset();
      int getyOffset();
      MapObjIterator getObjBegin();
      MapObjIterator getObjEnd();

      void moveX( int dir );
      void moveY( int dir );

      int getX();
      int getY();

      ~Camera();

      //TODO: get rid of that
      int screenElemH;

   private:

      //TODO: mix camera with map object might be not a good idea
      void _updateObjIndices();

      int _stepX, _stepY, _borderX, _borderY, _x, _y,
          _i0, _j0, _xoffset, _yoffset, _counterX, _counterY;
          
        

      std::vector<MapObject *> *_objsRef;
      MapObjIterator _objBegin, _objEnd;
   };
}

#endif /* _CAMERA_H_ */