#ifndef _RPS_ENGINE_H_
#define _RPS_ENGINE_H_

#include <memory>
#include <map>

#include "IScene.h"
#include "IGameState.h"

#include "sdl/SDL.h"

class RPSEngine
{
public:
    enum eScene
    {
        eScene_Menu = 0,
        eScene_Game = 1,
        eScene_LobbyCreate = 2,
        eScene_LobbyJoin = 3
    };

    RPSEngine() = default;
    ~RPSEngine();

    bool Initialize(IGameState* cb);
    void Close();
    void Animate();
    void SetActiveScene(eScene scene);
private:
    bool m_bSDLInitialized = false;
    bool m_bSDLImgInitialized = false;
    bool m_bTTFInitialized = false;

    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;

    IGameState* m_pGameStateCb;

    std::map<eScene, std::unique_ptr<IScene>> m_vScenes;
    eScene m_iActiveScene;
};

#endif // _RPS_ENGINE_H_