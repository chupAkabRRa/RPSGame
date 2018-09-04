#ifndef _RPS_NETWORK_H_
#define _RPS_NETWORK_H_

#include <string>
#include <vector>
#include <memory>

#include "IGameState.h"
#include "RPSCommon.h"
#include "gog/GalaxyApi.h"

class LobbyHost;
class LobbyClient;

class RPSNetwork : public common::IPickProvider
{
public:
    RPSNetwork();
    ~RPSNetwork();

    bool Initialize(IGameState* cb);
    void ProcessData();

    bool SignIn(const std::string& strName, const std::string& strPass);
    bool IsSignedIn() const;

    bool CreateLobby(const std::string& strLobbyName);
    bool SearchLobby(const std::string& strLobbyName);
    bool JoinLobby();

    bool SendOwnPick(common::ePick pick);
    // common::IPickProvider
    common::ePick GetEnemyPick() override;

private:
    class AuthListener : public galaxy::api::GlobalAuthListener
    {
    public:
        AuthListener(RPSNetwork* pWrapper);
        virtual ~AuthListener() = default;
        virtual void OnAuthSuccess() override;
        virtual void OnAuthFailure(FailureReason reason) override;
        virtual void OnAuthLost() override;
    private:
        RPSNetwork* m_pWrapper;
    };

    void InitializeInternal();

    IGameState* m_pGameStateCb;
    bool m_bIsGogInitialized = false;
    
    bool m_bIsUserSignedIn = false;
    std::string m_strUserName;

    std::unique_ptr<LobbyHost> m_pHost;
    std::unique_ptr<LobbyClient> m_pClient;

    std::vector<std::unique_ptr<galaxy::api::IGalaxyListener>> m_vListeners;
};

#endif // _RPS_NETWORK_H_