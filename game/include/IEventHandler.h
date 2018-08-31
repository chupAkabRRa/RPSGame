#ifndef _IEVENT_HANDLER_H_
#define _IEVENT_HANDLER_H_

#include "sdl/SDL.h"

class IEventHandler
{
public:
    virtual ~IEventHandler() = default;
    virtual bool HandleEvent(SDL_Event* e) = 0;
};

#endif // _IEVENT_HANDLER_H_