#include "RPSConfig.h"

#include <fstream>
#include <streambuf>

#include "logger/easylogging++.h"

bool RPSConfig::Init()
{
   std::ifstream cfg(m_strCfgPath);
   if (!cfg)
   {
       return false;
   }

   // This method should be changed if config file appears to be larger than
   // 50 KB because of performance issues
   std::stringstream buffer;
   buffer << cfg.rdbuf();

   // Let’s parse config
   if (!m_jsonDocument.Parse(buffer.str().c_str()).HasParseError())
   {
       m_bInitialized = true;
   }
   else
   {
       return false;
   }

   return true;
}

bool RPSConfig::GetBool(const char* option, bool& bVal)
{
   if (m_bInitialized)
   {
       if (m_jsonDocument.HasMember(option) && m_jsonDocument[option].IsBool())
       {
           bVal = m_jsonDocument[option].GetBool();
           return true;
       }
   }

   LOG(ERROR) << "Wrong BOOL property requested: " << option;

   return false;
}

bool RPSConfig::GetInt(const char* option, int& iVal)
{
   if (m_bInitialized)
   {
       if (m_jsonDocument.HasMember(option) && m_jsonDocument[option].IsInt())
       {
           iVal = m_jsonDocument[option].GetInt();
           return true;
       }
   }

   LOG(ERROR) << "Wrong INT property requested: " << option;

   return false;
}

bool RPSConfig::GetString(const char* option, std::string& strVal)
{
   if (m_bInitialized)
   {
       if (m_jsonDocument.HasMember(option) && m_jsonDocument[option].IsString())
       {
           strVal = m_jsonDocument[option].GetString();
           return true;
       }
   }

   LOG(ERROR) << "Wrong STRING property requested: " << option;

   return false;
}