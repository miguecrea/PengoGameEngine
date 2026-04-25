#include "DebugRenderComponent.h"
#include"Renderer.h"
#include"iostream"
#include"RenderComponent.h"
#include"GameObject.h"
#include"SceneManager.h"
#include"Scene.h"
#include"PengoComponent.h"
#include"TileComponent.h"
#include"TileStaticHelper.h"
#include"GameObject.h"
#include"RenderComponent.h"
#include"Audio.h"
#include"staticHeader.h"
#include"RectangleComponent.h"
#include"TagComponent.h"
#include"Tags.h"
#include <fstream>
#include <sstream>
#include <algorithm>

#define BLOCK 9
#define COLLISION 8
#define SPECIAL_CUBE 5
#define UNHATCH_EGGS 7
#define FREE 0

dae::MapComponent::MapComponent(std::shared_ptr<dae::GameObject> Player,const std::string & mapFileName, int NumberEnemies, std::shared_ptr<dae::GameObject> Player2) :
	m_Pengo{ Player },m_MapFileName{ mapFileName },m_NumberEnemies{ NumberEnemies },m_Player2{Player2}
{

	parseMapFile(m_MapFileName);

	for (int y = 0; y < m_NumberTilesY; ++y)
	{
		for (int x = 0; x < m_NumberTilesX; ++x)
		{
			if (MapArray[y][x] == BLOCK)
			{

				//only the block has a reference 
				auto IceBlockGameObject = std::make_shared<dae::GameObject>();
				auto DiamondBlockRenderer = std::make_shared<dae::RenderComponent>(-2,true);

				DiamondBlockRenderer->SetTexture("MIGUEL_Diamonds.png");
				DiamondBlockRenderer->AddToVector(1, 2,1, 1, dae::AnimationComponent::Type::StartAtNoZero, 0, 0);
				DiamondBlockRenderer->AddToVector(1, 2,1, 1, dae::AnimationComponent::Type::StartAtNoZero, 1, 0);
				auto TileComponent = std::make_shared<dae::TileComponent>();
				auto RectComponent = std::make_shared<dae::RectangleComponent>(48,48);
				auto TagComponent = std::make_shared<dae::TagComponent>(CUBE);

				TileComponent->m_CurrentRow = y;
				TileComponent->m_CurrentColumn = x;

				if ( (x % 2 == 0 || y % 2 == 1) && NumberOfEnemiesNest < m_NumberEnemies)
				{
				NumberOfEnemiesNest++;
				TileComponent->m_IsNest = true;
				m_TilesWidhEnemies.push_back(IceBlockGameObject);
				DiamondBlockRenderer->m_state = 0;

				}
				else
				{
				DiamondBlockRenderer->m_state = 1;

				}
				DiamondBlockRenderer->SetDimension(3.f);
				IceBlockGameObject->AddComponent(DiamondBlockRenderer);
				IceBlockGameObject->AddComponent(RectComponent);
				IceBlockGameObject->AddComponent(TileComponent);
				IceBlockGameObject->AddComponent(TagComponent);
				IceBlockGameObject->SetPosition(x * m_TileSize, y * m_TileSize);
				SceneManager::GetInstance().GetCurrentScene()->Add(IceBlockGameObject);
				m_TileObjects[y][x] = IceBlockGameObject;
				TileComponent->SetMap(this);

				m_TilesVector.push_back(IceBlockGameObject);



			}

			if (MapArray[y][x] == SPECIAL_CUBE)
			{
				auto DiamondBlockGameObject = std::make_shared<dae::GameObject>();
				auto DiamondBlockRenderer = std::make_shared<dae::RenderComponent>(-2, false);
				auto TileComponent = std::make_shared<dae::TileComponent>();
				auto RectComponent = std::make_shared<dae::RectangleComponent>(48, 48);
				auto TagComponent = std::make_shared<dae::TagComponent>(CUBE);

				DiamondBlockRenderer->SetTexture("DiamondBlock.png");
				DiamondBlockRenderer->SetDimension(3.f);
				DiamondBlockGameObject->AddComponent(DiamondBlockRenderer);
				DiamondBlockGameObject->AddComponent(TileComponent);
				DiamondBlockGameObject->AddComponent(RectComponent);
				DiamondBlockGameObject->AddComponent(TagComponent);
				DiamondBlockGameObject->SetPosition(x * m_TileSize, y * m_TileSize);
				SceneManager::GetInstance().GetCurrentScene()->Add(DiamondBlockGameObject);
				m_TileObjects[y][x] = DiamondBlockGameObject;
				m_SpeciaTiles.push_back(TileInfo{ y,x });

				m_TilesVector.push_back(DiamondBlockGameObject);
			}
		}
	}




}
void dae::MapComponent::BeginPlay()
{
	if (!m_Pengo)
	{
		std::cout << "Error\n";
		return;
	}
	auto PengoComponent = m_Pengo->GetComponent<dae::PengoComponent>();
	if (PengoComponent)
	{
		using namespace std::placeholders;
		PengoComponent->m_OnPengoBreakOrMove.Add(std::bind(&MapComponent::PengoAttackResponse, this, _1, _2, _3,_4));
	}


	if (m_Player2)
	{
		auto PengoComponent2 = m_Player2->GetComponent<dae::PengoComponent>();
		if (PengoComponent2)
		{
			using namespace std::placeholders;
			PengoComponent2->m_OnPengoBreakOrMove.Add(std::bind(&MapComponent::PengoAttackResponse, this, _1, _2, _3,_4));
		}
	}








}

void dae::MapComponent::Render()
{
}

void dae::MapComponent::Update()
{



}

std::vector<std::shared_ptr<dae::GameObject>> dae::MapComponent::GetTiles()
{
	return m_TilesVector;
}

std::vector<std::shared_ptr<dae::GameObject>> dae::MapComponent::GetTilesWidthEnmies()
{
	return m_TilesWidhEnemies;
}

void dae::MapComponent::SetPlayer2(std::shared_ptr<dae::GameObject> player2)
{
	m_Player2 = player2;
}

void dae::MapComponent::parseMapFile(const std::string & filename)
{

	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Failed to open map file: " << filename << std::endl;
		return;
	}

	std::string line;
	int row = 0;

	while (std::getline(file, line) && row < MapHeight)
	{
		std::istringstream ss(line);
		for (int col = 0; col < MapWidth; ++col)
		{
			ss >> MapArray[row][col];
		}
		++row;
	}

	file.close();

	

}

void dae::MapComponent::PengoAttackResponse(Direction PengoDirection, int currenRow, int currentColumn,bool OnlyBreak)
{

	int TilesToChekc{ 1 };

	auto HasBlockInFrontOneCube = HasABlockInFront(PengoDirection, currenRow, currentColumn, TilesToChekc, BLOCK);
	auto HasBlockInFrontSpecial = HasABlockInFront(PengoDirection, currenRow, currentColumn, TilesToChekc, SPECIAL_CUBE);

	if (HasBlockInFrontOneCube.m_hasSomething || HasBlockInFrontSpecial.m_hasSomething)
	{
		TilesToChekc = 2;

		auto HasNormalBlockInFrontTwoCubes = HasABlockInFront(PengoDirection, currenRow, currentColumn, TilesToChekc, BLOCK);
		auto IsThereWallTwoSpacesAway = HasABlockInFront(PengoDirection, currenRow, currentColumn, TilesToChekc, COLLISION);
		auto HasSpecialBlock2SpacesAwat = HasABlockInFront(PengoDirection, currenRow, currentColumn, TilesToChekc, SPECIAL_CUBE);

		if (HasNormalBlockInFrontTwoCubes.m_hasSomething || IsThereWallTwoSpacesAway.m_hasSomething || HasSpecialBlock2SpacesAwat.m_hasSomething)
		{

			if (HasBlockInFrontOneCube.m_hasSomething)
			{
				std::cout << "Cube in front Is Normal\n";
				m_IncreaseScoreDelegate.Broadcast();
				DestroyCube(HasBlockInFrontOneCube);

			}
			else if (HasBlockInFrontSpecial.m_hasSomething)
			{
				std::cout << "Cube in front Is Special\n";
			}

		}
		else
		{

			if (OnlyBreak) return;

			TileInfo NextOccupiedTile{};
			TileInfo NextFoundCollision{};
			TileInfo NextFoundSpecialBlock{};
			do
			{
				TilesToChekc++;
				NextOccupiedTile = HasABlockInFront(PengoDirection, currenRow, currentColumn, TilesToChekc, BLOCK);
				NextFoundCollision = HasABlockInFront(PengoDirection, currenRow, currentColumn, TilesToChekc, COLLISION);
				NextFoundSpecialBlock = HasABlockInFront(PengoDirection, currenRow, currentColumn, TilesToChekc, SPECIAL_CUBE);

			} while (!NextOccupiedTile.m_hasSomething && !NextFoundCollision.m_hasSomething && !NextFoundSpecialBlock.m_hasSomething);

			int MovingTileRow{};
			int MovingTileColumn{};

			if (HasBlockInFrontSpecial.m_hasSomething)
			{
				MovingTileRow = HasBlockInFrontSpecial.row;
				MovingTileColumn = HasBlockInFrontSpecial.column;
			}
			else
			{
				MovingTileRow = HasBlockInFrontOneCube.row;
				MovingTileColumn = HasBlockInFrontOneCube.column;
			}


			auto TileInFrontTileComponent = m_TileObjects[MovingTileRow][MovingTileColumn]->GetComponent<dae::TileComponent>();
			auto RectComponent = m_TileObjects[MovingTileRow][MovingTileColumn]->GetComponent<dae::RectangleComponent>();

			int DesiredRow{};
			int DesiredColumn{};


			if (NextOccupiedTile.m_hasSomething)
			{
				ReturnDesired(DesiredRow, DesiredColumn, NextOccupiedTile);
			}
			else if (NextFoundCollision.m_hasSomething)
			{
				ReturnDesired(DesiredRow, DesiredColumn, NextFoundCollision);
			}
			else if (NextFoundSpecialBlock.m_hasSomething) 
			{
				ReturnDesired(DesiredRow, DesiredColumn, NextFoundSpecialBlock);
			}

			if (HasBlockInFrontOneCube.m_hasSomething)
			{

				MapArray[MovingTileRow][MovingTileColumn] = FREE;
				MapArray[DesiredRow][DesiredColumn] = BLOCK;
			}
			else if (HasBlockInFrontSpecial.m_hasSomething)
			{


				MapArray[MovingTileRow][MovingTileColumn] = FREE;
				MapArray[DesiredRow][DesiredColumn] = SPECIAL_CUBE;

				auto it = std::find_if(m_SpeciaTiles.begin(), m_SpeciaTiles.end(), [MovingTileRow, MovingTileColumn](const dae::TileInfo & tile) {
					return tile.row == MovingTileRow && tile.column == MovingTileColumn;
					});

				
				if (it != m_SpeciaTiles.end())
				{
				    std::cout << "MOVED  Special Tile\n";
					it->row = DesiredRow;
					it->column = DesiredColumn;
				}

				if (HasThreeAlignedTiles(m_SpeciaTiles))
				{

					m_On3TilesMatched.Broadcast();
					std::cout << "Alignment detected!\n";
				}
				else {
					std::cout << "No alignment.\n";
				}
				
			}

			TileInFrontTileComponent->m_DesiredRow = DesiredRow;
			TileInFrontTileComponent->m_DesiredColumn = DesiredColumn;

			m_TileObjects[DesiredRow][DesiredColumn] = m_TileObjects[MovingTileRow][MovingTileColumn];
			m_TileObjects[MovingTileRow][MovingTileColumn].reset();

			TileInFrontTileComponent->SetActive(PengoDirection);
			RectComponent->m_Active = true;
			dae::SoundSystem& audio{ dae::Audio::Get() };
			audio.Play(s_PushedBlockSound, 1.f, 0);
		}


	}
	std::cout << "\n\n\n";

}

void dae::MapComponent::DestroyCube(dae::TileInfo& HasBlockInFrontOneCube)
{
	auto object = m_TileObjects[HasBlockInFrontOneCube.row][HasBlockInFrontOneCube.column];

	if (object)
	{

		dae::SoundSystem& audio{ dae::Audio::Get() };
		audio.Play(s_IceBlockDestroyed, 1.f, 0);
		object->GetComponent<dae::RenderComponent>()->SwapVisibility();
		object->GetComponent<dae::TileComponent>()->m_Destroyed = true;
		m_TileObjects[HasBlockInFrontOneCube.row][HasBlockInFrontOneCube.column].reset();
		MapArray[HasBlockInFrontOneCube.row][HasBlockInFrontOneCube.column] = FREE;
		std::cout << MapArray[HasBlockInFrontOneCube.row][HasBlockInFrontOneCube.column] << "\n";

	}
}



dae::TileInfo dae::MapComponent::HasABlockInFront(Direction PengoDirection, int row, int column, int numberTilesToCheck, int TypeOBlock)
{

	TileInfo TileInfo{};

	TileInfo.direction = PengoDirection;
	int rowToCheck{};
	int columnToCheck{};

	switch (PengoDirection)
	{
	case Direction::UP:

		if (MapArray[row - numberTilesToCheck][column] == TypeOBlock)
		{
			rowToCheck = row - numberTilesToCheck;
			columnToCheck = column;
			TileInfo.row = rowToCheck;
			TileInfo.column = columnToCheck;
			TileInfo.m_hasSomething = true;
		}

		//std::cout << "up\n";
		return TileInfo;

		break;
	case Direction::DOWN:

		if (MapArray[row + numberTilesToCheck][column] == TypeOBlock)
		{

			rowToCheck = row + numberTilesToCheck;
			columnToCheck = column;

			TileInfo.row = rowToCheck;
			TileInfo.column = columnToCheck;
			TileInfo.m_hasSomething = true;
		}

		//std::cout << "down\n";
		return TileInfo;

		break;
	case Direction::LEFT:

		if (MapArray[row][column - numberTilesToCheck] == TypeOBlock)
		{

			rowToCheck = row;
			columnToCheck = column - numberTilesToCheck;

			TileInfo.row = rowToCheck;
			TileInfo.column = columnToCheck;
			TileInfo.m_hasSomething = true;
		}



		//std::cout << "left\n";

		return TileInfo;


		break;
	case Direction::RIGHT:


		if (MapArray[row][column + numberTilesToCheck] == TypeOBlock)
		{

			rowToCheck = row;
			columnToCheck = column + numberTilesToCheck;

			TileInfo.row = rowToCheck;
			TileInfo.column = columnToCheck;
			TileInfo.m_hasSomething = true;
		}
		//std::cout << "right\n";

		return TileInfo;

		break;
	default:

		return TileInfo;
		break;
	}

}

bool dae::MapComponent::IsCubeCollision(Direction PengoDirection, int row, int column, int numberTilesToCheck)
{

	switch (PengoDirection)
	{
	case Direction::UP:

		if (MapArray[row - numberTilesToCheck][column] == COLLISION)
		{
			return true;
		}

		return false;

		break;
	case Direction::DOWN:

		if (MapArray[row + numberTilesToCheck][column] == COLLISION)
		{
			return true;
		}

		return false;

		break;
	case Direction::LEFT:

		if (MapArray[row][column - numberTilesToCheck] == COLLISION)
		{

			return true;
		}
		return false;


		break;
	case Direction::RIGHT:


		if (MapArray[row][column + numberTilesToCheck] == COLLISION)
		{
			return true;

		}
		return false;

		break;
	default:

		return false;
		break;
	}
}

void dae::MapComponent::ReturnDesired(int& row, int& column, const TileInfo& Tile)
{

	row = Tile.row;
	column = Tile.column;

	switch (Tile.direction)
	{
	case Direction::UP:

		row += 1;


		break;
	case Direction::DOWN:

		row -= 1;

		break;
	case Direction::LEFT:

		column += 1;

		break;
	case Direction::RIGHT:

		column -= 1;
		break;
	default:
		break;
	}

}

bool dae::MapComponent::HasThreeAlignedTiles(const std::vector<TileInfo>& tiles)
{
	
	std::unordered_map<int, std::vector<int>> rows;    // row index -> list of columns
	std::unordered_map<int, std::vector<int>> columns; // column index -> list of rows

	// Group tiles by row and column
	for (const TileInfo& tile : tiles) {
		rows[tile.row].push_back(tile.column);
		columns[tile.column].push_back(tile.row);
	}

	// Helper lambda to check for 3 or more consecutive values
	auto hasThreeConsecutive = [](std::vector<int>& values) -> bool {
		if (values.size() < 3) return false;

		std::sort(values.begin(), values.end());
		int count = 1;

		for (size_t i = 1; i < values.size(); ++i) {
			if (values[i] == values[i - 1] + 1) {
				count++;
				if (count >= 3) return true;
			}
			else if (values[i] != values[i - 1]) {
				count = 1;
			}
		}

		return false;
		};


	for (auto& pair : rows) {
		int rowIndex = pair.first;
		rowIndex += 0;
		auto& columnList = pair.second;

		if (hasThreeConsecutive(columnList))
			return true;
	}
	


	// Check each column for 3+ consecutive rows
	
	for (auto& pair : columns)
	{
		int columnIndex = pair.first;
		columnIndex += 0;
		auto& rowList = pair.second;

		if (hasThreeConsecutive(rowList))
			return true;
	}
	return false;
}

bool dae::MapComponent::percentage(int percent)
{
	 return std::rand() % 100 < percent;
}


