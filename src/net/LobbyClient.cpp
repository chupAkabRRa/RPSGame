#include "LobbyClient.h"

#include "logger/easylogging++.h"

LobbyClient::~LobbyClient()
{
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

bool LobbyClient::ReadData(std::string& strData)
{
    uint32_t size = 0;
    if (galaxy::api::Networking()->IsP2PPacketAvailable(&size))
    {
        char data[256] = {0};
        uint32_t sizeOut = 0;
        galaxy::api::GalaxyID outId;

        if (galaxy::api::Networking()->ReadP2PPacket((void*)data, 256, &sizeOut, outId))
        {
            strData = std::string(data);
            return true;
        }
    }

    return false;
}

bool LobbyClient::SendData(const std::string& strData)
{
    return galaxy::api::Networking()->SendP2PPacket(m_enemyId,
        (const void*)strData.c_str(), strData.size(), galaxy::api::P2PSendType::P2P_SEND_RELIABLE);
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