#ifndef _IGAME_STATE_H_
#define _IGAME_STATE_H_

#include "RPSCommon.h"

class IGameState
{
public:
    enum eScene
    {
        eScene_Menu = 0,
        eScene_Game = 1,
    };

    virtual ~IGameState() = default;
    virtual void OnSceneChange(IGameState::eScene newScene) = 0;
    virtual void OnQuitApp() = 0;
    virtual void OnPlayerPick(common::ePick pick) = 0;
    virtual void GetPicks(common::ePick& player, common::ePick& enemy) = 0;
    virtual bool IsRoundFinished() = 0;
    virtual void OnNewRound() = 0;
    
    IGameState::eScene GetCurrScene() const { return m_currScene; }
    bool GetQuitState() const { return m_bQuit; }

protected:
    IGameState::eScene m_currScene = eScene_Menu;
    bool m_bQuit = false;
};

#endif // _IGAME_STATE_H_