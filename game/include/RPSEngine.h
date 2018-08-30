#ifndef _RPS_ENGINE_H_
#define _RPS_ENGINE_H_

#include "sdl/SDL.h"

class RPSEngine
{
public:
    RPSEngine() = default;
    ~RPSEngine();

    bool Initialize();
    void Close();
    void GameLoop();
private:
    bool m_bSDLInitialized = false;
    bool m_bSDLImgInitialized = false;
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;

    // Assets
    SDL_Texture* m_pTextureBg;

    SDL_Texture* LoadTexture(const std::string& strPath);
    //bool LoadResources();
};

#endif // _RPS_ENGINE_H_