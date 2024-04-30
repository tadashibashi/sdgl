#include "SdlGuard.h"
#include <SDL2/SDL.h>

#include <mutex>
#include <iostream>

static std::mutex s_initMutex;
static int s_initCount;

static constexpr auto InitFlags = SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER;

sdgl::SdlGuard::SdlGuard() : m_wasInit(false)
{
    s_initMutex.lock();
    if (s_initCount > 0 && SDL_WasInit(InitFlags) > 0)
    {
        ++s_initCount;
        m_wasInit = true;
    }
    else
    {
        if (SDL_Init(InitFlags) != 0)
        {
            std::cerr << "Failed to initialize SDL2: " << SDL_GetError() << '\n';
        }
        else
        {
            m_wasInit = true;
            s_initCount = 1;
        }
    }
    s_initMutex.unlock();
}

sdgl::SdlGuard::~SdlGuard()
{
    s_initMutex.lock();
    if (m_wasInit)
    {
        --s_initCount;
        if (s_initCount <= 0)
        {
            SDL_Quit();
            s_initCount = 0;
        }
    }
    s_initMutex.unlock();
}

bool sdgl::SdlGuard::wasInit()
{
    return s_initCount > 0;
}
