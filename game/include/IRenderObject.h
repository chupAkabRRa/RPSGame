#ifndef _IRENDER_OBJECT_H_
#define _IRENDER_OBJECT_H_

class IRenderObject
{
public:
    virtual ~IRenderObject() = default;
    virtual bool Initialize() = 0;
    virtual bool Render() = 0;
};

#endif // _IRENDER_OBJECT_H_