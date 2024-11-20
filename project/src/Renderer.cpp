//External includes
#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "Renderer.h"
#include "Maths.h"
#include "Texture.h"
#include "Utils.h"
#include <iostream>
#include<algorithm>

using namespace dae;

Renderer::Renderer(SDL_Window* pWindow) :
	m_pWindow(pWindow)
{
	m_pTex = Texture::LoadFromFile("resources/uv_grid_2.png");

	//Initialize
	SDL_GetWindowSize(pWindow, &m_Width, &m_Height);

	//Create Buffers
	m_pFrontBuffer = SDL_GetWindowSurface(pWindow);
	m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
	m_pBackBufferPixels = (uint32_t*)m_pBackBuffer->pixels;

	m_pDepthBufferPixels = new float[m_Width * m_Height];

	//Initialize Camera
	m_Camera.Initialize(static_cast<float>(m_Width) / m_Height,60.f, { 0.f,0.f,-10.f });
}

Renderer::~Renderer()
{
	delete[] m_pDepthBufferPixels;
}

void Renderer::Update(Timer* pTimer)
{
	m_Camera.Update(pTimer);
	ChangeRenderMode();
}

void Renderer::Render()
{
	//@START
	//Lock BackBuffer
	SDL_LockSurface(m_pBackBuffer);

	//RENDER LOGIC
	//Renderer_W1_Part1();
	//Renderer_W1_Part2();
	//Renderer_W1_Part3();
	//Renderer_W1_Part4();
	//Renderer_W1_Part5();
	//Renderer_W2();
	Renderer_W3();
	

	//@END
	//Update SDL Surface
	SDL_UnlockSurface(m_pBackBuffer);
	SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
	SDL_UpdateWindowSurface(m_pWindow);
}

void dae::Renderer::VertexTransformationFunction(const std::vector<Vertex>& vertices_in, std::vector<Vertex_Out>& vertices_out, Matrix worldMatrix) const
{

	float aspectRatio{ static_cast<float>(m_Width) / m_Height };
	Matrix worldVieuwProjectionMatrix = worldMatrix * m_Camera.viewMatrix * m_Camera.projectionMatrix;
	
		

		//Vector4 position4 = Vector4(meshes_screen[mesh].vertices[idx].position.x, meshes_screen[mesh].vertices[idx].position.y, meshes_screen[mesh].vertices[idx].position.z, 1);
		//meshes_screen[mesh].vertices_out[idx].position = meshes_screen[mesh].vertices[idx].position * worldVieuwProjectionMatrix;

	
	for (int idx{}; idx < vertices_in.size(); ++idx)
	{

		vertices_out.emplace_back();
		//World to NDC
		Matrix temp{};
		temp[0] = { vertices_in[idx].position,1 };

		vertices_out[idx].position = { (temp * worldVieuwProjectionMatrix)[0]};


		//perspective Divide
		vertices_out[idx].position.x = { vertices_out[idx].position.x / vertices_out[idx].position.w };
		vertices_out[idx].position.y = { vertices_out[idx].position.y / vertices_out[idx].position.w };
		vertices_out[idx].position.z = { vertices_out[idx].position.z / vertices_out[idx].position.w };

		//NDC To camera
		vertices_out[idx].position.x = (vertices_out[idx].position.x + 1) / 2 * m_Width;
		vertices_out[idx].position.y = (1 - vertices_out[idx].position.y) / 2 * m_Height;
	}
}

//void dae::Renderer::Renderer_W1_Part1()
//{
//	const std::vector<Vertex> vertices_ndc
//	{
//		{ { 0.f,0.5f,1.f },{} },
//		{{0.5f,-0.5f,1.f},{}},
//		{{ -0.5f,-0.5f,1.f },{} }
//
//	};
//
//	std::vector<Vertex>screenSpaceVertices{};
//	screenSpaceVertices.reserve(vertices_ndc.size());
//	VertexTransformationFunction(vertices_ndc, screenSpaceVertices);
//
//	for (int px{}; px < m_Width; ++px)
//	{
//		for (int py{}; py < m_Height; ++py)
//		{
//			
//
//			/*Vector3 P{ px + 0.5f,py + 0.5f,1.f };
//
//			Vector3 e0{ screenSpaceVertices[1].position - screenSpaceVertices[0].position };
//			Vector3 e1{ screenSpaceVertices[2].position - screenSpaceVertices[1].position };
//			Vector3 e2{ screenSpaceVertices[0].position - screenSpaceVertices[2].position };
//			Vector3 p0{ P - screenSpaceVertices[0].position };
//			Vector3 p1{ P - screenSpaceVertices[1].position };
//			Vector3 p2{ P - screenSpaceVertices[2].position };*/
//			Vector2 P{ px + 0.5f,py + 0.5f };
//
//			Vector2 e0{ screenSpaceVertices[1].position.x - screenSpaceVertices[0].position.x,screenSpaceVertices[1].position.y - screenSpaceVertices[0].position.y };
//			Vector2 p0{ P.x - screenSpaceVertices[0].position.x,P.y - screenSpaceVertices[0].position.y };
//			Vector2 e1{ screenSpaceVertices[2].position.x - screenSpaceVertices[1].position.x,screenSpaceVertices[2].position.y - screenSpaceVertices[1].position.y };
//			Vector2 p1{ P.x - screenSpaceVertices[1].position.x,P.y - screenSpaceVertices[1].position.y };
//			Vector2 e2{ screenSpaceVertices[0].position.x - screenSpaceVertices[2].position.x,screenSpaceVertices[0].position.y - screenSpaceVertices[2].position.y };
//			Vector2 p2{ P.x - screenSpaceVertices[2].position.x,P.y - screenSpaceVertices[2].position.y };
//
//			ColorRGB finalColor{ 0, 0, 0 };
//
//			if (Vector2::Cross(p0, e0) > 0) finalColor = {0,0,0};
//			else if (Vector2::Cross(p1, e1) > 0) finalColor={ 0,0,0 };
//			else if (Vector2::Cross(p2, e2) > 0) finalColor = { 0,0,0 };
//			else finalColor = { 1, 1, 1 };
//
//			//Update Color in Buffer
//			finalColor.MaxToOne();
//
//			m_pBackBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
//				static_cast<uint8_t>(finalColor.r * 255),
//				static_cast<uint8_t>(finalColor.g * 255),
//				static_cast<uint8_t>(finalColor.b * 255));
//		}
//	}
//}
//
//void dae::Renderer::Renderer_W1_Part2()
//{
//	std::vector<Vertex> vertices_world
//	{
//		{{0.f,2.f,0.f}},
//		{{1.f,0.f,0.f}},
//		{{-1.f,0.f,0.f}}
//	};
//
//	std::vector<Vertex>screenSpaceVertices{};
//	screenSpaceVertices.reserve(vertices_world.size());
//	VertexTransformationFunction(vertices_world, screenSpaceVertices);
//
//
//	for (int px{}; px < m_Width; ++px)
//	{
//		for (int py{}; py < m_Height; ++py)
//		{
//			Vector2 P{ px + 0.5f,py + 0.5f };
//
//			Vector2 e0{ screenSpaceVertices[1].position.x - screenSpaceVertices[0].position.x,screenSpaceVertices[1].position.y - screenSpaceVertices[0].position.y };
//			Vector2 p0{ P.x - screenSpaceVertices[0].position.x,P.y - screenSpaceVertices[0].position.y };
//			Vector2 e1{ screenSpaceVertices[2].position.x - screenSpaceVertices[1].position.x,screenSpaceVertices[2].position.y - screenSpaceVertices[1].position.y };
//			Vector2 p1{ P.x - screenSpaceVertices[1].position.x,P.y - screenSpaceVertices[1].position.y };
//			Vector2 e2{ screenSpaceVertices[0].position.x - screenSpaceVertices[2].position.x,screenSpaceVertices[0].position.y - screenSpaceVertices[2].position.y };
//			Vector2 p2{ P.x - screenSpaceVertices[2].position.x,P.y - screenSpaceVertices[2].position.y };
//
//			ColorRGB finalColor{ 0, 0, 0 };
//
//			if (Vector2::Cross(p0, e0) > 0) finalColor = { 0,0,0 };
//			else if (Vector2::Cross(p1, e1) > 0) finalColor = { 0,0,0 };
//			else if (Vector2::Cross(p2, e2) > 0) finalColor = { 0,0,0 };
//			else finalColor = { 1, 1, 1 };
//
//			//Update Color in Buffer
//			finalColor.MaxToOne();
//
//			m_pBackBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
//				static_cast<uint8_t>(finalColor.r * 255),
//				static_cast<uint8_t>(finalColor.g * 255),
//				static_cast<uint8_t>(finalColor.b * 255));
//		}
//	}
//}
//
//void dae::Renderer::Renderer_W1_Part3()
//{
//	std::vector<Vertex> vertices_world
//	{
//		{{0.f,4.f,2.f}, {1.f,0.f,0.f}},
//		{{3.f,-2.f,2.f}, {0.f,1.f,0.f}},
//		{{-3.f,-2.f,2.f}, {0.f,0.f,1.f}}
//	};
//
//	std::vector<Vertex>screenSpaceVertices{};
//	screenSpaceVertices.reserve(vertices_world.size());
//	VertexTransformationFunction(vertices_world, screenSpaceVertices);
//
//	for (int px{}; px < m_Width; ++px)
//	{
//		for (int py{}; py < m_Height; ++py)
//		{
//			Vector2 P{ px + 0.5f,py + 0.5f };
//
//			Vector2 e0{ screenSpaceVertices[1].position.x - screenSpaceVertices[0].position.x,screenSpaceVertices[1].position.y - screenSpaceVertices[0].position.y };
//			Vector2 p0{ P.x - screenSpaceVertices[0].position.x,P.y - screenSpaceVertices[0].position.y };
//			Vector2 e1{ screenSpaceVertices[2].position.x - screenSpaceVertices[1].position.x,screenSpaceVertices[2].position.y - screenSpaceVertices[1].position.y };
//			Vector2 p1{ P.x - screenSpaceVertices[1].position.x,P.y - screenSpaceVertices[1].position.y };
//			Vector2 e2{ screenSpaceVertices[0].position.x - screenSpaceVertices[2].position.x,screenSpaceVertices[0].position.y - screenSpaceVertices[2].position.y };
//			Vector2 p2{ P.x - screenSpaceVertices[2].position.x,P.y - screenSpaceVertices[2].position.y };
//
//			Vector2 v0{ screenSpaceVertices[0].position.x, screenSpaceVertices[0].position.y };
//			Vector2 v1{ screenSpaceVertices[1].position.x, screenSpaceVertices[1].position.y };
//			Vector2 v2{ screenSpaceVertices[2].position.x, screenSpaceVertices[2].position.y };
//
//			float totalArea{ Vector2::Cross(v1 - v0,v2 - v0) };
//
//			float weight0{ abs(Vector2::Cross(P - v1 ,v2 - v1)) / totalArea };
//			float weight1{ abs(Vector2::Cross(P - v2 ,v0 - v2)) / totalArea };
//			float weight2{ abs(Vector2::Cross(P - v0 ,v1 - v0)) / totalArea };
//
//			ColorRGB finalColor{ };
//			if (Vector2::Cross(p0, e0) > 0) finalColor = { 0,0,0 };
//			else if (Vector2::Cross(p1, e1) > 0) finalColor = { 0,0,0 };
//			else if (Vector2::Cross(p2, e2) > 0) finalColor = { 0,0,0 };
//			else finalColor = { vertices_world[0].color * weight0 + vertices_world[1].color * weight1 + vertices_world[2].color * weight2 };
//
//			//Update Color in Buffer
//			finalColor.MaxToOne();
//
//			m_pBackBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
//				static_cast<uint8_t>(finalColor.r * 255),
//				static_cast<uint8_t>(finalColor.g * 255),
//				static_cast<uint8_t>(finalColor.b * 255));
//		}
//	}
//}

//void dae::Renderer::Renderer_W1_Part4()
//{
//	/*std::vector<Mesh> meshes_world
//	{
//		Mesh{
//				{
//					Vertex{{ -3,3,-2 }},
//					Vertex{{ 0,3,-2}},
//					Vertex{{ 3,3,-2}},
//					Vertex{{ -3,0,-2}},
//					Vertex{{0,0,-2 }},
//					Vertex{{ 3,0,-2}},
//					Vertex{{-3,-3,-2 }},
//					Vertex{{0,-3,-2 }},
//					Vertex{{3,-3,-2 }}
//				},
//				{
//					3, 0, 4, 1, 5, 2,
//					2, 6,
//					6, 3, 7, 4, 8, 5
//				},
//			PrimitiveTopology::TriangleStrip
//		}
//	};*/
//
//
//
//
//
//
//
//	std::vector<Vertex>screenSpaceVertices{};
//	screenSpaceVertices.reserve(vertices_world.size());
//	VertexTransformationFunction(vertices_world, screenSpaceVertices);
//
//
//	std::vector<float> depthBuffer{};
//	depthBuffer.reserve(m_Width * m_Height);
//
//	SDL_FillRect(m_pBackBuffer, 0 ,SDL_MapRGB(m_pBackBuffer->format, static_cast < uint8_t>(100), static_cast < uint8_t>(100), static_cast<uint8_t>(100)));
//
//	for (int idx{}; idx < screenSpaceVertices.size(); idx += 3)
//	{
//		for (int px{}; px < m_Width; ++px)
//		{
//			for (int py{}; py < m_Height; ++py)
//			{
//				depthBuffer.emplace_back(FLT_MAX);
//
//
//				Vector2 P{ px + 0.5f,py + 0.5f };
//
//				Vector2 e0{ screenSpaceVertices[idx + 1].position.x - screenSpaceVertices[idx].position.x,screenSpaceVertices[idx + 1].position.y - screenSpaceVertices[idx].position.y };
//				Vector2 p0{ P.x - screenSpaceVertices[idx].position.x,P.y - screenSpaceVertices[idx].position.y };
//				Vector2 e1{ screenSpaceVertices[idx + 2].position.x - screenSpaceVertices[idx + 1].position.x,screenSpaceVertices[idx + 2].position.y - screenSpaceVertices[idx + 1].position.y };
//				Vector2 p1{ P.x - screenSpaceVertices[idx + 1].position.x,P.y - screenSpaceVertices[idx + 1].position.y };
//				Vector2 e2{ screenSpaceVertices[idx].position.x - screenSpaceVertices[idx + 2].position.x,screenSpaceVertices[idx].position.y - screenSpaceVertices[idx + 2].position.y };
//				Vector2 p2{ P.x - screenSpaceVertices[idx + 2].position.x,P.y - screenSpaceVertices[idx + 2].position.y };
//
//				Vector2 v0{ screenSpaceVertices[idx].position.x, screenSpaceVertices[idx].position.y };
//				Vector2 v1{ screenSpaceVertices[idx + 1].position.x, screenSpaceVertices[idx + 1].position.y };
//				Vector2 v2{ screenSpaceVertices[idx + 2].position.x, screenSpaceVertices[idx + 2].position.y };
//
//				float totalArea{ Vector2::Cross(v1 - v0,v2 - v0) };
//
//				float weight0{ abs(Vector2::Cross(P - v1 ,v2 - v1)) / totalArea };
//				float weight1{ abs(Vector2::Cross(P - v2 ,v0 - v2)) / totalArea };
//				float weight2{ abs(Vector2::Cross(P - v0 ,v1 - v0)) / totalArea };
//
//				ColorRGB finalColor{ };
//				if (Vector2::Cross(p0, e0) > 0) continue;
//				else if (Vector2::Cross(p1, e1) > 0) continue;
//				else if (Vector2::Cross(p2, e2) > 0) continue;
//				else finalColor = { vertices_world[idx].color * weight0 + vertices_world[idx + 1].color * weight1 + vertices_world[idx + 2].color * weight2 };
//
//				float depth{ vertices_world[idx].position.z * weight0 + vertices_world[idx + 1].position.z * weight1 + vertices_world[idx + 2].position.z * weight2 };
//				if (depth > depthBuffer[px * m_Height + py]) continue;
//
//				depthBuffer[px * m_Height + py] = depth;
//
//				//Update Color in Buffer
//				finalColor.MaxToOne();
//
//				m_pBackBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
//					static_cast<uint8_t>(finalColor.r * 255),
//					static_cast<uint8_t>(finalColor.g * 255),
//					static_cast<uint8_t>(finalColor.b * 255));
//
//			}
//		}
//	}
//}

//void dae::Renderer::Renderer_W1_Part5()
//{
//	std::vector<Vertex> vertices_world
//	{
//		//Triangle0
//		{{0.f,2.f,0.f}, {1.f,0.f,0.f}},
//		{{1.5f,-1.f,0.f}, {1.f,0.f,0.f}},
//		{{-1.5f,-1.f,0.f}, {1.f,0.f,0.f}},
//
//		//Triangle1
//		{{0.f,4.f,2.f}, {1.f,0.f,0.f}},
//		{{3.f,-2.f,2.f}, {0.f,1.f,0.f}},
//		{{-3.f,-2.f,2.f}, {0.f,0.f,1.f}}
//	};
//
//	std::vector<Vertex>screenSpaceVertices{};
//	screenSpaceVertices.reserve(vertices_world.size());
//	VertexTransformationFunction(vertices_world, screenSpaceVertices);
//
//	std::vector<float> depthBuffer{};
//	depthBuffer.reserve(m_Width * m_Height);
//
//	SDL_FillRect(m_pBackBuffer, 0, SDL_MapRGB(m_pBackBuffer->format, static_cast <uint8_t>(100), static_cast <uint8_t>(100), static_cast<uint8_t>(100)));
//
//	for (int idx{}; idx < screenSpaceVertices.size(); idx += 3)
//	{
//		float maxX{ std::max(screenSpaceVertices[idx].position.x,screenSpaceVertices[idx + 1].position.x) };
//		maxX = { std::max(maxX,screenSpaceVertices[idx + 2].position.x) };
//		if (maxX < 0) maxX = 0;
//		if (maxX > m_Width) maxX = m_Width;
//		//Clamp(maxX, 1.f, static_cast<float>(m_Width-1));
//		float minX{ std::min(screenSpaceVertices[idx].position.x,screenSpaceVertices[idx + 1].position.x) };
//		minX = { std::min(minX,screenSpaceVertices[idx + 2].position.x) };
//		if (minX < 0) minX = 0;
//		if (minX > m_Width) minX = m_Width;
//		//Clamp(minX, 1.f, static_cast<float>(m_Width-1));
//
//
//		float maxY{ std::max(screenSpaceVertices[idx].position.y,screenSpaceVertices[idx + 1].position.y) };
//		maxY = { std::max(maxY,screenSpaceVertices[idx + 2].position.y) };
//		if (maxY < 0) maxY = 0;
//		if (maxY > m_Height) maxY = m_Height;
//		//Clamp(maxY, 1.f, static_cast<float>(m_Height-1));
//		float minY{ std::min(screenSpaceVertices[idx].position.y,screenSpaceVertices[idx + 1].position.y) };
//		minY = { std::min(minY,screenSpaceVertices[idx + 2].position.y) };
//		if (minY < 0) minY = 0;
//		if (minY > m_Height) minY = m_Height;
//		//Clamp(minY, 1.f,  static_cast<float>(m_Height-1));
//
//		for (int px{}; px < m_Width; ++px)
//		{
//			for (int py{}; py < m_Height; ++py)
//			{
//				depthBuffer.emplace_back(FLT_MAX);
//			}
//		}
//
//		for (int px{ static_cast<int>(minX) }; px < static_cast<int>(maxX); ++px)
//		{
//			for (int py{ static_cast<int>(minY) }; py < static_cast<int>(maxY); ++py)
//			{
//				
//
//
//				Vector2 P{ px + 0.5f,py + 0.5f };
//
//				Vector2 e0{ screenSpaceVertices[idx + 1].position.x - screenSpaceVertices[idx].position.x,screenSpaceVertices[idx + 1].position.y - screenSpaceVertices[idx].position.y };
//				Vector2 p0{ P.x - screenSpaceVertices[idx].position.x,P.y - screenSpaceVertices[idx].position.y };
//				Vector2 e1{ screenSpaceVertices[idx + 2].position.x - screenSpaceVertices[idx + 1].position.x,screenSpaceVertices[idx + 2].position.y - screenSpaceVertices[idx + 1].position.y };
//				Vector2 p1{ P.x - screenSpaceVertices[idx + 1].position.x,P.y - screenSpaceVertices[idx + 1].position.y };
//				Vector2 e2{ screenSpaceVertices[idx].position.x - screenSpaceVertices[idx + 2].position.x,screenSpaceVertices[idx].position.y - screenSpaceVertices[idx + 2].position.y };
//				Vector2 p2{ P.x - screenSpaceVertices[idx + 2].position.x,P.y - screenSpaceVertices[idx + 2].position.y };
//
//				Vector2 v0{ screenSpaceVertices[idx].position.x, screenSpaceVertices[idx].position.y };
//				Vector2 v1{ screenSpaceVertices[idx + 1].position.x, screenSpaceVertices[idx + 1].position.y };
//				Vector2 v2{ screenSpaceVertices[idx + 2].position.x, screenSpaceVertices[idx + 2].position.y };
//
//				float totalArea{ Vector2::Cross(v1 - v0,v2 - v0) };
//
//				float weight0{ abs(Vector2::Cross(P - v1 ,v2 - v1)) / totalArea };
//				float weight1{ abs(Vector2::Cross(P - v2 ,v0 - v2)) / totalArea };
//				float weight2{ abs(Vector2::Cross(P - v0 ,v1 - v0)) / totalArea };
//
//				ColorRGB finalColor{ };
//				if (Vector2::Cross(p0, e0) > 0) continue;
//				else if (Vector2::Cross(p1, e1) > 0) continue;
//				else if (Vector2::Cross(p2, e2) > 0) continue;
//				else finalColor = { vertices_world[idx].color * weight0 + vertices_world[idx + 1].color * weight1 + vertices_world[idx + 2].color * weight2 };
//
//				float depth{ vertices_world[idx].position.z * weight0 + vertices_world[idx + 1].position.z * weight1 + vertices_world[idx + 2].position.z * weight2 };
//				if (depth > depthBuffer[px + (py * m_Width)]) continue;
//
//				depthBuffer[px + (py * m_Width)] = depth;
//
//				//Update Color in Buffer
//				finalColor.MaxToOne();
//
//				m_pBackBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
//					static_cast<uint8_t>(finalColor.r * 255),
//					static_cast<uint8_t>(finalColor.g * 255),
//					static_cast<uint8_t>(finalColor.b * 255));
//
//			}
//		}
//	}
//}
//
//void dae::Renderer::Renderer_W2()
//{
//	
//	/*std::vector<Mesh> meshes_world
//	{
//		Mesh{
//				{
//					Vertex{{ -3,3,-2 }},
//					Vertex{{ 0,3,-2}},
//					Vertex{{ 3,3,-2}},
//					Vertex{{ -3,0,-2}},
//					Vertex{{0,0,-2 }},
//					Vertex{{ 3,0,-2}},
//					Vertex{{-3,-3,-2 }},
//					Vertex{{0,-3,-2 }},
//					Vertex{{3,-3,-2 }}
//				},
//				{
//					3, 0, 1,
//					1, 4, 3,
//					4, 1, 2,
//					2, 5, 4,
//					6, 3, 4,
//					4, 7, 6,
//					7, 4, 5,
//					5, 8, 7	
//				},
//			PrimitiveTopology::TriangleList
//		}
//	};*/
//
//	std::vector<Mesh> meshes_world
//	{
//		Mesh{
//			{
//				Vertex{{ -3,3,-2 },{1.f,1.f,1.f},{0,0}},
//				Vertex{{ 0,3,-2},{1.f,1.f,1.f},{0.5f,0}},
//				Vertex{{ 3,3,-2},{1.f,1.f,1.f},{1,0}},
//				Vertex{{ -3,0,-2},{1.f,1.f,1.f},{0,0.5}},
//				Vertex{{0,0,-2 },{1.f,1.f,1.f},{0.5f,0.5f}},
//				Vertex{{ 3,0,-2},{1.f,1.f,1.f},{1,0.5f}},
//				Vertex{{-3,-3,-2 },{1.f,1.f,1.f},{0,1}},
//				Vertex{{0,-3,-2 },{1.f,1.f,1.f},{0.5f,1}},
//				Vertex{{3,-3,-2 },{1.f,1.f,1.f},{1,1}}
//			},
//			{
//				3, 0, 4, 1, 5, 2,
//				2, 6,
//				6, 3, 7, 4, 8, 5
//			},
//		PrimitiveTopology::TriangleStrip
//		}
//	};
//
//	std::vector<Mesh> meshes_screen{ meshes_world };
//	for (int i{}; i < meshes_world.size(); ++i)
//	{
//		VertexTransformationFunction(meshes_world[i].vertices, meshes_screen[i].vertices);
//	}
//
//	std::vector<float> depthBuffer{};
//	depthBuffer.reserve(m_Width * m_Height);
//
//	SDL_FillRect(m_pBackBuffer, 0, SDL_MapRGB(m_pBackBuffer->format, static_cast <uint8_t>(100), static_cast <uint8_t>(100), static_cast<uint8_t>(100)));
//
//	for (int px{}; px < m_Width; ++px)
//	{
//		for (int py{}; py < m_Height; ++py)
//		{
//			depthBuffer.emplace_back(FLT_MAX);
//		}
//	}
//	for (int mesh{}; mesh < meshes_screen.size(); ++mesh)
//	{
//		int step{ 3 };
//		for (int idx{}; idx < meshes_screen[mesh].indices.size(); idx += step)
//		{
//			uint32_t index1 = meshes_screen[mesh].indices[idx];
//			uint32_t index2 = meshes_screen[mesh].indices[idx + 1];
//			uint32_t index3 = meshes_screen[mesh].indices[idx + 2];
//
//			if (meshes_screen[mesh].primitiveTopology == PrimitiveTopology::TriangleStrip)
//			{
//				step = 1;
//				if (idx == (meshes_screen[mesh].indices.size() - 3)) step = 3;
//				if (idx % 2)
//				{
//					std::swap(index2, index3);
//				}
//			}
//
//			float maxX{ std::max(meshes_screen[mesh].vertices[index1].position.x,meshes_screen[mesh].vertices[index2].position.x) };
//			maxX = { std::max(maxX,meshes_screen[mesh].vertices[index3].position.x) };
//			/*if (maxX < 0.f) maxX = 0.f;
//			if (maxX > m_Width) maxX = m_Width;*/
//			maxX = std::clamp(maxX, 0.f, static_cast<float>(m_Width));
//			float minX{ std::min(meshes_screen[mesh].vertices[index1].position.x,meshes_screen[mesh].vertices[index2].position.x) };
//			minX = { std::min(minX,meshes_screen[mesh].vertices[index3].position.x) };
//			/*if (minX < 0.f) minX = -0.1f;
//			if (minX > m_Width) minX = m_Width;*/
//			minX = std::clamp(minX, 0.f, static_cast<float>(m_Width));
//
//
//			float maxY{ std::max(meshes_screen[mesh].vertices[index1].position.y,meshes_screen[mesh].vertices[index2].position.y) };
//			maxY = { std::max(maxY,meshes_screen[mesh].vertices[index3].position.y) };
//			/*if (maxY < 0.f) maxY = 0.f;
//			if (maxY > m_Height) maxY = m_Height;*/
//			maxY = std::clamp(maxY, 0.f, static_cast<float>(m_Height));
//			float minY{ std::min(meshes_screen[mesh].vertices[index1].position.y,meshes_screen[mesh].vertices[index2].position.y) };
//			minY = { std::min(minY,meshes_screen[mesh].vertices[index3].position.y) };
//			/*if (minY < 0.f) minY = 0.f;
//			if (minY > m_Height) minY = m_Height;*/
//			minY = std::clamp(minY, 0.f, static_cast<float>(m_Height));
//
//			float maxZ{ std::max(meshes_screen[mesh].vertices[index1].position.z,meshes_screen[mesh].vertices[index2].position.z) };
//			maxZ = { std::max(maxZ,meshes_screen[mesh].vertices[index3].position.z) };
//			if (maxZ <= 0)
//			{
//				continue;
//			}
//
//
//
//			for (int px{ static_cast<int>(minX) }; px < maxX; ++px)
//			{
//				for (int py{ static_cast<int>(minY) }; py < maxY; ++py)
//				{
//					Vector2 P{ px + 0.5f,py + 0.5f };
//
//					Vector2 e0{ meshes_screen[mesh].vertices[index2].position.x - meshes_screen[mesh].vertices[index1].position.x,meshes_screen[mesh].vertices[index2].position.y - meshes_screen[mesh].vertices[index1].position.y };
//					Vector2 p0{ P.x - meshes_screen[mesh].vertices[index1].position.x,P.y - meshes_screen[mesh].vertices[index1].position.y };
//					Vector2 e1{ meshes_screen[mesh].vertices[index3].position.x - meshes_screen[mesh].vertices[index2].position.x,meshes_screen[mesh].vertices[index3].position.y - meshes_screen[mesh].vertices[index2].position.y };
//					Vector2 p1{ P.x - meshes_screen[mesh].vertices[index2].position.x,P.y - meshes_screen[mesh].vertices[index2].position.y };
//					Vector2 e2{ meshes_screen[mesh].vertices[index1].position.x - meshes_screen[mesh].vertices[index3].position.x, meshes_screen[mesh].vertices[index1].position.y - meshes_screen[mesh].vertices[index3].position.y };
//					Vector2 p2{ P.x - meshes_screen[mesh].vertices[index3].position.x,P.y - meshes_screen[mesh].vertices[index3].position.y };
//
//					Vector2 v0{ meshes_screen[mesh].vertices[index1].position.x,  meshes_screen[mesh].vertices[index1].position.y };
//					Vector2 v1{ meshes_screen[mesh].vertices[index2].position.x,  meshes_screen[mesh].vertices[index2].position.y };
//					Vector2 v2{ meshes_screen[mesh].vertices[index3].position.x,  meshes_screen[mesh].vertices[index3].position.y };
//
//					float totalArea{ Vector2::Cross(v1 - v0,v2 - v0) };
//
//					float weight0{ abs(Vector2::Cross(P - v1 ,v2 - v1)) / totalArea };
//					float weight1{ abs(Vector2::Cross(P - v2 ,v0 - v2)) / totalArea };
//					float weight2{ abs(Vector2::Cross(P - v0 ,v1 - v0)) / totalArea };
//
//					float ZInterpolated{ 1 / ((1 / meshes_screen[mesh].vertices[index1].position.z) * weight0 + (1 / meshes_screen[mesh].vertices[index2].position.z) * weight1 + (1 / meshes_screen[mesh].vertices[index3].position.z) * weight2) };
//
//					depthBuffer[px + (py * m_Width)] = ZInterpolated;
//
//					ColorRGB finalColor{ };
//					if (Vector2::Cross(p0, e0) > 0) continue;
//					else if (Vector2::Cross(p1, e1) > 0) continue;
//					else if (Vector2::Cross(p2, e2) > 0) continue;
//
//					//else finalColor = { meshes_world[mesh].vertices[index1].color * weight0 + meshes_world[mesh].vertices[index2].color * weight1 + meshes_world[mesh].vertices[index3].color * weight2 };
//					Vector2 uv{ (meshes_world[mesh].vertices[index1].uv / meshes_screen[mesh].vertices[index1].position.z *
//						weight0 + meshes_world[mesh].vertices[index2].uv / meshes_screen[mesh].vertices[index2].position.z *
//						weight1 + meshes_world[mesh].vertices[index3].uv / meshes_screen[mesh].vertices[index3].position.z * weight2) * ZInterpolated };
//					finalColor = m_pTex->Sample(uv);
//
//
//					//float depth{ (meshes_world[mesh].vertices[index1].position.z * weight0 + meshes_world[mesh].vertices[index2].position.z * weight1 + meshes_world[mesh].vertices[index3].position.z * weight2)* ZInterpolated };
//					float depth{ ZInterpolated };
//					if (depth > depthBuffer[px + (py * m_Width)]) continue;
//
//					depthBuffer[px + (py * m_Width)] = depth;
//
//					//Update Color in Buffer
//					finalColor.MaxToOne();
//
//					m_pBackBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
//						static_cast<uint8_t>(finalColor.r * 255),
//						static_cast<uint8_t>(finalColor.g * 255),
//						static_cast<uint8_t>(finalColor.b * 255));
//				}
//			}
//		}
//	}
//}

void dae::Renderer::Renderer_W3()
{
	std::vector<Mesh> meshes_world
	{
		Mesh{
			{
				Vertex{{ -3,3,-2 },{1.f,1.f,1.f},{0,0}},
				Vertex{{ 0,3,-2},{1.f,1.f,1.f},{0.5f,0}},
				Vertex{{ 3,3,-2},{1.f,1.f,1.f},{1,0}},
				Vertex{{ -3,0,-2},{1.f,1.f,1.f},{0,0.5}},
				Vertex{{0,0,-2 },{1.f,1.f,1.f},{0.5f,0.5f}},
				Vertex{{ 3,0,-2},{1.f,1.f,1.f},{1,0.5f}},
				Vertex{{-3,-3,-2 },{1.f,1.f,1.f},{0,1}},
				Vertex{{0,-3,-2 },{1.f,1.f,1.f},{0.5f,1}},
				Vertex{{3,-3,-2 },{1.f,1.f,1.f},{1,1}}
			},
			{
				3, 0, 4, 1, 5, 2,
				2, 6,
				6, 3, 7, 4, 8, 5
			},
		PrimitiveTopology::TriangleStrip
		}
	};

	//std::vector<Mesh> meshes_screen{ meshes_world };
	for (int i{}; i < meshes_world.size(); ++i)
	{
		VertexTransformationFunction(meshes_world[i].vertices, meshes_world[i].vertices_out , meshes_world[i].GetWorldMat());
	}

	/*std::vector<float> depthBuffer{};
	depthBuffer.reserve(m_Width * m_Height);*/

	SDL_FillRect(m_pBackBuffer, 0, SDL_MapRGB(m_pBackBuffer->format, static_cast <uint8_t>(100), static_cast <uint8_t>(100), static_cast<uint8_t>(100)));

	for (int px{}; px < m_Width; ++px)
	{
		for (int py{}; py < m_Height; ++py)
		{
			m_pDepthBufferPixels[px + (py * m_Width)] = FLT_MAX;
		}
	}
	for (int mesh{}; mesh < meshes_world.size(); ++mesh)
	{
		int step{ 3 };
		for (int idx{}; idx < meshes_world[mesh].indices.size(); idx += step)
		{
			


			uint32_t index1 = meshes_world[mesh].indices[idx];
			uint32_t index2 = meshes_world[mesh].indices[idx + 1];
			uint32_t index3 = meshes_world[mesh].indices[idx + 2];

			if (meshes_world[mesh].primitiveTopology == PrimitiveTopology::TriangleStrip)
			{
				step = 1;
				if (idx == (meshes_world[mesh].indices.size() - 3)) step = 3;
				if (idx % 2)
				{
					std::swap(index2, index3);
				}
			}

			float maxX{ std::max(meshes_world[mesh].vertices_out[index1].position.x,meshes_world[mesh].vertices_out[index2].position.x) };
			maxX = { std::max(maxX,meshes_world[mesh].vertices_out[index3].position.x) };
			/*if (maxX < 0.f) maxX = 0.f;
			if (maxX > m_Width) maxX = m_Width;*/
			maxX = std::clamp(maxX, 0.f, static_cast<float>(m_Width));
			float minX{ std::min(meshes_world[mesh].vertices_out[index1].position.x,meshes_world[mesh].vertices_out[index2].position.x) };
			minX = { std::min(minX,meshes_world[mesh].vertices_out[index3].position.x) };
			/*if (minX < 0.f) minX = -0.1f;
			if (minX > m_Width) minX = m_Width;*/
			minX = std::clamp(minX, 0.f, static_cast<float>(m_Width));


			float maxY{ std::max(meshes_world[mesh].vertices_out[index1].position.y,meshes_world[mesh].vertices_out[index2].position.y) };
			maxY = { std::max(maxY,meshes_world[mesh].vertices_out[index3].position.y) };
			/*if (maxY < 0.f) maxY = 0.f;
			if (maxY > m_Height) maxY = m_Height;*/
			maxY = std::clamp(maxY, 0.f, static_cast<float>(m_Height));
			float minY{ std::min(meshes_world[mesh].vertices_out[index1].position.y,meshes_world[mesh].vertices_out[index2].position.y) };
			minY = { std::min(minY,meshes_world[mesh].vertices_out[index3].position.y) };
			/*if (minY < 0.f) minY = 0.f;
			if (minY > m_Height) minY = m_Height;*/
			minY = std::clamp(minY, 0.f, static_cast<float>(m_Height));

			/*float maxZ{ std::max(meshes_screen[mesh].vertices_out[index1].position.w,meshes_screen[mesh].vertices_out[index2].position.w) };
			maxZ = { std::max(maxZ,meshes_screen[mesh].vertices_out[index3].position.w) };
			if (maxZ <= 0)
			{
				continue;
			}*/



			for (int px{ static_cast<int>(minX) }; px < maxX; ++px)
			{
				for (int py{ static_cast<int>(minY) }; py < maxY; ++py)
				{
					Vector2 P{ px + 0.5f,py + 0.5f };

					Vector2 e0{ meshes_world[mesh].vertices_out[index2].position.x - meshes_world[mesh].vertices_out[index1].position.x,meshes_world[mesh].vertices_out[index2].position.y - meshes_world[mesh].vertices_out[index1].position.y };
					Vector2 p0{ P.x - meshes_world[mesh].vertices_out[index1].position.x,P.y - meshes_world[mesh].vertices_out[index1].position.y };
					Vector2 e1{ meshes_world[mesh].vertices_out[index3].position.x - meshes_world[mesh].vertices_out[index2].position.x,meshes_world[mesh].vertices_out[index3].position.y - meshes_world[mesh].vertices_out[index2].position.y };
					Vector2 p1{ P.x - meshes_world[mesh].vertices_out[index2].position.x,P.y - meshes_world[mesh].vertices_out[index2].position.y };
					Vector2 e2{ meshes_world[mesh].vertices_out[index1].position.x - meshes_world[mesh].vertices_out[index3].position.x, meshes_world[mesh].vertices_out[index1].position.y - meshes_world[mesh].vertices_out[index3].position.y };
					Vector2 p2{ P.x - meshes_world[mesh].vertices_out[index3].position.x,P.y - meshes_world[mesh].vertices_out[index3].position.y };

					Vector2 v0{ meshes_world[mesh].vertices_out[index1].position.x,  meshes_world[mesh].vertices_out[index1].position.y };
					Vector2 v1{ meshes_world[mesh].vertices_out[index2].position.x,  meshes_world[mesh].vertices_out[index2].position.y };
					Vector2 v2{ meshes_world[mesh].vertices_out[index3].position.x,  meshes_world[mesh].vertices_out[index3].position.y };

					float totalArea{ Vector2::Cross(v1 - v0,v2 - v0) };

					float weight0{ abs(Vector2::Cross(P - v1 ,v2 - v1)) / totalArea };
					float weight1{ abs(Vector2::Cross(P - v2 ,v0 - v2)) / totalArea };
					float weight2{ abs(Vector2::Cross(P - v0 ,v1 - v0)) / totalArea };

					float totalWeight{weight0+ weight1+weight2};

					float WInterpolated{ 1 / ((1 / meshes_world[mesh].vertices_out[index1].position.w) * weight0 + (1 / meshes_world[mesh].vertices_out[index2].position.w) * weight1 + (1 / meshes_world[mesh].vertices_out[index3].position.w) * weight2) };

					const float ZInterpolated{1.f / 
						(
						  (1 / meshes_world[mesh].vertices_out[index1].position.z * weight0 )
						+ (1 / meshes_world[mesh].vertices_out[index2].position.z * weight1 )
						+ (1 / meshes_world[mesh].vertices_out[index3].position.z * weight2 )
						) };

					if ((ZInterpolated < 0 || ZInterpolated > 1))
					{
						continue;
					}
					if (m_pDepthBufferPixels[px + (py * m_Width)] < ZInterpolated)
					{
						std::cout << "Wrong depthBuffer" << px + (py * m_Width) << "\n";
						continue;
					}

					m_pDepthBufferPixels[px + (py * m_Width)] = ZInterpolated;

					ColorRGB finalColor{ };
					if (Vector2::Cross(p0, e0) > 0) continue;
					else if (Vector2::Cross(p1, e1) > 0) continue;
					else if (Vector2::Cross(p2, e2) > 0) continue;

					//else finalColor = { meshes_world[mesh].vertices[index1].color * weight0 + meshes_world[mesh].vertices[index2].color * weight1 + meshes_world[mesh].vertices[index3].color * weight2 };
					if (UsingFinalColor == true)
					{
						Vector2 uv{ (meshes_world[mesh].vertices[index1].uv / meshes_world[mesh].vertices_out[index1].position.w *
							weight0 + meshes_world[mesh].vertices[index2].uv / meshes_world[mesh].vertices_out[index2].position.w *
							weight1 + meshes_world[mesh].vertices[index3].uv / meshes_world[mesh].vertices_out[index3].position.w * weight2) * WInterpolated };
						finalColor = m_pTex->Sample(uv);
					}
					else
					{
						float depthValue = m_pDepthBufferPixels[px + (py * m_Width)];
						depthValue = Remap(depthValue, 0.985f, 1.f);
						finalColor = ColorRGB(depthValue, depthValue, depthValue);
					}


					//float depth{ (meshes_world[mesh].vertices[index1].position.z * weight0 + meshes_world[mesh].vertices[index2].position.z * weight1 + meshes_world[mesh].vertices[index3].position.z * weight2)* ZInterpolated };
					/*float depth{ ZInterpolated };
					if (depth > depthBuffer[px + (py * m_Width)]) continue;

					depthBuffer[px + (py * m_Width)] = depth;*/

					//Update Color in Buffer
					finalColor.MaxToOne();

					m_pBackBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
						static_cast<uint8_t>(finalColor.r * 255),
						static_cast<uint8_t>(finalColor.g * 255),
						static_cast<uint8_t>(finalColor.b * 255));
				}
			}
		}
	}

	
}

void dae::Renderer::ChangeRenderMode()
{
	const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);

	if (wasF4Pressed && pKeyboardState[SDL_SCANCODE_F4] == false)
	{
		UsingFinalColor = !UsingFinalColor;
	}
	if (pKeyboardState[SDL_SCANCODE_F4])
	{
		wasF4Pressed = true;
	}
	if (!pKeyboardState[SDL_SCANCODE_F4])
	{
		wasF4Pressed = false;
	}

}

float dae::Renderer::Remap(float value, float outMin, float outMax)
{
	// Ensure the input is within the expected range
	if (value < 0.0f || value > 1.0f) {
		std::cerr << "Error: Input value must be between 0 and 1.\n";
		return outMin; // Return the start of the output range by default
	}

	// Perform the remap calculation
	return outMin + value * (outMax - outMin);
}

bool Renderer::SaveBufferToImage() const
{
	return SDL_SaveBMP(m_pBackBuffer, "Rasterizer_ColorBuffer.bmp");
}


