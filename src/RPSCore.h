#ifndef _RPS_CORE_H_
#define _RPS_CORE_H_

#include <memory>

#include "IGameState.h"
#include "game/GameBot.h"

class RPSEngine;
class GameLogic;

class RPSCore : public IGameState
{
public:
	RPSCore();
	~RPSCore();

    bool Initialize();
    void Run();

    // IGameState methods
    void OnSceneChange(IGameState::eScene newScene) override;
    void OnQuitApp() override;
    void OnPlayerPick(common::ePick pick) override;
    void GetPicks(common::ePick& player, common::ePick& enemy) override;
    bool IsRoundFinished() override;
    void OnNewRound() override;
    void GetScores(int& iPlayerScore, int& iEnemyScore) override;

private:
    std::unique_ptr<RPSEngine> m_pEngine;
    bool m_bInitialized = false;

    std::unique_ptr<GameLogic> m_pGameLogic;
    GameBot m_bot;
};

#endif // _RPD_CORE_H_