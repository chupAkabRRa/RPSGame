#ifndef _ISCENE_H_
#define _ISCENE_H_

#include "IEventHandler.h"
#include "IRenderObject.h"

class IScene: public IRenderObject, public IEventHandler
{
public:
    virtual ~IScene() = default;
    virtual bool Initialize() = 0;
    virtual void Render() = 0;
    virtual bool HandleEvent(SDL_Event* e) = 0;
};

#endif // _ISCENE_H_