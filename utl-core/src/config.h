#ifndef _CONFIG_H_
#define _CONFIG_H_

namespace utl_core
{  
   namespace Config
   {
      static const int texW = 128, texH = 128, countX = 4, countY = 4;
      static const int minimapW = 200, minimapH = 100;

      
      static const double regW = 1.0 * texW / countX, 
                          regH = 1.0 * texH / countY;
   }

};

#endif /* _CONFIG_H_ */