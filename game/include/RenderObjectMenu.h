#ifndef _RENDER_OBJECT_MENU_H_
#define _RENDER_OBJECT_MENU_H_

#include "IRenderObject.h"

#include <memory>
#include <string>

#include "sdl/SDL.h"

class LTexture;

class RenderObjectMenu : public IRenderObject
{
public:
    RenderObjectMenu(SDL_Renderer* pRenderer);
    ~RenderObjectMenu();

    bool Initialize() override;
    bool Render() override;

private:
    const int m_iFontSize = 32;
    const std::string m_strFontName;

    SDL_Renderer* m_pRenderer;
    SDL_Rect m_DrawingRect;

    std::unique_ptr<LTexture> m_pTextureBg;
    std::unique_ptr<LTexture> m_pMenuItemSingle;
    std::unique_ptr<LTexture> m_pMenuItemCreateLobby;
    std::unique_ptr<LTexture> m_pMenuItemJoinLobby;
    std::unique_ptr<LTexture> m_pMenuItemExit;
};

#endif // _RENDER_OBJECT_MENU_H_