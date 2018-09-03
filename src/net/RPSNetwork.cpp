#include "RPSNetwork.h"

#include "LobbyHost.h"
#include "LobbyClient.h"

#include "logger/easylogging++.h"
#include "config/RPSConfig.h"

#include <thread>
#include <chrono>

RPSNetwork::RPSNetwork() = default;

RPSNetwork::~RPSNetwork()
{
    m_vListeners.clear();

    if (m_bIsGogInitialized)
    {
        galaxy::api::Shutdown();
        m_bIsGogInitialized = false;
    }
}

bool RPSNetwork::Initialize(IGameState* cb)
{
    bool bRes = false;
    std::string strClientId, strClientSecret;

    if (!RPSConfig::get().GetString("GOG_ClientId", strClientId) ||
        !RPSConfig::get().GetString("GOG_ClientSecret", strClientSecret))
    {
        LOG(ERROR) << "Can't initialize GOG: CLIENT_ID/CLIENT_SECRET can't be found";
        bRes = false;
    }
    else
    {
        try
        {
            galaxy::api::Init({strClientId.c_str(), strClientSecret.c_str()});
            InitializeInternal();
            m_pGameStateCb = cb;
            m_bIsGogInitialized = bRes = true;
        }
        catch (const galaxy::api::IError& e)
        {
            LOG(ERROR) << "Can't initialize GOG: " << e.GetMsg();
            bRes = false;
        } 
    }

    return bRes;
}

void RPSNetwork::ProcessData()
{
    if (m_bIsGogInitialized)
    {
        galaxy::api::ProcessData();
    }
}

bool RPSNetwork::SignIn(const std::string& strName, const std::string& strPass)
{
    if (m_bIsGogInitialized)
    {
        galaxy::api::User()->SignIn(strName.c_str(), strPass.c_str());
        
        // Let's make this method sync
        int attempts = 0;
        do
        {
            galaxy::api::ProcessData();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            attempts++;
        } while (attempts < 10 && !m_bIsUserSignedIn);

        return m_bIsUserSignedIn;
    }

    return false;
}

void RPSNetwork::InitializeInternal()
{
    m_vListeners.emplace_back(std::make_unique<AuthListener>(this));
    m_pHost = nullptr;
}

bool RPSNetwork::CreateLobby(const std::string& strLobbyName)
{
    bool bRes = false;

    if (m_bIsGogInitialized && m_bIsUserSignedIn)
    {
        m_pHost.reset(new LobbyHost);
        m_pHost->Initialize(m_pGameStateCb);
        m_pHost->CreateLobby(strLobbyName);
        bRes = true;
    }

    return bRes;
}

bool RPSNetwork::SearchLobby(const std::string& strLobbyName)
{
    bool bRes = false;

    if (m_bIsGogInitialized && m_bIsUserSignedIn)
    {
        m_pClient.reset(new LobbyClient);
        m_pClient->Initialize(m_pGameStateCb);
        m_pClient->SearchLobby(strLobbyName);
        bRes = true;       
    }

    return bRes;
}

bool RPSNetwork::JoinLobby()
{
    bool bRes = false;

    if (m_bIsGogInitialized && m_bIsUserSignedIn && m_pClient)
    {
        m_pClient->JoinLobby();
        bRes = true;
    }

    return bRes;
}

//
// AuthListener
//

RPSNetwork::AuthListener::AuthListener(RPSNetwork* pWrapper)
    : m_pWrapper(pWrapper)
{
}

void RPSNetwork::AuthListener::OnAuthSuccess()
{
    LOG(INFO) << "Connected";
    m_pWrapper->m_bIsUserSignedIn = true;
}

void RPSNetwork::AuthListener::OnAuthFailure(FailureReason reason)
{
    LOG(ERROR) << "Can't sign in: " << reason;
    m_pWrapper->m_bIsUserSignedIn = false;
}

void RPSNetwork::AuthListener::OnAuthLost()
{

}
