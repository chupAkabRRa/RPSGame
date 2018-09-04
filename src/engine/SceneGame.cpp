#include "SceneGame.h"
#include "LTexture.h"
#include "LButton.h"

#include "logger/easylogging++.h"

SceneGame::SceneGame(SDL_Renderer* pRenderer, IGameState* cb)
    : m_pRenderer(pRenderer)
    , m_pGameStateCb(cb)
    , m_strFontName("assets/action-man-bold.ttf")
{
}

SceneGame::~SceneGame()
{
}

bool SceneGame::Initialize()
{
    SDL_RenderGetViewport(m_pRenderer, &m_DrawingRect);

    m_pTextureBg = std::make_unique<LTexture>(m_pRenderer);
    m_pTextureBg->LoadFromFile("assets/blue-burst-abstract-bg.png");

    m_vTexturePicks[common::ePick::None] = std::make_unique<LTexture>(m_pRenderer);
    m_vTexturePicks[common::ePick::Rock] = std::make_unique<LTexture>(m_pRenderer);
    m_vTexturePicks[common::ePick::Scissors] = std::make_unique<LTexture>(m_pRenderer);
    m_vTexturePicks[common::ePick::Paper] = std::make_unique<LTexture>(m_pRenderer);
    m_vTexturePicks[common::ePick::Rock]->LoadFromFile("assets/rock.png");
    m_vTexturePicks[common::ePick::Scissors]->LoadFromFile("assets/scissors.png");
    m_vTexturePicks[common::ePick::Paper]->LoadFromFile("assets/paper.png");

    SDL_Color textColor = { 255, 255, 255 };
    m_vButtons[eButton_Rock] = std::make_unique<LButton>(m_pRenderer, "Rock", m_strFontName, m_iFontSize, textColor);
    m_vButtons[eButton_Paper] = std::make_unique<LButton>(m_pRenderer, "Paper", m_strFontName, m_iFontSize, textColor);
    m_vButtons[eButton_Scissors] = std::make_unique<LButton>(m_pRenderer, "Scissors", m_strFontName, m_iFontSize, textColor);
    m_vButtons[eButton_BackToMenu] = std::make_unique<LButton>(m_pRenderer, "<-", m_strFontName, m_iFontSize, textColor);
    m_vButtons[eButton_You] = std::make_unique<LButton>(m_pRenderer, "You: 0", m_strFontName, m_iFontSize, textColor, false);
    m_vButtons[eButton_Enemy] = std::make_unique<LButton>(m_pRenderer, "Enemy: 0", m_strFontName, m_iFontSize, textColor, false);

    for (auto& i : m_vButtons)
    {
        i.second->Initialize();
    }

    // Set correct coordinates for buttons
    int xPaper = (m_DrawingRect.w - m_DrawingRect.x) / 2 - m_vButtons[eButton_Paper]->GetWidth() / 2 - 10;
    int xRock = xPaper - m_vButtons[eButton_Rock]->GetWidth() - 20;
    int xScissors = xPaper + m_vButtons[eButton_Paper]->GetWidth() + 20;
    // "Rock"
    m_vButtons[eButton_Rock]->SetPos(xRock, m_DrawingRect.h - m_DrawingRect.y - 35);
    // "Paper"
    m_vButtons[eButton_Paper]->SetPos(xPaper, m_DrawingRect.h - m_DrawingRect.y - 35);
    // "Scissors"
    m_vButtons[eButton_Scissors]->SetPos(xScissors, m_DrawingRect.h - m_DrawingRect.y - 35);
    // "Back to main menu"
    m_vButtons[eButton_BackToMenu]->SetPos(10, 10);
    // "You: XXX"
    m_vButtons[eButton_You]->SetPos((m_DrawingRect.w - m_DrawingRect.x) / 4 - m_vButtons[eButton_You]->GetWidth() / 2, 10);
    // "Enemy: YYY"
    m_vButtons[eButton_Enemy]->SetPos(3 * (m_DrawingRect.w - m_DrawingRect.x) / 4 - m_vButtons[eButton_You]->GetWidth() / 2, 10);

    return true;
}

void SceneGame::Render()
{
    m_pTextureBg->Render(0, 0);

    SDL_RenderDrawLine(m_pRenderer,
        (m_DrawingRect.w - m_DrawingRect.x) / 2, 0,
        (m_DrawingRect.w - m_DrawingRect.x) / 2, m_DrawingRect.h - m_DrawingRect.y - 50);
    SDL_RenderDrawLine(m_pRenderer,
        0, 50,
        m_DrawingRect.w - m_DrawingRect.x, 50);
    SDL_RenderDrawLine(m_pRenderer,
        0, m_DrawingRect.h - m_DrawingRect.y - 50,
        m_DrawingRect.w - m_DrawingRect.x, m_DrawingRect.h - m_DrawingRect.y - 50);

    for (auto& i : m_vButtons)
    {
        i.second->Render();
    }

    if (m_pGameStateCb->GetCurrState() == IGameState::eState::eState_GameRoundFinished)
    {
        SDL_Delay(1000);
        m_pGameStateCb->OnStateChange(IGameState::eState::eState_GameNewRound);

        int iPlayer, iEnemy;
        m_pGameStateCb->GetScores(iPlayer, iEnemy);
        std::string strPlayerScore = std::string("You: ") + std::to_string(iPlayer);
        std::string strEnemyScore = std::string("Enemy: ") + std::to_string(iEnemy);
        m_vButtons[eButton_You]->UpdateCaption(strPlayerScore);
        m_vButtons[eButton_Enemy]->UpdateCaption(strEnemyScore);
    }

    common::ePick player, enemy;
    m_pGameStateCb->GetPicks(player, enemy);
    if (player != common::ePick::None)
    {
        int x, y;
        x = (m_DrawingRect.w - m_DrawingRect.x) / 4 - m_vTexturePicks[player]->GetWidth() / 2;
        y = (m_DrawingRect.h - m_DrawingRect.y - 100) / 2 - m_vTexturePicks[player]->GetHeight() / 2;
        m_vTexturePicks[player]->Render(x, y);
    }
    if (enemy != common::ePick::None)
    {
        int x, y;
        x = 3 * (m_DrawingRect.w - m_DrawingRect.x) / 4 - m_vTexturePicks[enemy]->GetWidth() / 2;
        y = (m_DrawingRect.h - m_DrawingRect.y - 100) / 2 - m_vTexturePicks[enemy]->GetHeight() / 2;
        m_vTexturePicks[enemy]->Render(x, y);
    }
}

bool SceneGame::HandleEvent(SDL_Event* e)
{
    for (auto& i : m_vButtons)
    {
        i.second->HandleEvent(e);

        if (i.second->IsClicked())
        {
            i.second->ClickedReset();

            switch (i.first)
            {
            case eButton_Rock:
                m_pGameStateCb->OnPlayerPick(common::ePick::Rock);
                break;
            case eButton_Paper:
                m_pGameStateCb->OnPlayerPick(common::ePick::Paper);
                break;
            case eButton_Scissors:
                m_pGameStateCb->OnPlayerPick(common::ePick::Scissors);
                break;
            case eButton_BackToMenu:
                m_vButtons[eButton_You]->UpdateCaption("You: 0");
                m_vButtons[eButton_Enemy]->UpdateCaption("Enemy: 0");
                m_pGameStateCb->OnStateChange(IGameState::eState::eState_Menu);
                break;
            };
        }
    }

    return true;
}