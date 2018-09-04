#ifndef _SENDER_RECEIVER_H_
#define _SENDER_RECEIVER_H_

#include <string>
#include "gog/GalaxyApi.h"

class SenderReceiver
{
public:
    SenderReceiver() = default;
    virtual ~SenderReceiver() = default;

    virtual bool ReadData(std::string& strData) const
    {
        uint32_t size = 0;
        if (galaxy::api::Networking()->IsP2PPacketAvailable(&size))
        {
            char data[256] = {0};
            uint32_t sizeOut = 0;
            galaxy::api::GalaxyID outId;

            if (galaxy::api::Networking()->ReadP2PPacket((void*)data, 256, &sizeOut, outId))
            {
                strData = std::string(data);
                return true;
            }
        }
        return false;
    }

    virtual bool SendData(const std::string& strData) const
    {
        return galaxy::api::Networking()->SendP2PPacket(m_enemyId,
            (const void*)strData.c_str(), (uint32_t)strData.size(), galaxy::api::P2PSendType::P2P_SEND_RELIABLE);
    }

protected:
    galaxy::api::GalaxyID m_enemyId;
};

#endif // _SENDER_RECEIVER_H_