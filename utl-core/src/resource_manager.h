#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <map>
#include <vector>

#include "SDL.h"

#include "game_map.h"
#include "utils.h"
#include "texture.h"

namespace utl_core
{
  //TODO: fix an extremely messy class
  class ResourceManager
  {
  public:

    ResourceManager( int resourcesnum );
      
    bool loadMedia( SDL_Renderer *_renderer );

    Texture &operator[]( mapelem::MapElem el );

    ~ResourceManager();

  private:

    void _initIndices();

    typedef std::pair<mapelem::MapElem, int> MapElemInt;

    std::map<mapelem::MapElem, int> _resourceConfig;
    std::vector<Texture> _resources;
  };
}

#endif /* _RESOURCE_MANAGER_H_ */