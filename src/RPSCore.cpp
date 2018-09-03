#include <string>

#include "RPSCore.h"
#include "engine/RPSEngine.h"
#include "game/GameLogic.h"

#include "logger/easylogging++.h"

// To allow forward declaration of RSPEngine for unique_ptr
// and avoid inlining of these two methods
RPSCore::RPSCore() = default;
RPSCore::~RPSCore() = default;

//
// IGameState callbacks
//

void RPSCore::OnStateChange(IGameState::eState newState)
{
    if (newState != m_currState)
    {
        switch (newState)
        {
        case IGameState::eState::eState_Menu:
            m_pGameLogic->ResetState();
            m_pEngine->SetActiveScene(RPSEngine::eScene::eScene_Menu);
            break;

        case IGameState::eState::eState_GameStarted:
            m_pEngine->SetActiveScene(RPSEngine::eScene::eScene_Game);
            break;

        case IGameState::eState::eState_GameQuit:
            m_bQuit = true;
            break;

        case IGameState::eState::eState_GameNewRound:
            m_pGameLogic->NewRound();
            break;
        }

        m_currState = newState;
    }
}

void RPSCore::OnPlayerPick(common::ePick pick)
{
    m_pGameLogic->SetPlayerPick(pick);
}

void RPSCore::GetPicks(common::ePick& player, common::ePick& enemy)
{
    m_pGameLogic->GetAllPicks(player, enemy);
}

void RPSCore::GetScores(int& iPlayerScore, int& iEnemyScore)
{
    m_pGameLogic->GetScores(iPlayerScore, iEnemyScore);
}

//
// RPSCore own methods
//

bool RPSCore::Initialize()
{
    // Initialize render engine and network layer
    m_pEngine = std::make_unique<RPSEngine>();
    if (m_pEngine->Initialize(this))
    {
        m_bInitialized = true;

        m_pGameLogic = std::make_unique<GameLogic>(&m_bot, this);
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
