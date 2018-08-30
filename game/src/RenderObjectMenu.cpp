#include "RenderObjectMenu.h"
#include "LTexture.h"

RenderObjectMenu::RenderObjectMenu(SDL_Renderer* pRenderer)
{
    m_pTextureBg = std::make_unique<LTexture>(pRenderer);
}

RenderObjectMenu::~RenderObjectMenu()
{

}

bool RenderObjectMenu::Initialize()
{
    return m_pTextureBg->LoadFromFile("assets/blue-burst-abstract-bg.png");
}

bool RenderObjectMenu::Render()
{
    m_pTextureBg->Render(0, 0);

    return true;
}