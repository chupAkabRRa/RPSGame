#include "LobbyHost.h"

#include "logger/easylogging++.h"

#include <thread>
#include <chrono>

LobbyHost::~LobbyHost()
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

void LobbyHost::Initialize(IGameState* cb)
{
    m_vListeners.emplace_back(std::make_unique<LobbyCreateListener>(this));
    m_vListeners.emplace_back(std::make_unique<LobbyMembersListener>(this));
    m_vListeners.emplace_back(std::make_unique<LobbyLeftListener>(this));
    m_pGameStateCb = cb;
}

void LobbyHost::CreateLobby(const std::string& strLobbyName)
{
    m_strLobbyName = strLobbyName;

    galaxy::api::Matchmaking()->CreateLobby(galaxy::api::LobbyType::LOBBY_TYPE_PUBLIC, 2,
        true, galaxy::api::LobbyTopologyType::LOBBY_TOPOLOGY_TYPE_FCM);
}

bool LobbyHost::LeaveLobbySync()
{
    // TODO: why crash?
    //galaxy::api::Matchmaking()->LeaveLobby(m_lobbyId);

    // Let's make this method sync
    int attempts = 0;
    do
    {
        galaxy::api::ProcessData();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        attempts++;
    } while (attempts < 1 && m_bIsConnected);

    return !m_bIsConnected;
}

//
// LobbyCreateListener
//

LobbyHost::LobbyCreateListener::LobbyCreateListener(LobbyHost* pWrapper)
    : m_pWrapper(pWrapper)
{
}

void LobbyHost::LobbyCreateListener::OnLobbyCreated(const galaxy::api::GalaxyID& id,
    galaxy::api::LobbyCreateResult res)
{
    if (res == galaxy::api::LobbyCreateResult::LOBBY_CREATE_RESULT_SUCCESS)
    {
        galaxy::api::Matchmaking()->SetLobbyData(id, "name", m_pWrapper->m_strLobbyName.c_str());
        m_pWrapper->m_pGameStateCb->OnLobbyCreated(true);
        m_pWrapper->m_lobbyId = id;
        m_pWrapper->m_bIsConnected = true;
    }
    else
    {
        LOG(ERROR) << "Can't create lobby";
        m_pWrapper->m_pGameStateCb->OnLobbyCreated(false);
    }
}

//
// LobbyMembersListener
//

LobbyHost::LobbyMembersListener::LobbyMembersListener(LobbyHost* pWrapper)
    : m_pWrapper(pWrapper)
{
}

void LobbyHost::LobbyMembersListener::OnLobbyMemberStateChanged(const galaxy::api::GalaxyID& lobbyId,
    const galaxy::api::GalaxyID& memberId, galaxy::api::LobbyMemberStateChange state)
{
    if (state == galaxy::api::LobbyMemberStateChange::LOBBY_MEMBER_STATE_CHANGED_ENTERED)
    {
        m_pWrapper->m_enemyId = memberId;
        m_pWrapper->m_pGameStateCb->OnStateChange(IGameState::eState::eState_GameStartedOnline);
    }   
}

//
// LobbyLeftListener
//

LobbyHost::LobbyLeftListener::LobbyLeftListener(LobbyHost* pWrapper)
    : m_pWrapper(pWrapper)
{
}

void LobbyHost::LobbyLeftListener::OnLobbyLeft(const galaxy::api::GalaxyID& lobbyId, bool ioFailure)
{
    m_pWrapper->m_bIsConnected = false;
}
