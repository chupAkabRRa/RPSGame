#ifndef _RPS_CORE_H_
#define _RPS_CORE_H_

#include <memory>

class RPSEngine;

class RPSCore
{
public:
	RPSCore();
	~RPSCore();

    bool Initialize();
    void Run();

private:
    std::unique_ptr<RPSEngine> m_pEngine;
    bool m_bInitialized = false;
};

#endif // _RPD_CORE_H_