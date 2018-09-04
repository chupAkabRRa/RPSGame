#include "RPSCore.h"

#include "engine/RPSEngine.h"
#include "game/GameLogic.h"
#include "game/GameBot.h"
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
            m_pNetwork->ResetState();
            m_pEngine->SetActiveScene(RPSEngine::eScene::eScene_Menu);
            break;

        case IGameState::eState::eState_GameStartedOnline:
            m_pGameLogic->UpdatePickProvider(m_pNetwork);
            m_pEngine->SetActiveScene(RPSEngine::eScene::eScene_Game);
            break;

        case IGameState::eState::eState_GameStarted:
            m_pGameLogic->UpdatePickProvider(m_pBot);
            m_pEngine->SetActiveScene(RPSEngine::eScene::eScene_Game);
            break;

        case IGameState::eState::eState_LobbyCreating:
            CreateLobby();
            break;

        case IGameState::eState::eState_LobbySearching:
            SearchLobby();
            break;
        
        case IGameState::eState::eState_LobbyConnecting:
            JoinLobby();
            break;

        case IGameState::eState::eState_GameQuit:
            m_pNetwork->ResetState();
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
    // Set internal state
    m_pGameLogic->SetPlayerPick(pick);
    // Notify opponent
    m_pNetwork->SendOwnPick(pick);
}

void RPSCore::OnLobbyCreated(bool bResult)
{
    if (bResult)
    {
        m_currState = IGameState::eState::eState_LobbyCreated;
    }
}

void RPSCore::OnLobbyFound(bool bResult)
{
    if (bResult)
    {
        m_currState = IGameState::eState::eState_LobbyFound;
    }
}

void RPSCore::GetPicks(common::ePick& player, common::ePick& enemy)
{
    m_pGameLogic->GetAllPicks(player, enemy);
}

void RPSCore::GetScores(int& iPlayerScore, int& iEnemyScore)
{
    m_pGameLogic->GetScores(iPlayerScore, iEnemyScore);
}

std::string RPSCore::GetGOGUserName() const
{
    return m_strUserName;
}

void RPSCore::OnSignedIn(const std::string& strUserName)
{
    m_strUserName = strUserName;
}

//
// RPSCore own methods
//

bool RPSCore::Initialize(const std::string& strUser)
{
    m_pBot = std::make_shared<GameBot>();

    // Initialize render engine and network layer
    m_pEngine = std::make_unique<RPSEngine>();
    m_pNetwork = std::make_shared<RPSNetwork>();
    if (m_pEngine->Initialize(this) && m_pNetwork->Initialize(this))
    {
        m_pGameLogic = std::make_unique<GameLogic>(m_pBot, this);
        m_pGameLogic->ResetState();

        LOG(INFO) << "Trying to sing in to GOG as " << strUser;
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
            m_bInitialized = false;
        }
        else
        {
            if (m_pNetwork->SignIn(strName.c_str(), strPass.c_str()))
            {
                m_bInitialized = true;
            }
        }
    }
    else
    {
        LOG(ERROR) << "[CRITICAL] Render/network engine can't be initialized";
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

void RPSCore::CreateLobby()
{
    if (m_pNetwork->IsSignedIn())
    {
        std::string strLobbyName;
        if (RPSConfig::get().GetString("LobbyName", strLobbyName))
        {
            m_pEngine->SetActiveScene(RPSEngine::eScene::eScene_LobbyCreate);
            m_pNetwork->CreateLobby(strLobbyName);
        }
        else
        {
            LOG(ERROR) << "Lobby name is not specified";
        }
    }
    else
    {
        LOG(WARNING) << "Can't create lobby when you are not signed in";
    }
}

void RPSCore::SearchLobby()
{
    if (m_pNetwork->IsSignedIn())
    {
        std::string strLobbyName;
        if (RPSConfig::get().GetString("LobbyName", strLobbyName))
        {
            m_pEngine->SetActiveScene(RPSEngine::eScene::eScene_LobbyJoin);
            m_pNetwork->SearchLobby(strLobbyName);
        }
        else
        {
            LOG(ERROR) << "Lobby name is not specified";
        }
    }
}

void RPSCore::JoinLobby()
{
    m_pNetwork->JoinLobby();
}