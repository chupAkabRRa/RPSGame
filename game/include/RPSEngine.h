#ifndef _RPS_ENGINE_H_
#define _RPS_ENGINE_H_

#include <memory>

#include "IScene.h"
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
    bool m_bTTFInitialized = false;

    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;

    std::unique_ptr<IScene> m_pScene;
};

#endif // _RPS_ENGINE_H_