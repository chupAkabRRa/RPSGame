#include <string>

#include "RPSCore.h"
#include "RPSEngine.h"
#include "logger/easylogging++.h"

// To allow forward declaration of RSPEngine for unique_ptr
// and avoid inlining of these two methods
RPSCore::RPSCore() = default;
RPSCore::~RPSCore() = default;

bool RPSCore::Initialize()
{
    // Initialize render engine
    m_pEngine = std::make_unique<RPSEngine>();
    if (m_pEngine->Initialize())
    {
        m_bInitialized = true;
    }
    else
    {
        LOG(ERROR) << "[CRITICAL] Render engine can't be initialized";
        m_pEngine->Close();
        m_bInitialized = false;
    }

    return m_bInitialized;
}

void RPSCore::Run()
{
    if (m_bInitialized)
    {
        m_pEngine->GameLoop();
    }
    else
    {
        LOG(ERROR) << "Can't RUN uninitialized core";
    }
}
