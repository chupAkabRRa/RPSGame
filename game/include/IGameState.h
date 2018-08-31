#ifndef _IGAME_STATE_H_
#define _IGAME_STATE_H_

class IGameState
{
public:
    enum class eScene
    {
        eScene_Menu = 0,
        eScene_Game = 1,
    };

    virtual ~IGameState() = default;
    virtual void OnSceneChange(IGameState::eScene newScene) = 0;
    virtual void OnQuitApp() = 0;
    
    IGameState::eScene GetCurrScene() const { return m_currScene; }
    bool GetQuitState() const { return m_bQuit; }

protected:
    IGameState::eScene m_currScene;
    bool m_bQuit = false;
};

#endif // _IGAME_STATE_H_