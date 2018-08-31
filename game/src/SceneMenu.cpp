#include "SceneMenu.h"
#include "LTexture.h"
#include "LButton.h"

#include "logger/easylogging++.h"

SceneMenu::SceneMenu(SDL_Renderer* pRenderer, IGameState* cb)
    : m_pRenderer(pRenderer)
    , m_pGameStateCb(cb)
    , m_strFontName("assets/sea-font.ttf")
{
}

SceneMenu::~SceneMenu()
{

}

bool SceneMenu::Initialize()
{
    SDL_RenderGetViewport(m_pRenderer, &m_DrawingRect);

    m_pTextureBg = std::make_unique<LTexture>(m_pRenderer);
    m_pTextureBg->LoadFromFile("assets/blue-burst-abstract-bg.png");

    SDL_Color textColor = { 255, 255, 255 };
    m_vButtons.push_back(std::make_unique<LButton>(m_pRenderer, "Single Player", m_strFontName, m_iFontSize, textColor));
    m_vButtons.push_back(std::make_unique<LButton>(m_pRenderer, "Create Lobby", m_strFontName, m_iFontSize, textColor));
    m_vButtons.push_back(std::make_unique<LButton>(m_pRenderer, "Join Lobby", m_strFontName, m_iFontSize, textColor));
    m_vButtons.push_back(std::make_unique<LButton>(m_pRenderer, "Exit", m_strFontName, m_iFontSize, textColor));

    for (auto& i : m_vButtons)
    {
        i->Initialize();
    }

    // Set correct coordinates for buttons
    // "Single Player"
    m_vButtons[0]->SetPos((m_DrawingRect.w - m_DrawingRect.x) / 2 - m_vButtons[0]->GetWidth() / 2, 150);
    // "Create Lobby"
    m_vButtons[1]->SetPos((m_DrawingRect.w - m_DrawingRect.x) / 2 - m_vButtons[1]->GetWidth() / 2, 200);
    // "Join Lobby"
    m_vButtons[2]->SetPos((m_DrawingRect.w - m_DrawingRect.x) / 2 - m_vButtons[2]->GetWidth() / 2, 250);
    // "Exit"
    m_vButtons[3]->SetPos((m_DrawingRect.w - m_DrawingRect.x) / 2 - m_vButtons[3]->GetWidth() / 2, 300);

    return true;
}

void SceneMenu::Render()
{
    m_pTextureBg->Render(0, 0);

    for (auto& i : m_vButtons)
    {
        i->Render();
    }
}

bool SceneMenu::HandleEvent(SDL_Event* e)
{
    for (std::size_t i = 0; i < m_vButtons.size(); i++)
    {
        m_vButtons[i]->HandleEvent(e);

        if (m_vButtons[i]->IsClicked())
        {
            m_vButtons[i]->ClickedReset();

            switch (i)
            {
            case eButton_SinglePlayer: 
                break;
            case eButton_CreateLobby:
                break;
            case eButton_JoinLobby:
                break;
            case eButton_Exit:
                m_pGameStateCb->OnQuitApp();
                break;
            };
        }
    }

    return true;
}