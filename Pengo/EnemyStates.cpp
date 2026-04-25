#include "EnemyStates.h"

#include "EnemyFSMComponent.h"
#include "AiComponent.h"
#include "RenderComponent.h"
#include "staticHeader.h"  // s_PauseEnemy

// ---------------- ChaseState ----------------

void ChaseState::Enter(dae::EnemyFSMComponent& fsm)
{
	if (auto* render = fsm.GetRenderComp())
	{
		render->m_state = 0;
	}
}

void ChaseState::Update(dae::EnemyFSMComponent & fsm, float deltaTime)
{
	// Respect global pause flag (collision response uses it elsewhere).
	if (s_PauseEnemy) return;

	if (auto * ai = fsm.GetAI())
	{
		ai->PerformChaseStep(deltaTime);
	}
}

void ChaseState::Exit(dae::EnemyFSMComponent& /*fsm*/)
{
}

// ---------------- StunnedState ----------------

void StunnedState::Enter(dae::EnemyFSMComponent & fsm)
{
	m_elapsed = 0.f;
	s_PauseEnemy = true;
	if (auto* render = fsm.GetRenderComp())
	{
		render->m_state = 1;
	}
}

void StunnedState::Update(dae::EnemyFSMComponent & fsm, float deltaTime)
{
	m_elapsed += deltaTime;
	if (m_elapsed >= m_stunDuration)
	{
		fsm.ChangeState(std::make_unique<ChaseState>());
	}
}

void StunnedState::Exit(dae::EnemyFSMComponent& fsm)
{
	s_PauseEnemy = false;
	if (auto* render = fsm.GetRenderComp())
	{
		render->m_state = 0;
	}
}

// ---------------- DeadState ----------------

void DeadState::Enter(dae::EnemyFSMComponent& /*fsm*/)
{
}

void DeadState::Update(dae::EnemyFSMComponent& /*fsm*/, float /*deltaTime*/)
{
}

void DeadState::Exit(dae::EnemyFSMComponent& /*fsm*/)
{
}
