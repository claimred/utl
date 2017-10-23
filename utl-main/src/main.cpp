
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include "game.h"

#ifdef UTLRELEASE
#include "physfs.h"
#endif

using namespace utl_core;

void loop_iteration( Game *g )
{
   g->loopIter();
}

int main( int argc, char *argv[] )
{

#ifdef UTLRELEASE
   PHYSFS_init(argv[0]);
   PHYSFS_mount("utl.data", NULL, 1);
#endif /* UTRLRELEASE */

   Game g;

   g.init();

#ifdef __EMSCRIPTEN__
   emscripten_set_main_loop_arg((em_arg_callback_func)loop_iteration, &g, 0, 1);
#else
   g.loop();
#endif /* __EMSCRIPTEN__ */


   g.stop();

   return 0;
}