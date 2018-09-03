#include "RPSCore.h"
#include "engine/RPSEngine.h"
#include "game/GameLogic.h"
#include "net/RPSNetwork.h"

#include "logger/easylogging++.h"
#include "config/RPSConfig.h"

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

        case IGameState::eState::eState_LobbyCreate:
            m_pEngine->SetActiveScene(RPSEngine::eScene::eScene_LobbyCreate);
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

std::string RPSCore::GetGOGUserName()
{
    return m_strUserName;
}

//
// RPSCore own methods
//

bool RPSCore::Initialize(const std::string& strUser)
{
    // Initialize render engine and network layer
    m_pEngine = std::make_unique<RPSEngine>();
    m_pNetwork = std::make_unique<RPSNetwork>();
    if (m_pEngine->Initialize(this) && m_pNetwork->Initialize(this))
    {
        m_pGameLogic = std::make_unique<GameLogic>(&m_bot, this);
        m_pGameLogic->ResetState();

        LOG(INFO) << "Trying to connect " << strUser;
        std::string strName, strPass;
        std::string strNameKey, strPassKey;
        if (strUser == "usera")
        {
            strNameKey = "UserA_Login";
            strPassKey = "UserA_Pass";
        }
        else
        {
            strNameKey = "UserB_Login";
            strPassKey = "UserB_Pass";
        }

        if (!RPSConfig::get().GetString(strNameKey.c_str(), strName) ||
            !RPSConfig::get().GetString(strPassKey.c_str(), strPass))
        {
            LOG(ERROR) << "[CRITICAL] Can't find credentials for " << strUser;
            m_pEngine->Close();
            m_bInitialized = false;
        }
        else
        {
            if (m_pNetwork->SignIn(strName.c_str(), strPass.c_str()))
            {
                m_strUserName = strName;
                m_bInitialized = true;
            }
        }
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
        while (!m_bQuit)
        {
            m_pEngine->Animate();
            m_pNetwork->ProcessData();
        }
    }
    else
    {
        LOG(ERROR) << "Can't RUN uninitialized core";
    }
}
