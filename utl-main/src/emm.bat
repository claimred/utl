emcc main.cpp ../../utl-core/src/camera.cpp ^
../../utl-core/src/game.cpp ^
../../utl-core/src/game_draw.cpp ^
../../utl-core/src/game_events.cpp ^
../../utl-core/src/game_map.cpp ^
../../utl-core/src/game_map_generator.cpp ^
../../utl-core/src/game_map_preparer.cpp ^
../../utl-core/src/game_map_settings.cpp ^
../../utl-core/src/game_map_writer.cpp ^
../../utl-core/src/gui_manager.cpp ^
../../utl-core/src/path_builder.cpp ^
../../utl-core/src/resource_manager.cpp ^
../../utl-core/src/texture.cpp ^
../../utl-core/src/timer.cpp ^
../../utl-core/src/utils.cpp ^
-I ../../utl-core/src -s USE_SDL_IMAGE=2 -s USE_SDL=2 -o utl.html -O2 --memory-init-file 0 -W --preload-file res -s ALLOW_MEMORY_GROWTH=1