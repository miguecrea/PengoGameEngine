#pragma once


#include"Singleton.h"


namespace dae
{
	class TileStaticHelper final : public Singleton<TileStaticHelper>
	{
	public:
		int GetRow(float YPos);
		int GetColumn(float XpOS);
		TileStaticHelper();
	private:
		float m_tileSize = 48.f;		
	};

}
