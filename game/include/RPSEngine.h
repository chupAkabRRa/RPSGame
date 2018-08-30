#ifndef _RPS_ENGINE_H_
#define _RPS_ENGINE_H_

#include <memory>

#include "IRenderObject.h"
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

    std::unique_ptr<IRenderObject> m_pScene;
};

#endif // _RPS_ENGINE_H_