#include "RPSEngine.h"
#include "logger/easylogging++.h"
#include "config/RPSConfig.h"

#include "sdl/SDL_image.h"

RPSEngine::~RPSEngine()
{
    Close();
}

bool RPSEngine::Initialize()
{
    bool bRes = false;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        LOG(ERROR) << "[CRITICAL] Can't initialize SDL: " <<  SDL_GetError();
        bRes = false;
    }
    else
    {
        m_bSDLInitialized = true;

        // Default window params
        std::string strTitle("RPSGame");
        int width = 640;
        int height = 480;
        // Let's update them from config file
        if (!RPSConfig::get().GetString("WinTitle", strTitle))
            LOG(WARNING) << "Default window title will be used";
        if (!RPSConfig::get().GetInt("WinWidth", width))
            LOG(WARNING) << "Default window width will be used";
        if (!RPSConfig::get().GetInt("WinHeight", height))
            LOG(WARNING) << "Default window height will be used";
        
        m_pWindow = SDL_CreateWindow(strTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height, SDL_WINDOW_SHOWN);
        if (m_pWindow)
        {
            // Create renderer for window
            m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);
            if (!m_pRenderer)
            {
                LOG(ERROR) << "[CRITICAL] Can't create SDL renderer: " <<  SDL_GetError();
                bRes = false;
            }
            else
            {
                // Initialize renderer color
                SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                
                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    LOG(ERROR) << "[CRITICAL] SDL_image could not initialize: " << IMG_GetError();
                    bRes = false;
                }
                else
                {
                    bRes = true;
                }
            }
        }
        else
        {
            LOG(ERROR) << "[CRITICAL] Can't create SDL window: " <<  SDL_GetError();
            bRes = false;
        }
    }

    return bRes;
}

void RPSEngine::Close()
{
    if (m_pWindow)
    {
        SDL_DestroyWindow(m_pWindow);
        m_pWindow = nullptr;
    }

    if (m_bSDLInitialized)
    {
        SDL_Quit();
        m_bSDLInitialized = false;
    }
}

void RPSEngine::GameLoop()
{
    bool bQuit = false;
    SDL_Event sdlEvent;

    while (!bQuit)
    {
        while(SDL_PollEvent(&sdlEvent) != 0)
        {
            if (sdlEvent.type == SDL_QUIT)
            {
                bQuit = true;
            }
        }
    }
}
