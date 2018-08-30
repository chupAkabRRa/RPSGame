#ifndef _RENDER_OBJECT_MENU_H_
#define _RENDER_OBJECT_MENU_H_

#include "IRenderObject.h"

#include <memory>

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
    std::unique_ptr<LTexture> m_pTextureBg;
};

#endif // _RENDER_OBJECT_MENU_H_