#include <string>

#include "RPSEngine.h"
#include "SceneMenu.h"
#include "SceneGame.h"

#include "logger/easylogging++.h"
#include "config/RPSConfig.h"

#include "sdl/SDL_image.h"
#include "sdl/SDL_ttf.h"

RPSEngine::~RPSEngine()
{
    Close();
}

bool RPSEngine::Initialize(IGameState* cb)
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

        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			LOG(WARNING) << "Linear texture filtering not enabled";
		} 

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
            m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (m_pRenderer)
            {
                // Initialize renderer color
                SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                
                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if ((IMG_Init(imgFlags) & imgFlags))
                {
                    m_bSDLImgInitialized = true;

                    // Initialize SDL_ttf
                    if (TTF_Init() > -1)
                    {
                        m_bTTFInitialized = true;
                        bRes = true;

                        m_pGameStateCb = cb;

                        // Load scenes
                        m_vScenes.push_back(std::make_unique<SceneMenu>(m_pRenderer, cb));
                        m_vScenes.push_back(std::make_unique<SceneGame>(m_pRenderer, cb));

                        for (auto& i : m_vScenes)
                        {
                            i->Initialize();
                        }

                        m_iActiveScene = eScene::eScene_Menu;
                    }
                    else
                    {
                        LOG(ERROR) << "[CRITICAL] TTF could not initialize: " << TTF_GetError();
                        bRes = false;
                    }
                }
                else
                {
                    LOG(ERROR) << "[CRITICAL] SDL_image could not initialize: " << IMG_GetError();
                    bRes = false;
                }
            }
            else
            {
                LOG(ERROR) << "[CRITICAL] Can't create SDL renderer: " <<  SDL_GetError();
                bRes = false;
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
    if (m_pRenderer)
    {
        SDL_DestroyRenderer(m_pRenderer);
        m_pRenderer = nullptr;
    }

    if (m_pWindow)
    {
        SDL_DestroyWindow(m_pWindow);
        m_pWindow = nullptr;
    }

    if (m_bTTFInitialized)
    {
        TTF_Quit();
        m_bTTFInitialized = false;
    }

    if (m_bSDLImgInitialized)
    {
        IMG_Quit();
        m_bSDLImgInitialized = false;
    }

    if (m_bSDLInitialized)
    {
        SDL_Quit();
        m_bSDLInitialized = false;
    }
}

void RPSEngine::SetActiveScene(eScene scene)
{
    m_iActiveScene = scene;
}

void RPSEngine::Animate()
{
    bool bQuit = false;
    SDL_Event sdlEvent;

    while(SDL_PollEvent(&sdlEvent) != 0)
    {
        if (sdlEvent.type == SDL_QUIT)
        {
            m_pGameStateCb->OnStateChange(IGameState::eState::eState_GameQuit);
        }

        m_vScenes[m_iActiveScene]->HandleEvent(&sdlEvent);
    }

    // Clear screen
    SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(m_pRenderer);

    // Render scene
    m_vScenes[m_iActiveScene]->Render();

    // Update screen
    SDL_RenderPresent(m_pRenderer);
}
