#ifndef _SCENE_CREATE_LOBBY_H_
#define _SCENE_CREATE_LOBBY_H_

#include "IScene.h"
#include "IGameState.h"

#include <memory>
#include <vector>
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
        eButton_Waiting = 0,
        eButton_BackToMenu = 1
    };
    const int m_iFontSize = 32;
    const std::string m_strFontName;

    SDL_Renderer* m_pRenderer;
    SDL_Rect m_DrawingRect;
    IGameState* m_pGameStateCb;

    std::unique_ptr<LTexture> m_pTextureBg;
    std::vector<std::unique_ptr<LButton>> m_vButtons;
};

#endif // _SCENE_CREATE_LOBBY_H_