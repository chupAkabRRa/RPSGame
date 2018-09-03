#ifndef _IRENDER_OBJECT_H_
#define _IRENDER_OBJECT_H_

#include "sdl/SDL.h"

class IRenderObject
{
public:
    virtual ~IRenderObject() = default;
    virtual bool Initialize() = 0;
    virtual void Render() = 0;
};

#endif // _IRENDER_OBJECT_H_