#include "SceneCreateLobby.h"
#include "LTexture.h"
#include "LButton.h"

#include "logger/easylogging++.h"

SceneCreateLobby::SceneCreateLobby(SDL_Renderer* pRenderer, IGameState* cb)
    : m_pRenderer(pRenderer)
    , m_pGameStateCb(cb)
    , m_strFontName("assets/action-man-bold.ttf")
{
}

SceneCreateLobby::~SceneCreateLobby()
{

}

bool SceneCreateLobby::Initialize()
{
    SDL_RenderGetViewport(m_pRenderer, &m_DrawingRect);

    m_pTextureBg = std::make_unique<LTexture>(m_pRenderer);
    m_pTextureBg->LoadFromFile("assets/blue-burst-abstract-bg.png");

    SDL_Color textColor = { 255, 255, 255 };
    m_vButtons.push_back(std::make_unique<LButton>(m_pRenderer, "Waiting for 2-nd player...", m_strFontName, m_iFontSize, textColor, false));
    m_vButtons.push_back(std::make_unique<LButton>(m_pRenderer, "<-", m_strFontName, m_iFontSize, textColor));

    for (auto& i : m_vButtons)
    {
        i->Initialize();
    }

    // Set correct coordinates for buttons
    // "Waiting..."
    m_vButtons[eButton_Waiting]->SetPos((m_DrawingRect.w - m_DrawingRect.x) / 2 - m_vButtons[0]->GetWidth() / 2, (m_DrawingRect.h - m_DrawingRect.y) / 2);
    // "Back to main menu"
    m_vButtons[eButton_BackToMenu]->SetPos(10, 10);

    return true;
}

void SceneCreateLobby::Render()
{
    m_pTextureBg->Render(0, 0);

    for (auto& i : m_vButtons)
    {
        i->Render();
    }
}

bool SceneCreateLobby::HandleEvent(SDL_Event* e)
{
    for (std::size_t i = 0; i < m_vButtons.size(); i++)
    {
        m_vButtons[i]->HandleEvent(e);

        if (m_vButtons[i]->IsClicked())
        {
            m_vButtons[i]->ClickedReset();

            switch (i)
            {
            case eButton_BackToMenu:
                m_pGameStateCb->OnStateChange(IGameState::eState::eState_Menu);
                break;
            };
        }
    }

    return true;
}