#include "SceneJoinLobby.h"
#include "LTexture.h"
#include "LButton.h"

#include "logger/easylogging++.h"

SceneJoinLobby::SceneJoinLobby(SDL_Renderer* pRenderer, IGameState* cb)
    : m_pRenderer(pRenderer)
    , m_pGameStateCb(cb)
    , m_strFontName("assets/action-man-bold.ttf")
{
}

SceneJoinLobby::~SceneJoinLobby()
{

}

bool SceneJoinLobby::Initialize()
{
    SDL_RenderGetViewport(m_pRenderer, &m_DrawingRect);

    m_pTextureBg = std::make_unique<LTexture>(m_pRenderer);
    m_pTextureBg->LoadFromFile("assets/blue-burst-abstract-bg.png");

    SDL_Color textColor = { 255, 255, 255 };
    m_vButtons[eButton_Status] = std::make_unique<LButton>(m_pRenderer, "Searching requested lobby...", m_strFontName, m_iFontSize, textColor, false);
    m_vButtons[eButton_BackToMenu] = std::make_unique<LButton>(m_pRenderer, "<-", m_strFontName, m_iFontSize, textColor);

    for (auto& i : m_vButtons)
    {
        i.second->Initialize();
    }

    // Set correct coordinates for buttons
    // "<Status>"
    m_vButtons[eButton_Status]->SetPos((m_DrawingRect.w - m_DrawingRect.x) / 2 - m_vButtons[eButton_Status]->GetWidth() / 2,
                                       (m_DrawingRect.h - m_DrawingRect.y) / 2);
    // "Back to main menu"
    m_vButtons[eButton_BackToMenu]->SetPos(10, 10);

    m_currStatus = eStatus::eStatus_Searching;

    return true;
}

void SceneJoinLobby::Render()
{
    m_pTextureBg->Render(0, 0);

    if (m_currStatus == eStatus::eStatus_Searching &&
        m_pGameStateCb->GetCurrState() == IGameState::eState::eState_LobbyFound)
    {
        m_vButtons[eButton_Status]->UpdateCaption("Connecting to lobby...");
        m_vButtons[eButton_Status]->SetPos((m_DrawingRect.w - m_DrawingRect.x) / 2 - m_vButtons[eButton_Status]->GetWidth() / 2,
                                           (m_DrawingRect.h - m_DrawingRect.y) / 2);
        m_currStatus = eStatus::eStatus_Connecting;
        m_pGameStateCb->OnStateChange(IGameState::eState::eState_LobbyConnecting);
    }
    else if (m_currStatus == eStatus::eStatus_Connecting &&
            m_pGameStateCb->GetCurrState() == IGameState::eState::eState_LobbySearching)
    {
        m_vButtons[eButton_Status]->UpdateCaption("Searching requested lobby...");
        m_vButtons[eButton_Status]->SetPos((m_DrawingRect.w - m_DrawingRect.x) / 2 - m_vButtons[eButton_Status]->GetWidth() / 2,
                                           (m_DrawingRect.h - m_DrawingRect.y) / 2);
        m_currStatus = eStatus::eStatus_Searching;
    }

    for (auto& i : m_vButtons)
    {
        i.second->Render();
    }
}

bool SceneJoinLobby::HandleEvent(SDL_Event* e)
{
    for (auto& i : m_vButtons)
    {
        i.second->HandleEvent(e);

        if (i.second->IsClicked())
        {
            i.second->ClickedReset();

            switch (i.first)
            {
            case eButton_BackToMenu:
                m_pGameStateCb->OnStateChange(IGameState::eState::eState_Menu);
                break;
            };
        }
    }

    return true;
}