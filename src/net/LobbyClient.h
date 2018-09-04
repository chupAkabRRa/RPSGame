#ifndef _LOBBY_CLIENT_H_
#define _LOBBY_CLIENT_H_

#include <string>
#include <vector>
#include <memory>

#include "IGameState.h"
#include "SenderReceiver.h"

#include "gog/GalaxyApi.h"

class LobbyClient : public SenderReceiver
{
public:
    LobbyClient() = default;
    ~LobbyClient();

    void Initialize(IGameState* cb);
    void SearchLobby(const std::string& strLobbyName);
    void JoinLobby();
private:
    class LobbyListListener : public galaxy::api::GlobalLobbyListListener
    {
    public:
        LobbyListListener(LobbyClient* pWrapper);
        virtual ~LobbyListListener() = default;
        virtual void OnLobbyList(uint32_t lobbyCount, bool ioFailure) override;
    private:
        LobbyClient* m_pWrapper;
    };

    class LobbyEnteredListener : public galaxy::api::GlobalLobbyEnteredListener
    {
    public:
        LobbyEnteredListener(LobbyClient* pWrapper);
        virtual ~LobbyEnteredListener() = default;
        virtual void OnLobbyEntered (const galaxy::api::GalaxyID &lobbyID,
            galaxy::api::LobbyEnterResult result) override;
    private:
        LobbyClient* m_pWrapper;
    };

    bool LeaveLobbySync();

    IGameState* m_pGameStateCb;
    std::string m_strLobbyName;
    bool m_bIsLobbyFound = false;
    bool m_bIsConnected = false;

    galaxy::api::GalaxyID m_lobbyId;

    std::vector<std::unique_ptr<galaxy::api::IGalaxyListener>> m_vListeners;
};

#endif // _LOBBY_CLIENT_H_