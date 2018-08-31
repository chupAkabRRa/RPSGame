#include "RenderObjectMenu.h"
#include "LTexture.h"

#include "logger/easylogging++.h"

RenderObjectMenu::RenderObjectMenu(SDL_Renderer* pRenderer)
    : m_pRenderer(pRenderer)
    , m_strFontName("assets/sea-font.ttf")
{
    m_pTextureBg = std::make_unique<LTexture>(pRenderer);
    m_pMenuItemSingle = std::make_unique<LTexture>(pRenderer);
    m_pMenuItemCreateLobby = std::make_unique<LTexture>(pRenderer);
    m_pMenuItemJoinLobby = std::make_unique<LTexture>(pRenderer);
    m_pMenuItemExit = std::make_unique<LTexture>(pRenderer);
}

RenderObjectMenu::~RenderObjectMenu()
{

}

bool RenderObjectMenu::Initialize()
{
    SDL_RenderGetViewport(m_pRenderer, &m_DrawingRect);
    m_pTextureBg->LoadFromFile("assets/blue-burst-abstract-bg.png");

    SDL_Color textColor = { 255, 255, 255 };
    m_pMenuItemSingle->LoadFromRenderedText(m_strFontName.c_str(), m_iFontSize, "Single Player", textColor);
    m_pMenuItemCreateLobby->LoadFromRenderedText(m_strFontName.c_str(), m_iFontSize, "Create Lobby", textColor);
    m_pMenuItemJoinLobby->LoadFromRenderedText(m_strFontName.c_str(), m_iFontSize, "Join Lobby", textColor);
    m_pMenuItemExit->LoadFromRenderedText(m_strFontName.c_str(), m_iFontSize, "Exit", textColor);

    return true;
}

bool RenderObjectMenu::Render()
{
    m_pTextureBg->Render(0, 0);

    m_pMenuItemSingle->Render((m_DrawingRect.w - m_DrawingRect.x) / 2 - m_pMenuItemSingle->GetWidth() / 2, 150);
    m_pMenuItemCreateLobby->Render((m_DrawingRect.w - m_DrawingRect.x) / 2 - m_pMenuItemSingle->GetWidth() / 2, 200);
    m_pMenuItemJoinLobby->Render((m_DrawingRect.w - m_DrawingRect.x) / 2 - m_pMenuItemSingle->GetWidth() / 2, 250);
    m_pMenuItemExit->Render((m_DrawingRect.w - m_DrawingRect.x) / 2 - m_pMenuItemSingle->GetWidth() / 2, 300);

    return true;
}