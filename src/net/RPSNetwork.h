#ifndef _RPS_NETWORK_H_
#define _RPS_NETWORK_H_

#include <string>
#include <vector>
#include <memory>

#include "IGameState.h"
#include "gog/GalaxyApi.h"

class RPSNetwork
{
public:
    RPSNetwork() = default;
    ~RPSNetwork();

    bool Initialize(IGameState* cb);
    void ProcessData();
    bool SignIn(const std::string& strName, const std::string& strPass);

private:
    class AuthListener : public galaxy::api::GlobalAuthListener
    {
    public:
        AuthListener(RPSNetwork* pWrapper);
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

    std::vector<std::unique_ptr<galaxy::api::IGalaxyListener>> m_vListeners;
};

#endif // _RPS_NETWORK_H_