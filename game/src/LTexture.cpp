#include "LTexture.h"

#include "sdl/SDL_image.h"
#include "logger/easylogging++.h"

LTexture::LTexture(SDL_Renderer* pRenderer)
    : m_pTexture(nullptr)
    , m_iWidth(0)
    , m_iHeight(0)
    , m_pRenderer(pRenderer)
    , m_pFont(nullptr)
{
}

LTexture::~LTexture()
{
    Free();
}

bool LTexture::LoadFromFile(const std::string& strPath)
{
    // Get rid of preexisting texture
    Free();

    //The final texture
    SDL_Texture* pTexture = nullptr;

    // Load image at specified path
    SDL_Surface* pSurface = IMG_Load(strPath.c_str());
    if (!pSurface)
    {
        LOG(ERROR) << "Can't load image " << strPath << ". Error: " << IMG_GetError();
    }
    else
    {
        // Color key image
        SDL_SetColorKey(pSurface, SDL_TRUE, SDL_MapRGB(pSurface->format, 0, 0xFF, 0xFF));

        // Create texture from surface pixels
        pTexture = SDL_CreateTextureFromSurface(m_pRenderer, pSurface);
        if (!pTexture)
        {
            LOG(ERROR) << "Can't create texture from " << strPath << ". Error: " << SDL_GetError();
        }
        else
        {
            // Get image dimensions
            m_iWidth = pSurface->w;
            m_iHeight = pSurface->h;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(pSurface);
    }

    // Return success
    m_pTexture = pTexture;
    return (m_pTexture != nullptr);
}

bool LTexture::LoadFromRenderedText(const std::string& strFont, int size, const std::string& strTextureText, SDL_Color textColor)
{
	// Get rid of preexisting texture
	Free();

    m_pFont = TTF_OpenFont(strFont.c_str(), size);
    if (!m_pFont)
    {
        LOG(ERROR) << "Can't load font " << strFont << ". Error: " << TTF_GetError();
    }
    else
    {
        // Render text surface
        SDL_Surface* pTextSurface = TTF_RenderText_Solid(m_pFont, strTextureText.c_str(), textColor);
        if (!pTextSurface)
        {
            LOG(ERROR) << "Can't render text surface: " << TTF_GetError();
        }
        else
        {
            // Create texture from surface pixels
            m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, pTextSurface);
            if (!m_pTexture)
            {
                LOG(ERROR) << "Can't create texture from rendered text: " << SDL_GetError();
            }
            else
            {
                // Get image dimensions
                m_iWidth = pTextSurface->w;
                m_iHeight = pTextSurface->h;
            }

            // Get rid of old surface
            SDL_FreeSurface(pTextSurface);
        }
    }
	
	// Return success
	return (m_pTexture != NULL);
}

void LTexture::Free()
{
	// Free texture if it exists
	if (m_pTexture)
	{
		SDL_DestroyTexture(m_pTexture);
		m_pTexture = nullptr;
		m_iWidth = 0;
		m_iHeight = 0;
	}
}

void LTexture::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
	// Modulate texture rgb
	SDL_SetTextureColorMod(m_pTexture, red, green, blue);
}

void LTexture::SetBlendMode(SDL_BlendMode blending)
{
	// Set blending function
	SDL_SetTextureBlendMode(m_pTexture, blending);
}
		
void LTexture::SetAlpha(Uint8 alpha)
{
	// Modulate texture alpha
	SDL_SetTextureAlphaMod(m_pTexture, alpha);
}

void LTexture::Render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	// Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, m_iWidth, m_iHeight };

	// Set clip rendering dimensions
	if (clip != nullptr)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	// Render to screen
	SDL_RenderCopyEx(m_pRenderer, m_pTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::GetWidth() const
{
	return m_iWidth;
}

int LTexture::GetHeight() const
{
	return m_iHeight;
} 