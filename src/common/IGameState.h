#ifndef _IGAME_STATE_H_
#define _IGAME_STATE_H_

#include <string>
#include "RPSCommon.h"

class IGameState
{
public:
    enum eState
    {
        eState_Menu = 0,
        eState_GameStarted = 1,
        eState_GameStartedOnline = 2,
        eState_GameNewRound = 3,
        eState_GameRoundFinished = 4,
        eState_GameQuit = 5,
        eState_LobbyCreating = 6,
        eState_LobbyCreated = 7,
        eState_LobbySearching = 8,
        eState_LobbyFound = 9,
        eState_LobbyConnecting = 10
    };

    virtual ~IGameState() = default;
    virtual void OnStateChange(IGameState::eState newState) = 0;
    virtual void OnPlayerPick(common::ePick pick) = 0;
    virtual void OnLobbyCreated(bool bResult) = 0;
    virtual void OnLobbyFound(bool bResult) = 0;
    virtual void OnSignedIn(const std::string& strUserName) = 0;

    virtual void GetPicks(common::ePick& player, common::ePick& enemy) = 0;
    virtual void GetScores(int& iPlayerScore, int& iEnemyScore) = 0;
    virtual std::string GetGOGUserName() const = 0;
    
    IGameState::eState GetCurrState() const { return m_currState; }
protected:
    IGameState::eState m_currState = eState_Menu;
    bool m_bQuit = false;
};

#endif // _IGAME_STATE_H_