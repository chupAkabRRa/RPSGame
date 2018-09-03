#ifndef _SCENE_MENU_H_
#define _SCENE_MENU_H_

#include "IScene.h"
#include "IGameState.h"

#include <memory>
#include <vector>
#include <string>

#include "sdl/SDL.h"

class LTexture;
class LButton;

class SceneMenu : public IScene
{
public:
    SceneMenu(SDL_Renderer* pRenderer, IGameState* cb);
    ~SceneMenu();

    bool Initialize() override;
    void Render() override;
    bool HandleEvent(SDL_Event* e) override;

private:
    enum eButtons
    {
        eButton_SinglePlayer = 0,
        eButton_CreateLobby = 1,
        eButton_JoinLobby = 2,
        eButton_Exit = 3
    };
    const int m_iFontSize = 32;
    const std::string m_strFontName;

    SDL_Renderer* m_pRenderer;
    SDL_Rect m_DrawingRect;
    IGameState* m_pGameStateCb;

    std::unique_ptr<LTexture> m_pTextureBg;
    std::vector<std::unique_ptr<LButton>> m_vButtons;
};

#endif // _SCENE_MENU_H_