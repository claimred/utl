#include "SDL.h"

#include "timer.h"

using namespace utl_core;

Timer::Timer()
{
   _startTicks = 0;
   _pausedTicks = 0;
   _paused = false;
   _started = false;    
}

void Timer::start()
{
   _started = true;
   _paused = false;
   _startTicks = SDL_GetTicks();    
}

void Timer::stop()
{
   _started = false;
   _paused = false;    
}

int Timer::getTicks()
{
   if (_started)
   {
      if (_paused)
         return _pausedTicks;
      else
         return SDL_GetTicks() - _startTicks;
   }

   return 0;    
}

void Timer::unpause()
{
   if (_paused)
   {
      _paused = false;
      _startTicks = SDL_GetTicks() - _pausedTicks;
      _pausedTicks = 0;
   }
}

bool Timer::isStarted()
{
   return _started;    
}

bool Timer::isPaused()
{
   return _paused;    
}