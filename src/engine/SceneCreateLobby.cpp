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
    m_vButtons[eButton_Status] = std::make_unique<LButton>(m_pRenderer, "Creating lobby...", m_strFontName, m_iFontSize, textColor, false);
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

    m_currStatus = eStatus::eStatus_Creating;

    return true;
}

void SceneCreateLobby::Render()
{
    m_pTextureBg->Render(0, 0);

    if (m_currStatus == eStatus::eStatus_Creating &&
        m_pGameStateCb->GetCurrState() == IGameState::eState::eState_LobbyCreated)
    {
        m_vButtons[eButton_Status]->UpdateCaption("Waiting for 2-nd player...");
        m_vButtons[eButton_Status]->SetPos((m_DrawingRect.w - m_DrawingRect.x) / 2 - m_vButtons[eButton_Status]->GetWidth() / 2,
                                           (m_DrawingRect.h - m_DrawingRect.y) / 2);
        m_currStatus = eStatus::eStatus_WaitingForPlayer;
    }
    else if (m_currStatus == eStatus::eStatus_WaitingForPlayer &&
            m_pGameStateCb->GetCurrState() == IGameState::eState::eState_LobbyCreating)
    {
        m_vButtons[eButton_Status]->UpdateCaption("Creating lobby...");
        m_vButtons[eButton_Status]->SetPos((m_DrawingRect.w - m_DrawingRect.x) / 2 - m_vButtons[eButton_Status]->GetWidth() / 2,
                                           (m_DrawingRect.h - m_DrawingRect.y) / 2);
        m_currStatus = eStatus::eStatus_Creating;
    }

    for (auto& i : m_vButtons)
    {
        i.second->Render();
    }
}

bool SceneCreateLobby::HandleEvent(SDL_Event* e)
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