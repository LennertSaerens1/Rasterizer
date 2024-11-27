#pragma once

#include <cstdint>
#include <vector>

#include "Camera.h"
#include "DataTypes.h"

struct SDL_Window;
struct SDL_Surface;

namespace dae
{
	class Texture;
	struct Mesh;
	struct Vertex;
	class Timer;
	class Scene;

	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Update(Timer* pTimer);
		void Render();

		bool SaveBufferToImage() const;

		void VertexTransformationFunction(const std::vector<Vertex>& vertices_in, std::vector<Vertex_Out>& vertices_out, Matrix worldMatrix) const;

		void PixelShading(const Vertex_Out& v, ColorRGB& finalColor, float depthValue, Texture* tex);

		/*void Renderer_W1_Part1();
		void Renderer_W1_Part2();
		void Renderer_W1_Part3();
		void Renderer_W1_Part4();
		void Renderer_W1_Part5();

		void Renderer_W2();*/

		void Renderer_W3();
		/*void Renderer_TucTuc();*/

		void ChangeRenderMode();

		float Remap(float value, float outMin, float outMax);

		enum class RenderMode
		{
			FinalColor,
			Depth,
			BRDF,
			Phong,
			observedArea
		};

	private:
		SDL_Window* m_pWindow{};

		SDL_Surface* m_pFrontBuffer{ nullptr };
		SDL_Surface* m_pBackBuffer{ nullptr };
		uint32_t* m_pBackBufferPixels{};

		float* m_pDepthBufferPixels{};

		Camera m_Camera{};

		int m_Width{};
		int m_Height{};

		std::vector<Texture*> m_pTex;
		std::vector<Texture*> m_pNormalMap;
		std::vector<Texture*> m_pGlossMap;
		std::vector<Texture*> m_pSpecularMap;


		RenderMode m_RenderMode{ RenderMode::FinalColor };

		bool wasF4Pressed{};

		std::vector<Mesh> meshes_world;
	};
		
}
