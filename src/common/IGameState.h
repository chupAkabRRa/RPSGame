#ifndef _IGAME_STATE_H_
#define _IGAME_STATE_H_

#include "RPSCommon.h"

class IGameState
{
public:
    enum eState
    {
        eState_Menu = 0,
        eState_GameStarted = 1,
        eState_GameNewRound = 2,
        eState_GameRoundFinished = 3,
        eState_GameQuit = 4
    };

    virtual ~IGameState() = default;
    virtual void OnStateChange(IGameState::eState newState) = 0;
    virtual void OnPlayerPick(common::ePick pick) = 0;
    virtual void GetPicks(common::ePick& player, common::ePick& enemy) = 0;
    virtual void GetScores(int& iPlayerScore, int& iEnemyScore) = 0;
    
    IGameState::eState GetCurrState() const { return m_currState; }
protected:
    IGameState::eState m_currState = eState_Menu;
    bool m_bQuit = false;
};

#endif // _IGAME_STATE_H_