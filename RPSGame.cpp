#include <memory>
#include <string>
#include <algorithm>

#include "src/RPSCore.h"

#include "logger/easylogging++.h"
#include "config/RPSConfig.h"

INITIALIZE_EASYLOGGINGPP

//
// Main entry poin.
// To execute the program, please, specify user you want to be like that:
// "RPSGame.exe UserA[/UserB]"
//

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

        if (argc < 2)
        {
            LOG(ERROR) << "[CRITICAL] Invalid number of arguments. Please, use following format: \"RPSGame.exe UserA[/UserB]\"";
        }
        else
        {
            std::string strUser(argv[1]);
            std::transform(strUser.begin(), strUser.end(), strUser.begin(), ::tolower);
            if (strUser == "usera" || strUser == "userb")
            {
                // We are ready for kick-off
                std::unique_ptr<RPSCore> pCore = std::make_unique<RPSCore>();
                if (pCore->Initialize(strUser))
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
                LOG(ERROR) << "[CRITICAL] Wrong user specified. User can be either UserA or UserB";
            }
        }
    }
    else
    {
        LOG(ERROR) << "[CRITICAL] Can't load main configuration file";
    }

    return 0;
}