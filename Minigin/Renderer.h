#pragma once
#include <SDL.h>
#include "Singleton.h"
#include<vector>

namespace dae
{

	struct Rectf
	{
		float x{};
		float y{};
		float width{};
		float height{};

	};

	class Texture2D;
	
	class Renderer final : public Singleton<Renderer>    //this one is to render 
	{
		SDL_Renderer * m_renderer{};
		SDL_Window * m_window{};
		SDL_Color m_clearColor{};	
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;   //what about this 
		void RenderTexture(const Texture2D& texture, SDL_Rect & destinationRect, SDL_Rect & SourceRect) const;

		SDL_Color m_Black{ 0,0,0,255 };
		SDL_Color m_White{255,255,255,255};
		SDL_Color m_PowerUpColor{m_Black};
		void ToogleColor();
		bool m_IsBlack = true;

		float m_TotalTimeElapsed{};
		std::vector<std::pair<int, int>> m_positions;
		mutable SDL_Rect m_destinationRect{};
	
		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
		void DrawSquare(float x, float y, float size, const SDL_Color& color) const;
		void FillSquare(float x, float y, float size, const SDL_Color& color) const;
		void FillSquare(float x, float y, float width,float height, const SDL_Color& color) const;


	};
}

