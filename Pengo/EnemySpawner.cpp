#include "EnemySpawner.h"
#include"GameObject.h"
#include"TileComponent.h"
#include"AiComponent.h"
#include"EnemyFSMComponent.h"
#include"CollisionWithComponent.h"
#include"CollsionResponse.h"
#include"AnimationComponent.h"
#include"RenderComponent.h"
#include"RectangleComponent.h"
#include"SceneManager.h"
#include"Scene.h"
#include"staticHeader.h"
#include"TagComponent.h"
#include"Tags.h"
#include"IEnemyState.h"

dae::EnemySpawner::EnemySpawner(std::vector<std::shared_ptr<dae::GameObject>> Tiles, std::shared_ptr<dae::GameObject > Map, std::shared_ptr<dae::GameObject> Pengo, const std::string fileName):
	m_Tiles{Tiles},m_Map{Map},m_Pengo{Pengo},m_fileName{fileName}
{

}

void dae::EnemySpawner::BeginPlay()
{


	for (size_t i = 0; i < m_Tiles.size(); i++)
	{
		auto tileComponent = m_Tiles[i]->GetComponent<dae::TileComponent>();
	
		using namespace std::placeholders;
		if (tileComponent)
		{
		tileComponent->m_BlockHatched.Add(std::bind(&EnemySpawner::SpawnEnemyCallBack, this, _1, _2));

		}
	
	}
	
}

void dae::EnemySpawner::Render()
{
}

void dae::EnemySpawner::Update()
{

}

void dae::EnemySpawner::SpawnEnemyCallBack(float x, float y)
{
	SpawEnemy(x, y);
}

void dae::EnemySpawner::SpawEnemy(float x, float y)
{
	auto Enemy = std::make_shared<dae::GameObject>();
	auto EnemyRenderer = std::make_shared<dae::RenderComponent>(-2, true);
	auto EnemyAi = std::make_shared<dae::AIComponent>(m_Map,m_Pengo);
	auto EnemyFSM = std::make_shared<dae::EnemyFSMComponent>(m_Map);
	auto EnemyRectComponent = std::make_shared<dae::RectangleComponent>(30, 30);
	auto CollisionComponentEnmies = std::make_shared<dae::CollisionWithComponent>(m_Tiles);
	auto CollisionResponseEnemy = std::make_shared<dae::CollsionResponse>();
	CollisionResponseEnemy->m_EnemySpawner = this;
	EnemyRenderer->SetTexture(m_fileName);
	EnemyRenderer->AddToVector(5, 8, 5, 5, dae::AnimationComponent::Type::loop, 0, 1);//down
	EnemyRenderer->AddToVector(5, 8, 8, 5, dae::AnimationComponent::Type::StartAtNoZero, 6, 0);//down
	EnemyRenderer->SetDimension(2.5f);
	Enemy->AddComponent(EnemyRenderer);
	Enemy->AddComponent(EnemyAi);
	Enemy->AddComponent(EnemyFSM);   // Drives behaviour via the state machine
	Enemy->AddComponent(EnemyRectComponent);
	Enemy->AddComponent(CollisionComponentEnmies);
	Enemy->AddComponent(CollisionResponseEnemy);
	Enemy->SetPosition(x,y);

	m_NumberActiveEnemies++;

	Enemy->BeginPlay();
	SceneManager::GetInstance().GetCurrentScene()->Add(Enemy);

}

void dae::EnemySpawner::DecreaseEnemyCount()
{
	std::cout<<"response\n";
	m_NumberActiveEnemies--;

	if (m_NumberActiveEnemies <= 0)
	{
		m_GameOver.Broadcast();
	}
}


