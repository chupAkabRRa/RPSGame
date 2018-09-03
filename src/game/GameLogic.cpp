#include "GameLogic.h"

GameLogic::GameLogic(IPickProvider* pProvider, IGameState* cb)
    : m_pPickProvider(pProvider)
    , m_pGameStateCb(cb)
{}

void GameLogic::UpdateResults()
{
    if (m_PlayerPick != common::ePick::None && m_EnemyPick != common::ePick::None)
    {
        if (m_PlayerPick == m_EnemyPick)
        {
            m_iPlayerScore++;
            m_iEnemyScore++;
        }
        else
        {
            if ((m_PlayerPick == common::ePick::Rock && m_EnemyPick == common::ePick::Scissors) ||
                (m_PlayerPick == common::ePick::Paper && m_EnemyPick == common::ePick::Rock) ||
                (m_PlayerPick == common::ePick::Scissors && m_EnemyPick == common::ePick::Paper))
            {
                m_iPlayerScore++;
            }
            else
            {
                m_iEnemyScore++;
            }
        }

        m_bRoundFinished = true;
        m_pGameStateCb->OnStateChange(IGameState::eState::eState_GameRoundFinished);
    }
}

void GameLogic::SetPlayerPick(common::ePick pick)
{
    if (m_PlayerPick == common::ePick::None)
    {
        m_PlayerPick = pick;
    }
}

void GameLogic::GetAllPicks(common::ePick& player, common::ePick& enemy)
{
    // Let's check the availability of enemy's result first
    if (m_EnemyPick == common::ePick::None && m_pPickProvider)
    {
        m_EnemyPick = m_pPickProvider->GetEnemyPick();
    }

    if (m_PlayerPick == common::ePick::None)
    {
        // Do not show anything if player hasn't made a choice yet
        player = common::ePick::None;
        enemy = common::ePick::None;
    }
    else
    {
        // We are ok to render
        player = m_PlayerPick;
        enemy = m_EnemyPick;
    }
    
    UpdateResults();
}

void GameLogic::ResetState()
{
    NewRound();
    m_iPlayerScore = 0;
    m_iEnemyScore = 0;
}