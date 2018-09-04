#include "LobbyClient.h"

#include "logger/easylogging++.h"

#include <thread>
#include <chrono>

LobbyClient::~LobbyClient()
{
    if (m_bIsConnected)
    {
        if (!LeaveLobbySync())
        {
            LOG(WARNING) << "Can't leave lobby correctly";
        }
    }
    
    m_vListeners.clear();
}

void LobbyClient::Initialize(IGameState* cb)
{
    m_vListeners.emplace_back(std::make_unique<LobbyListListener>(this));
    m_vListeners.emplace_back(std::make_unique<LobbyEnteredListener>(this));
    m_pGameStateCb = cb;
}

void LobbyClient::SearchLobby(const std::string& strLobbyName)
{
    m_bIsLobbyFound = false;
    m_bIsConnected = false;
    m_strLobbyName = strLobbyName;

    galaxy::api::Matchmaking()->AddRequestLobbyListStringFilter("name", strLobbyName.c_str(),
        galaxy::api::LobbyComparisonType::LOBBY_COMPARISON_TYPE_EQUAL);
    galaxy::api::Matchmaking()->RequestLobbyList();
}

void LobbyClient::JoinLobby()
{
    if (m_bIsLobbyFound)
    {
        galaxy::api::Matchmaking()->JoinLobby(m_lobbyId);
    }
}

bool LobbyClient::LeaveLobbySync()
{
    galaxy::api::Matchmaking()->LeaveLobby(m_lobbyId);

    // Let's make this method sync
    int attempts = 0;
    do
    {
        galaxy::api::ProcessData();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        attempts++;
    } while (attempts < 10 && m_bIsConnected);

    return !m_bIsConnected;
}

//
// LobbyListListener
//

LobbyClient::LobbyListListener::LobbyListListener(LobbyClient* pWrapper)
    : m_pWrapper(pWrapper)
{
}

void LobbyClient::LobbyListListener::OnLobbyList(uint32_t lobbyCount, bool ioFailure)
{
    if (lobbyCount > 0 && !ioFailure)
    {
        // Let's take only first lobby
        m_pWrapper->m_lobbyId = galaxy::api::Matchmaking()->GetLobbyByIndex(0);
        m_pWrapper->m_pGameStateCb->OnLobbyFound(true);
        m_pWrapper->m_bIsLobbyFound = true;
    }
    else
    {
        LOG(WARNING) << "Can't find specified lobby";
        m_pWrapper->m_pGameStateCb->OnLobbyFound(false);
        m_pWrapper->m_bIsLobbyFound = true;
    }
}

//
// LobbyEnteredListener
//

LobbyClient::LobbyEnteredListener::LobbyEnteredListener(LobbyClient* pWrapper)
    : m_pWrapper(pWrapper)
{
}

void LobbyClient::LobbyEnteredListener::OnLobbyEntered(const galaxy::api::GalaxyID& lobbyID,
    galaxy::api::LobbyEnterResult result)
{
    if (result == galaxy::api::LobbyEnterResult::LOBBY_ENTER_RESULT_SUCCESS)
    {
        m_pWrapper->m_lobbyId = lobbyID;
        if (galaxy::api::Matchmaking()->GetNumLobbyMembers(lobbyID) > 1)
        {
            m_pWrapper->m_enemyId = galaxy::api::Matchmaking()->GetLobbyMemberByIndex(lobbyID, 1);
            m_pWrapper->m_pGameStateCb->OnStateChange(IGameState::eState::eState_GameStartedOnline);
        }
    }
}