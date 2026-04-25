#include "TileStaticHelper.h"
#include<cmath>


int dae::TileStaticHelper::GetRow(float YPos)
{
	return static_cast<int>(std::floor(YPos / m_tileSize));
}

int dae::TileStaticHelper::GetColumn(float XpOS)
{
	return  static_cast<int>(std::floor(XpOS / m_tileSize));
}

dae::TileStaticHelper::TileStaticHelper()
{
}
