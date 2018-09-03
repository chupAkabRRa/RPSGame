#ifndef _GAME_LOGIC_H_
#define _GAME_LOGIC_H_

#include "RPSCommon.h"
#include "IGameState.h"

class GameLogic
{
public:
    class IPickProvider
    {
    public:
        virtual ~IPickProvider() = default;
        virtual common::ePick GetEnemyPick() = 0;
    };

    GameLogic(IPickProvider* pProvider, IGameState* cb);
    ~GameLogic() = default;

    bool IsRoundFinished() const { return m_bRoundFinished; }
    void NewRound() { m_bRoundFinished = false; m_PlayerPick = m_EnemyPick = common::ePick::None; }
    void ResetState();
    void GetScores(int& iPlayerScore, int& iEnemyScore) const
    {
        iPlayerScore = m_iPlayerScore;
        iEnemyScore = m_iEnemyScore;
    }

    void SetPlayerPick(common::ePick pick);
    void GetAllPicks(common::ePick& player, common::ePick& enemy);

private:
    IPickProvider* m_pPickProvider;
    IGameState* m_pGameStateCb;
    bool m_bRoundFinished = false;
    int m_iPlayerScore = 0;
    int m_iEnemyScore = 0;

    common::ePick m_PlayerPick = common::ePick::None;
    common::ePick m_EnemyPick = common::ePick::None;

    void UpdateResults();
};

#endif // _GAME_LOGIC_H_