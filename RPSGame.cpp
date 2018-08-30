#include <memory>

#include "logger/easylogging++.h"
#include "config/RPSConfig.h"
#include "RPSCore.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    if (RPSConfig::get().Init())
    {
        LOG(INFO) << "Starting RSPGame(v.0.1)";

        std::string strLoggerCfgPath;
        if (RPSConfig::get().GetString("LoggerConfig", strLoggerCfgPath))
        {
            el::Configurations logConf(strLoggerCfgPath);
            el::Loggers::reconfigureLogger("default", logConf);
            LOG(INFO) << "Logger config successfully loaded";
        }
        else
        {
            LOG(WARNING) << "Using logging with default settings";
        }

        // We are ready for kick-off
        std::unique_ptr<RPSCore> pCore = std::make_unique<RPSCore>();
        if (pCore->Initialize())
        {
            pCore->Run();
        }
        else
        {
            LOG(ERROR) << "[CRITICAL] Core can't be initialized";
        }
    }
    else
    {
        LOG(ERROR) << "[CRITICAL] Can't load main configuration file";
    }

    return 0;
}