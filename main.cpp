// Library includes
#include <SDL2/SDL.h>
#include "headers/LTimer.hpp"


// Includes
#include <vector>
#include <algorithm>
#include <deque>
#include <random>
#include <cstdio>


// Globals
const int SCREEN_WIDTH{ 800 };
const int SCREEN_HEIGHT{ 600 };

const int SCREEN_FPS = 8;
const int SCREEN_TICK_PER_FRAME{ 1000 / SCREEN_FPS};

// Keeps Snake in Window
void screenBound(SDL_Rect *head);

int main()
{
   // Random number generation
   std::random_device rd;
   std::uniform_int_distribution<int> d(1, SCREEN_WIDTH);

   // Initializes SDL2 Video Sub-System
   if(SDL_Init((SDL_INIT_VIDEO) < 0)) {
      std::printf("SDL could not initialize %s\n",
                  SDL_GetError());
      return 1;
   }

   // Make window and renderer variables and create them
   SDL_Window *window = nullptr;
   SDL_Renderer *renderer = nullptr;
   SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC, &window, &renderer);

   // Give the window a title
   SDL_SetWindowTitle(window, "Snake Game");
   

   // Scoped-Enum for movement
   enum class Direction{
      UP,
      DOWN,
      LEFT,
      RIGHT
   };

   SDL_Event event;
   bool isRunning = true;

   // By default direction is UP
   Direction direction = Direction::UP;

   // Dynamic Array for Apples
   std::vector<SDL_Rect> apples;

   // Generates Apple Location based on random number generator
   for(int i = 0; i < 100; i++)
   {
      SDL_Rect apple {d(rd), d(rd), 10, 10};
      
      apples.push_back(apple);
   }

   // Creates Snake Head
   SDL_Rect head {400, 300, 10, 10};
   // Body of Snake
   std::deque<SDL_Rect> body;
   // Size of Snake
   std::size_t size = 1;

   // LTimer objects responsible for handling and capping FPS
   LTimer fpsTimer;
   LTimer capTimer;

   int countedFrames{ 0 };
   fpsTimer.start();
   
   while(isRunning)
   {
      capTimer.start();
      
      while(SDL_PollEvent(&event))
      {
         switch(event.type)
         {
            case SDL_QUIT: {
               isRunning = false;
            } break;

            case SDL_KEYDOWN: {
               if(event.key.keysym.sym == SDLK_UP)
               {
                  direction = Direction::UP;
               }
               
               if(event.key.keysym.sym == SDLK_DOWN)
               {
                  direction = Direction::DOWN;
               }
               
               if(event.key.keysym.sym == SDLK_LEFT)
               {
                  direction = Direction::LEFT;
               }
               
               if(event.key.keysym.sym == SDLK_RIGHT)
               {
                  direction = Direction::RIGHT;
               }

               if(event.key.keysym.sym == SDLK_ESCAPE)
               {
                  isRunning = false;
               }
            }break;

            default: {
            }break;
         }
      }/** END EVENTS **/

      // Calculate and correct FPS
      float avgFPS = countedFrames /
         (fpsTimer.getTicks() / 1000.f);
      if(avgFPS > 20000000){
         avgFPS = 0;
      }


      /** Movement **/
      switch(direction)
      {
         case Direction::UP: {
            head.y -= 10;
         }break;
            
         case Direction::DOWN: {
            head.y += 10;
         }break;

         case Direction::LEFT: {
            head.x -= 10;
         }break;

         case Direction::RIGHT: {
            head.x += 10;
         }break;
            
         default: {
         }break;
      }

      // Clear Window
      SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
      SDL_RenderClear(renderer);


      // Draw
      for(std::size_t i{ 0 }; i < apples.size(); i++)
      {
         SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
         SDL_RenderFillRect(renderer, &apples.at(i));
      }

      /** Old apple Collision
      for(std::size_t i{ 0 }; i < apples.size(); i++)
      {
         if(SDL_HasIntersection(&apples.at(i), &head))
         {
            ateApple(&head,body);
         }
      }
      **/

      // Apple Collision detect with Snake
      std::for_each(apples.begin(), apples.end(), [&](auto& apple){
         if(SDL_HasIntersection(&apple, &head))
         {
            size += 1;
            apple.x = -10;
            apple.y = -10;
         }
      });

      screenBound(&head);

      // Snake head collision with body
      std::for_each(body.begin(), body.end(), [&](auto& snake_bod){
         if(SDL_HasIntersection(&snake_bod, &head))
         {
            size = 1;
         }
      });

      // Add new head to Snake
      body.push_front(head);

      // If the snake body is bigger than the size, pop the back of the deque body
      while(body.size() > size)
      {
         body.pop_back();
      }

      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

      /** Old Body Drawing
      for(std::size_t i{ 0 }; i < body.size(); i++)
      {
         SDL_RenderFillRect(renderer, &body.at(i));
      }
      SDL_RenderFillRect(renderer, &head);
      **/

      // Handles coloring each part of body
      std::for_each(body.begin(), body.end(), [&](auto& snake_bod){
         SDL_RenderFillRect(renderer, &snake_bod);
      });

      // Display
      SDL_RenderPresent(renderer);
      ++countedFrames;

      // If frame finished early
      int frameTicks = capTimer.getTicks();
      if(frameTicks < SCREEN_TICK_PER_FRAME){
         SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
      }
   }

   // Frees memory
   SDL_DestroyWindow(window);
   SDL_DestroyRenderer(renderer);
   // For safety measures
   window = nullptr;
   renderer = nullptr;

   // Exit SDL
   SDL_Quit();


   // Notification that user has indeed exit the application
   std::cout << "\n========APPLICATION CLOSED========\n" << std::endl;
   
   return 0;
}

// Keeps the Snake on screen
void screenBound(SDL_Rect *head)
{
   if(head->x > SCREEN_WIDTH)
   {
      head->x = 0;
   } else if (head->x < 0){
      head->x = SCREEN_WIDTH;
   }

   if(head->y > SCREEN_HEIGHT)
   {
      head->y = 0;
   } else if (head->y < 0)
   {
      head->y = SCREEN_HEIGHT;
   }
}
