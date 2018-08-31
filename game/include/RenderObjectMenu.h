#ifndef _RENDER_OBJECT_MENU_H_
#define _RENDER_OBJECT_MENU_H_

#include "IScene.h"

#include <memory>
#include <vector>
#include <string>

#include "sdl/SDL.h"

class LTexture;
class LButton;

class RenderObjectMenu : public IScene
{
public:
    RenderObjectMenu(SDL_Renderer* pRenderer);
    ~RenderObjectMenu();

    bool Initialize() override;
    void Render() override;
    bool HandleEvent(SDL_Event* e) override;

private:
    const int m_iFontSize = 32;
    const std::string m_strFontName;

    SDL_Renderer* m_pRenderer;
    SDL_Rect m_DrawingRect;

    std::unique_ptr<LTexture> m_pTextureBg;
    
    std::vector<std::unique_ptr<LButton>> m_vButtons;
};

#endif // _RENDER_OBJECT_MENU_H_