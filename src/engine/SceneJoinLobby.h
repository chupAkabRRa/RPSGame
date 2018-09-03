#ifndef _SCENE_JOIN_LOBBY_H_
#define _SCENE_JOIN_LOBBY_H_

#include "IScene.h"
#include "IGameState.h"

#include <memory>
#include <vector>
#include <string>

#include "sdl/SDL.h"

class LTexture;
class LButton;

class SceneJoinLobby : public IScene
{
public:
    SceneJoinLobby(SDL_Renderer* pRenderer, IGameState* cb);
    ~SceneJoinLobby();

    bool Initialize() override;
    void Render() override;
    bool HandleEvent(SDL_Event* e) override;

private:
    enum eButtons
    {
        eButton_Status = 0,
        eButton_BackToMenu = 1
    };

    enum eStatus
    {
        eStatus_Searching = 0,
        eStatus_Connecting = 1
    };

    const int m_iFontSize = 32;
    const std::string m_strFontName;

    SDL_Renderer* m_pRenderer;
    SDL_Rect m_DrawingRect;
    IGameState* m_pGameStateCb;

    std::unique_ptr<LTexture> m_pTextureBg;
    std::vector<std::unique_ptr<LButton>> m_vButtons;

    eStatus m_currStatus;
};

#endif // _SCENE_JOIN_LOBBY_H_