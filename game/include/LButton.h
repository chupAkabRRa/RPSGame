#ifndef _LBUTTON_H_
#define _LBUTTON_H_

#include <string>

#include "IEventHandler.h"
#include "IRenderObject.h"
#include "LTexture.h"

#include "sdl/SDL.h"

class LButton : private LTexture, public IEventHandler, public IRenderObject
{
public:
    LButton(SDL_Renderer* pRenderer, const std::string& strCaption, const std::string& strFont, int iSize, SDL_Color textColor);
    ~LButton();
    bool Initialize();
    
    // Gets image dimensions
    int GetWidth() const;
    int GetHeight() const;

    void SetPos(int x, int y);

    void Render() override;
    bool HandleEvent(SDL_Event* e) override;

private:
    std::string m_strCaption;
    int m_x = 0;
    int m_y = 0;

    std::string m_strOriginalFont;
    int m_iOriginalSize;
    SDL_Color m_Color;
};

#endif // _LBUTTON_H_