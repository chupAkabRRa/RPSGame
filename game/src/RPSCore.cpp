#include <string>

#include "RPSCore.h"
#include "RPSEngine.h"
#include "GameLogic.h"

#include "logger/easylogging++.h"

// To allow forward declaration of RSPEngine for unique_ptr
// and avoid inlining of these two methods
RPSCore::RPSCore() = default;
RPSCore::~RPSCore() = default;

void RPSCore::OnSceneChange(IGameState::eScene newScene)
{
    if (newScene == IGameState::eScene::eScene_Menu)
    {
        m_pGameLogic->ResetState();
    }

    m_currScene = newScene;
}

void RPSCore::OnQuitApp()
{
    m_bQuit = true;
}

void RPSCore::OnPlayerPick(common::ePick pick)
{
    m_pGameLogic->SetPlayerPick(pick);
}

void RPSCore::GetPicks(common::ePick& player, common::ePick& enemy)
{
    m_pGameLogic->GetAllPicks(player, enemy);
}

bool RPSCore::IsRoundFinished()
{
    return m_pGameLogic->IsRoundFinished();
}

void RPSCore::OnNewRound()
{
    m_pGameLogic->NewRound();
}

void RPSCore::GetScores(int& iPlayerScore, int& iEnemyScore)
{
    m_pGameLogic->GetScores(iPlayerScore, iEnemyScore);
}

bool RPSCore::Initialize()
{
    // Initialize render engine
    m_pEngine = std::make_unique<RPSEngine>();
    if (m_pEngine->Initialize(this))
    {
        m_bInitialized = true;

        m_pGameLogic = std::make_unique<GameLogic>(&m_bot);
        m_pGameLogic->ResetState();
    }
    else
    {
        LOG(ERROR) << "[CRITICAL] Render engine can't be initialized";
        m_pEngine->Close();
        m_bInitialized = false;
    }

    return m_bInitialized;
}

void RPSCore::Run()
{
    if (m_bInitialized)
    {
        m_pEngine->GameLoop();
    }
    else
    {
        LOG(ERROR) << "Can't RUN uninitialized core";
    }
}
