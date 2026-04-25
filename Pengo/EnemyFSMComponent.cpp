#include "EnemyFSMComponent.h"

#include "GameObject.h"
#include "AiComponent.h"
#include "DebugRenderComponent.h"   // MapComponent lives here
#include "RenderComponent.h"
#include "SceneManager.h"
#include "IEnemyState.h"
#include "EnemyStates.h"

#include <functional>

dae::EnemyFSMComponent::EnemyFSMComponent(std::shared_ptr<GameObject> map)
	: m_mapObject{ std::move(map) }
{
}

void dae::EnemyFSMComponent::BeginPlay()
{
	m_pSceneManager = &SceneManager::GetInstance();

	if (auto * owner = GetOwner())
	{
		if (auto ai = owner->GetComponent<AIComponent>())
		{
			m_pAI = ai.get();
			m_pAI->SetFSMControlled(true);
		}
		if (auto rc = owner->GetComponent<RenderComponent>())
		{
			m_pRender = rc.get();
		}
	}

	if (m_mapObject)
	{
		if (auto mapComp = m_mapObject->GetComponent<MapComponent>())
		{
			m_pMap = mapComp.get();
			m_pMap->m_On3TilesMatched.Add(std::bind(&EnemyFSMComponent::OnTilesMatched, this));
		}
	}

	ChangeState(std::make_unique<ChaseState>());
}

void dae::EnemyFSMComponent::Update()
{
	if (m_dead) return;
	if (!m_pCurrentState) return;

	const float dt = m_pSceneManager ? m_pSceneManager->GetDeltaTime() : 0.f;
	m_pCurrentState->Update(*this, dt);
}

void dae::EnemyFSMComponent::Render()
{
}

void dae::EnemyFSMComponent::ChangeState(std::unique_ptr<IEnemyState> newState)
{
	if (m_pCurrentState)
	{
		m_pCurrentState->Exit(*this);
	}
	m_pCurrentState = std::move(newState);
	if (m_pCurrentState)
	{
		m_pCurrentState->Enter(*this);
	}
}

void dae::EnemyFSMComponent::Kill()
{
	m_dead = true;
	if (m_pAI) m_pAI->m_dead = true;
	ChangeState(std::make_unique<DeadState>());
}

void dae::EnemyFSMComponent::OnTilesMatched()
{
	if (m_dead) return;
	ChangeState(std::make_unique<StunnedState>());
}
