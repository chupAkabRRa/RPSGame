#ifndef _RPS_COMMON_H_
#define _RPS_COMMON_H_

namespace common
{
    enum ePick
    {
        None = 0,
        Rock = 1,
        Paper = 2,
        Scissors = 3
    };

    class IPickProvider
    {
    public:
        virtual ~IPickProvider() = default;
        virtual common::ePick GetEnemyPick() = 0;
    };
}

#endif // _RPS_COMMON_H_