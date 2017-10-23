
namespace utl_core
{
   //The timer
   class Timer
   {   
   public:
      
      Timer();

      void start();
      void stop();
      void pause();
      void unpause();

      int getTicks();

      bool isStarted();
      bool isPaused();    
   
   private:

      int _startTicks;
      int _pausedTicks;
      bool _paused, _started;

   };
}