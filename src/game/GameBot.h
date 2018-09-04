#ifndef _GAME_BOT_H_
#define _GAME_BOT_H_

#include <random>

#include "RPSCommon.h"
#include "GameLogic.h"

class GameBot : public common::IPickProvider
{
public:
    GameBot() = default;
    ~GameBot() = default;

    common::ePick GetEnemyPick() override
    {
        std::mt19937 rng;
        rng.seed(std::random_device()());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, 3);

        return static_cast<common::ePick>(dist(rng));
    }
};

#endif // _GAME_BOT_H_