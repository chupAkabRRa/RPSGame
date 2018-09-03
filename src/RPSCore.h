#ifndef _RPS_CORE_H_
#define _RPS_CORE_H_

#include <memory>
#include <string>

#include "IGameState.h"
#include "game/GameBot.h"

class RPSEngine;
class RPSNetwork;
class GameLogic;

class RPSCore : public IGameState
{
public:
	RPSCore();
	~RPSCore();

    bool Initialize(const std::string& strUser);
    void Run();

    // IGameState methods
    void OnStateChange(IGameState::eState newState) override;
    void OnPlayerPick(common::ePick pick) override;
    void OnLobbyCreated(bool bResult) override;
    void OnLobbyFound(bool bResult) override;

    void GetPicks(common::ePick& player, common::ePick& enemy) override;
    void GetScores(int& iPlayerScore, int& iEnemyScore) override;
    std::string GetGOGUserName() override;

private:
    void CreateLobby();
    void SearchLobby();
    void JoinLobby();

    std::unique_ptr<RPSEngine> m_pEngine;
    std::unique_ptr<RPSNetwork> m_pNetwork;
    bool m_bInitialized = false;
    bool m_bConnected = false;

    std::string m_strUserName;

    std::unique_ptr<GameLogic> m_pGameLogic;
    GameBot m_bot;
};

#endif // _RPD_CORE_H_