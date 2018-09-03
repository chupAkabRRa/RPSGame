#include "LButton.h"

#include "logger/easylogging++.h"

LButton::LButton(SDL_Renderer* pRenderer, const std::string& strCaption, const std::string& strFont, int iSize, SDL_Color textColor, bool bInteractive)
    : LTexture(pRenderer)
    , m_strCaption(strCaption)
    , m_strOriginalFont(strFont)
    , m_iOriginalSize(iSize)
    , m_Color(textColor)
    , m_bInteractive(bInteractive)
{
}

LButton::~LButton()
{

}

bool LButton::Initialize()
{
    return LTexture::LoadFromRenderedText(m_strOriginalFont, m_iOriginalSize, m_strCaption, m_Color);
}

int LButton::GetWidth() const
{
    return LTexture::GetWidth();
}

int LButton::GetHeight() const
{
    return LTexture::GetHeight();
}

bool LButton::UpdateCaption(const std::string& strNewCaption)
{
    m_strCaption = strNewCaption;
    return LTexture::LoadFromRenderedText(m_strOriginalFont, m_iOriginalSize, m_strCaption, m_Color);
}

void LButton::Render()
{
    LTexture::SetColor(m_Color.r, m_Color.g, m_Color.b);
    LTexture::Render(m_x, m_y);
}

bool LButton::HandleEvent(SDL_Event* e)
{
    if (m_bInteractive)
    {
        // If mouse event happened
        if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
        {
            // Get mouse position
            int x, y;
            SDL_GetMouseState(&x, &y);

            bool bInside = true;
            // Mouse is left of the button
            if (x < m_x)
            {
                bInside = false;
            }
            // Mouse is right of the button
            else if ( x > m_x + GetWidth())
            {
                bInside = false;
            }
            // Mouse above the button
            else if (y < m_y)
            {
                bInside = false;
            }
            // Mouse below the button
            else if (y > m_y + GetHeight())
            {
                bInside = false;
            }

            if (bInside)
            {
                m_Color = {255, 0, 0};
                
                if (e->type == SDL_MOUSEBUTTONDOWN)
                {
                    m_bIsMouseBtnDown = true;
                }
                else if (e->type == SDL_MOUSEBUTTONUP)
                {
                    if (m_bIsMouseBtnDown)
                    {
                        m_bIsMouseBtnDown = false;
                        m_bIsClicked = true;
                    }
                }
            }
            else
            {
                m_Color = {255, 255, 255};

                if (e->type == SDL_MOUSEBUTTONUP && m_bIsMouseBtnDown)
                {
                    m_bIsMouseBtnDown = false;
                }
            }
        }
    }

    return true;
}