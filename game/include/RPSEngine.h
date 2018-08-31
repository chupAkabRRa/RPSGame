#ifndef _RPS_ENGINE_H_
#define _RPS_ENGINE_H_

#include <memory>
#include <vector>

#include "IScene.h"
#include "IGameState.h"

#include "sdl/SDL.h"

class RPSEngine
{
public:
    RPSEngine() = default;
    ~RPSEngine();

    bool Initialize(IGameState* cb);
    void Close();
    void GameLoop();
private:
    bool m_bSDLInitialized = false;
    bool m_bSDLImgInitialized = false;
    bool m_bTTFInitialized = false;

    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;

    IGameState* m_pGameStateCb;

    std::vector<std::unique_ptr<IScene>> m_vScenes;
};

#endif // _RPS_ENGINE_H_