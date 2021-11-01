// MarioBaseProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include "constants.h"
#include "Texture2D.h"
#include "Commons.h"
#include "GameScreenManager.h"
#include "SoundManager.h"
using namespace std;

// Globals
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
Uint32 g_old_time;
Mix_Music* g_music = nullptr;
GameScreenManager* g_manager = nullptr;
SoundManager* g_sound_manager = nullptr;

bool InitSDL();
void CloseSDL();
bool Update();
void Render();
void LoadMusic(string path);

int main(int argc, char* args[])
{
    if (InitSDL())
    {
        // Setup Manager
        g_manager = new GameScreenManager(g_renderer, SCREEN_MENU);
        g_sound_manager = new SoundManager();

        // set Time
        g_old_time = SDL_GetTicks();
        // Flag to check if we wish to quit
        bool quit = false;

        // Game Loop
        while (!quit)
        {
            Render();
            quit = Update();
        }
    }
    CloseSDL();
    
    return 0;
}


bool Update()
{
    Uint32 new_time = SDL_GetTicks();
    // Event handler
    SDL_Event e;

    // Get Events
    SDL_PollEvent(&e);

    // Handle Events
    switch (e.type)
    {
    case SDL_QUIT:
        // "X" Clicked
        return true;
        break;
    case SDL_KEYUP:
        switch (e.key.keysym.sym) 
        {
        case SDLK_q:
            // if Q is pressed, close
            break;
        }
    }

    g_manager->Update((float)(new_time - g_old_time) / 1000.0f, e);
    g_old_time = new_time;
    return false;
}


bool InitSDL()
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "SDL did not initialise. Error: " << SDL_GetError();
        return false;
    }
    else
    {
        TTF_Init();

        // Setup Passed, create window
        g_window = SDL_CreateWindow("Mario Project",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);
        // Did the window get created?
        if (g_window == nullptr)
        {
            // Window failed
            cout << "Window was not created. Error: " << SDL_GetError();
            return false;
        }

        // Renderer
        g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_renderer != nullptr)
        {
            // init PNG loading
            int imageFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imageFlags)& imageFlags))
            {
                cout << "SDL_Image could not initialise. Error: " << IMG_GetError();
                return false;
            }
        }
        else
        {
            cout << "Renderer could not be initialised. Error: " << SDL_GetError();
        }

        //initialise the mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            cout << "Mixer could not init. Error: " << Mix_GetError();
            return false;
        }

    }
    return true;
}

void Render()
{
    // Clear the screen
    SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(g_renderer);
    
    g_manager->Render();

    // Update the screen
    SDL_RenderPresent(g_renderer);
}


void LoadMusic(string path)
{
    g_music = Mix_LoadMUS(path.c_str());
    if (g_music == nullptr)
    {
        cout << "Failed to load music. Error: " << Mix_GetError() << endl;
    }
}

void CloseSDL()
{
    // release window
    SDL_DestroyWindow(g_window);
    g_window = nullptr;

    // release renderer
    SDL_DestroyRenderer(g_renderer);
    g_renderer = nullptr;

    // Destroy scene manager
    delete g_manager;
    g_manager = nullptr;

    //clear up music
    Mix_FreeMusic(g_music);
    g_music = nullptr;

    // quit subsystems
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
