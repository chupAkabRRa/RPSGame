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

    SDL_Color textColor = { 255, 255, 255 };
    m_vButtons.push_back(std::make_unique<LButton>(m_pRenderer, "Rock", m_strFontName, m_iFontSize, textColor));
    m_vButtons.push_back(std::make_unique<LButton>(m_pRenderer, "Paper", m_strFontName, m_iFontSize, textColor));
    m_vButtons.push_back(std::make_unique<LButton>(m_pRenderer, "Scissors", m_strFontName, m_iFontSize, textColor));
    m_vButtons.push_back(std::make_unique<LButton>(m_pRenderer, "<-", m_strFontName, m_iFontSize, textColor));
    m_vButtons.push_back(std::make_unique<LButton>(m_pRenderer, "You: 0", m_strFontName, m_iFontSize, textColor, false));
    m_vButtons.push_back(std::make_unique<LButton>(m_pRenderer, "Enemy: 0", m_strFontName, m_iFontSize, textColor, false));

    for (auto& i : m_vButtons)
    {
        i->Initialize();
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
        i->Render();
    }
}

bool SceneGame::HandleEvent(SDL_Event* e)
{
    for (std::size_t i = 0; i < m_vButtons.size(); i++)
    {
        m_vButtons[i]->HandleEvent(e);

        if (m_vButtons[i]->IsClicked())
        {
            m_vButtons[i]->ClickedReset();

            switch (i)
            {
            case eButton_Rock: 
                break;
            case eButton_Paper:
                break;
            case eButton_Scissors:
                break;
            case eButton_BackToMenu:
                m_pGameStateCb->OnSceneChange(IGameState::eScene::eScene_Menu);
                break;
            };
        }
    }

    return true;
}