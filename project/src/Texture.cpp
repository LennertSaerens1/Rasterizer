	#include "Texture.h"
#include "Vector2.h"
#include <SDL_image.h>
#include <algorithm>

namespace dae
{
	Texture::Texture(SDL_Surface* pSurface) :
		m_pSurface{ pSurface },
		m_pSurfacePixels{ (uint32_t*)pSurface->pixels }
	{
	}

	Texture::~Texture()
	{
		if (m_pSurface)
		{
			SDL_FreeSurface(m_pSurface);
			m_pSurface = nullptr;
		}
	}

	Texture* Texture::LoadFromFile(const std::string& path)
	{
		//TODO
		//Load SDL_Surface using IMG_LOAD
		//Create & Return a new Texture Object (using SDL_Surface)
		SDL_Surface* surface{};

		surface = IMG_Load(path.c_str());
		
		return { new Texture {surface} };
	}

	ColorRGB Texture::Sample(const Vector2& uv) const
	{
		//TODO
		//Sample the correct texel for the given uv

		int uvx{ static_cast<int>(uv.x * m_pSurface->w)};
		int uvy{ static_cast<int>(uv.y * m_pSurface->h)};

		uvx = std::min(std::max(uvx, 0), m_pSurface->w - 1);
		uvy = std::min(std::max(uvy, 0), m_pSurface->h - 1);
		
		uint32_t pixel = m_pSurfacePixels[uvx + uvy*m_pSurface->w];

		uint8_t r;
		uint8_t g;
		uint8_t b;
		SDL_GetRGB(pixel, m_pSurface->format, &r, &g, &b);

		return ColorRGB{ r / 255.f,g / 255.f,b / 255.f };
	}
}