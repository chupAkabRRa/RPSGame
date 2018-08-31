#ifndef _RPS_CORE_H_
#define _RPS_CORE_H_

#include <memory>

#include "IGameState.h"

class RPSEngine;

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

private:
    std::unique_ptr<RPSEngine> m_pEngine;
    bool m_bInitialized = false;
};

#endif // _RPD_CORE_H_