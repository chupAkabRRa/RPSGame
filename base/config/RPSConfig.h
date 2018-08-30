#ifndef _RPS_CONFIG_H_
#define _RPS_CONFIG_H_

#include "json/document.h"

// Singleton class which is responsible for:
// 1) Loading of correct global settings file (it may be located either in
//    app folder somewhere (after installation) or be local to binary (tests))
// 2) Parsing of this config file
// 3) Providing values for requested params

class RPSConfig
{
public:
   static RPSConfig& get()
   {
       static RPSConfig instance;
       return instance;
   }
   RPSConfig(const RPSConfig&) = delete;
   RPSConfig& operator=(const RPSConfig&) = delete;

   bool Init();

   bool GetBool(const char* option, bool& bVal);
   bool GetInt(const char* option, int& iVal);
   bool GetString(const char* option, std::string& strVal);

private:
   RPSConfig(){};

   const std::string m_strCfgPath = "./RPSGame.conf";
   rapidjson::Document m_jsonDocument;
   bool m_bInitialized = false;
};

#endif // _RPS_CONFIG_H_