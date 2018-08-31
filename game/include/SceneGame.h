#ifndef _SCENE_GAME_H_
#define _SCENE_GAMEU_H_

#include "IScene.h"
#include "IGameState.h"

#include <memory>
#include <vector>
#include <string>

#include "sdl/SDL.h"

class LTexture;
class LButton;

class SceneGame : public IScene
{
public:
    SceneGame(SDL_Renderer* pRenderer, IGameState* cb);
    ~SceneGame();

    bool Initialize() override;
    void Render() override;
    bool HandleEvent(SDL_Event* e) override;

private:
    enum eButtons
    {
        eButton_Rock = 0,
        eButton_Paper = 1,
        eButton_Scissors = 2,
        eButton_BackToMenu = 3,
        eButton_You = 4,
        eButton_Enemy = 5
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