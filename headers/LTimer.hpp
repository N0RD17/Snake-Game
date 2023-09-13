#pragma once

#include <SDL2/SDL.h>


#include <iostream>


class LTimer{
public:
   // Initializes variables
   LTimer();
   virtual ~LTimer() = default;

   // The various clock actions
   void start();
   void stop();
   void pause();
   void unpause();

   // Gets the timer's time
   Uint32 getTicks();

   // Checks the status of the timer
   bool isStarted();
   bool isPaused();
   
private:
   // The clock time when timer started
   Uint32 m_StartTicks { 0 };
   
   Uint32 m_PausedTicks { 0 };

   // The timer status
   bool m_Paused { true };
   bool m_Started { false };
};
