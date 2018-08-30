#ifndef _LTEXTURE_H_
#define _LTEXTURE_H_

#include <string>
#include "sdl/SDL.h"
#include "sdl/SDL_ttf.h"

// Texture wrapper class
class LTexture
{
public:
    LTexture(SDL_Renderer* pRenderer);
    ~LTexture();

    bool LoadFromFile(const std::string& strPath);
    // Creates image from font string
    bool LoadFromRenderedText(const std::string& strFont, int size, const std::string& strTextureText, SDL_Color textColor);
    // Set color modulation
    void SetColor(Uint8 red, Uint8 green, Uint8 blue);
    // Set blending
    void SetBlendMode(SDL_BlendMode blending);
    // Set alpha modulation
    void SetAlpha(Uint8 alpha);
    
    // Renders texture at given point
    void Render(int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE );

    // Gets image dimensions
    int GetWidth() const;
    int GetHeight() const;

private:
    // The actual hardware texture
    SDL_Texture* m_pTexture;

    // Image dimensions
    int m_iWidth;
    int m_iHeight;

    SDL_Renderer* m_pRenderer;
    TTF_Font* m_pFont;

    void Free();
}; 

#endif // _LTEXTURE_H_