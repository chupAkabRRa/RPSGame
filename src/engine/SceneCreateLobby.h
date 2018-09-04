#ifndef _SCENE_CREATE_LOBBY_H_
#define _SCENE_CREATE_LOBBY_H_

#include "IScene.h"
#include "IGameState.h"

#include <memory>
#include <map>
#include <string>

#include "sdl/SDL.h"

class LTexture;
class LButton;

class SceneCreateLobby : public IScene
{
public:
    SceneCreateLobby(SDL_Renderer* pRenderer, IGameState* cb);
    ~SceneCreateLobby();

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
        eStatus_Creating = 0,
        eStatus_WaitingForPlayer = 1
    };

    const int m_iFontSize = 32;
    const std::string m_strFontName;

    SDL_Renderer* m_pRenderer;
    SDL_Rect m_DrawingRect;
    IGameState* m_pGameStateCb;

    std::unique_ptr<LTexture> m_pTextureBg;
    std::map<eButtons, std::unique_ptr<LButton>> m_vButtons;

    eStatus m_currStatus;
};

#endif // _SCENE_CREATE_LOBBY_H_