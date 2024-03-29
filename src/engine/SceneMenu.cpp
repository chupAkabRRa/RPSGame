#include "SceneMenu.h"
#include "LTexture.h"
#include "LButton.h"

#include "logger/easylogging++.h"

SceneMenu::SceneMenu(SDL_Renderer* pRenderer, IGameState* cb)
    : m_pRenderer(pRenderer)
    , m_pGameStateCb(cb)
    , m_strFontName("assets/action-man-bold.ttf")
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
    m_vButtons[eButton_SinglePlayer] = std::make_unique<LButton>(m_pRenderer, "Single Player", m_strFontName, m_iFontSize, textColor);
    m_vButtons[eButton_CreateLobby] = std::make_unique<LButton>(m_pRenderer, "Create Lobby", m_strFontName, m_iFontSize, textColor);
    m_vButtons[eButton_JoinLobby] = std::make_unique<LButton>(m_pRenderer, "Join Lobby", m_strFontName, m_iFontSize, textColor);
    m_vButtons[eButton_Exit] = std::make_unique<LButton>(m_pRenderer, "Exit", m_strFontName, m_iFontSize, textColor);
    m_vButtons[eButton_Logged] = std::make_unique<LButton>(m_pRenderer, "Logged as: ", m_strFontName, m_iFontSize / 2, textColor, false);

    for (auto& i : m_vButtons)
    {
        i.second->Initialize();
    }

    // Set correct coordinates for buttons
    // "Single Player"
    m_vButtons[eButton_SinglePlayer]->SetPos((m_DrawingRect.w - m_DrawingRect.x) / 2 - m_vButtons[eButton_SinglePlayer]->GetWidth() / 2, 150);
    // "Create Lobby"
    m_vButtons[eButton_CreateLobby]->SetPos((m_DrawingRect.w - m_DrawingRect.x) / 2 - m_vButtons[eButton_CreateLobby]->GetWidth() / 2, 200);
    // "Join Lobby"
    m_vButtons[eButton_JoinLobby]->SetPos((m_DrawingRect.w - m_DrawingRect.x) / 2 - m_vButtons[eButton_JoinLobby]->GetWidth() / 2, 250);
    // "Exit"
    m_vButtons[eButton_Exit]->SetPos((m_DrawingRect.w - m_DrawingRect.x) / 2 - m_vButtons[eButton_Exit]->GetWidth() / 2, 300);
    // "Logged as: "
    m_vButtons[eButton_Logged]->SetPos(10, (m_DrawingRect.h - m_DrawingRect.y) - 20);

    return true;
}

void SceneMenu::Render()
{
    m_pTextureBg->Render(0, 0);

    m_vButtons[eButton_Logged]->UpdateCaption("Logged as: " + m_pGameStateCb->GetGOGUserName());

    for (auto& i : m_vButtons)
    {
        i.second->Render();
    }
}

bool SceneMenu::HandleEvent(SDL_Event* e)
{
    for (auto& i : m_vButtons)
    {
        i.second->HandleEvent(e);

        if (i.second->IsClicked())
        {
            i.second->ClickedReset();

            switch (i.first)
            {
            case eButton_SinglePlayer:
                m_pGameStateCb->OnStateChange(IGameState::eState::eState_GameStarted);
                break;
            case eButton_CreateLobby:
                m_pGameStateCb->OnStateChange(IGameState::eState::eState_LobbyCreating);
                break;
            case eButton_JoinLobby:
                m_pGameStateCb->OnStateChange(IGameState::eState::eState_LobbySearching);
                break;
            case eButton_Exit:
                m_pGameStateCb->OnStateChange(IGameState::eState::eState_GameQuit);
                break;
            };
        }
    }

    return true;
}