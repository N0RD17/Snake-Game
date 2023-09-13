#include "headers/LTimer.hpp"


LTimer::LTimer()
{
   // Initialize the variables
   m_StartTicks = 0;
   m_PausedTicks = 0;

   m_Paused = false;
   m_Started = false;
}


void LTimer::start()
{
   // Start the timer
   m_Started = true;

   // Unpause timer
   m_Paused = false;

   // Get current clock time
   m_StartTicks = SDL_GetTicks();
   m_PausedTicks = 0;
}

void LTimer::stop()
{
   // Start the timer
   m_Started = false;

   // Unpause timer
   m_Paused = true;

   // Get current clock time
   m_StartTicks = 0;
   m_PausedTicks = 0;
}

void LTimer::pause()
{
   // If timer running and not paused
   if(m_Started && !m_Paused) {
      // Pause the timer
      m_Paused = true;

      // Calculate the paused ticks
      m_PausedTicks = SDL_GetTicks() - m_StartTicks;
      m_StartTicks = 0;
      
   }
}

void LTimer::unpause()
{
   // If timer running and not paused
   if(m_Started && m_Paused) {
      // Pause the timer
      m_Paused = false;

      // Calculate the paused ticks
      m_StartTicks = SDL_GetTicks() - m_PausedTicks;

      // Reset Paused ticks
      m_PausedTicks = 0;
      
   }
}

Uint32 LTimer::getTicks()
{
   // The actual timer's time
   Uint32 time = 0;

   // If timer is running
   if(m_Started)
   {
      // If timer is paused
      if(m_Paused)
      {
         // Return number of ticks when timer was paused
         time = m_PausedTicks;
      } else {
         // Return current time minus start time
         time = SDL_GetTicks() - m_StartTicks;
      }
   }

   return time;
}

bool LTimer::isStarted()
{
   // Timer is running and paused or unpaused
   return m_Started;
}

bool LTimer::isPaused()
{
   // Timer is running and paused
   return m_Paused && m_Started;
}
