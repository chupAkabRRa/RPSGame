#ifndef _LOBBY_HOST_H_
#define _LOBBY_HOST_H_

#include <string>
#include <vector>
#include <memory>

#include "IGameState.h"
#include "gog/GalaxyApi.h"

class LobbyHost
{
public:
    LobbyHost() = default;
    ~LobbyHost();

    void Initialize(IGameState* cb);
    void CreateLobby(const std::string& strLobbyName);
private:
    class LobbyCreateListener : public galaxy::api::GlobalLobbyCreatedListener
    {
    public:
        LobbyCreateListener(LobbyHost* pWrapper);
        virtual ~LobbyCreateListener() = default;
        virtual void OnLobbyCreated(const galaxy::api::GalaxyID& id,
            galaxy::api::LobbyCreateResult res) override;
    private:
        LobbyHost* m_pWrapper;
    };

    class LobbyMembersListener : public galaxy::api::GlobalLobbyMemberStateListener
    {
    public:
        LobbyMembersListener(LobbyHost* pWrapper);
        virtual ~LobbyMembersListener() = default;
        virtual void OnLobbyMemberStateChanged(const galaxy::api::GalaxyID& lobbyId,
            const galaxy::api::GalaxyID& memberId, galaxy::api::LobbyMemberStateChange state) override;
    private:
        LobbyHost* m_pWrapper;
    };

    class LobbyLeftListener : public galaxy::api::GlobalLobbyLeftListener
    {
    public:
        LobbyLeftListener(LobbyHost* pWrapper);
        virtual ~LobbyLeftListener() = default;
        virtual void OnLobbyLeft(const galaxy::api::GalaxyID& lobbyId, bool ioFailure) override;
    private:
        LobbyHost* m_pWrapper;
    };

    bool LeaveLobbySync();

    IGameState* m_pGameStateCb;
    std::string m_strLobbyName;
    bool m_bIsConnected = false;
    galaxy::api::GalaxyID m_lobbyId;
    galaxy::api::GalaxyID m_enemyId;

    std::vector<std::unique_ptr<galaxy::api::IGalaxyListener>> m_vListeners;
};

#endif // _LOBBY_HOST_H_