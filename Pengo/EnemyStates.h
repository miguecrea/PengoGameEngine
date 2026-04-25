#pragma once

#include "IEnemyState.h"

namespace dae
{
	class EnemyFSMComponent;
}

class ChaseState : public IEnemyState
{
public:
	void Enter(dae::EnemyFSMComponent& fsm) override;
	void Update(dae::EnemyFSMComponent& fsm, float deltaTime) override;
	void Exit(dae::EnemyFSMComponent& fsm) override;
};

class StunnedState : public IEnemyState
{
public:
	void Enter(dae::EnemyFSMComponent& fsm) override;
	void Update(dae::EnemyFSMComponent& fsm, float deltaTime) override;
	void Exit(dae::EnemyFSMComponent& fsm) override;

private:
	float m_elapsed{ 0.f };
	const float m_stunDuration{ 4.f };
};

class DeadState : public IEnemyState
{
public:
	void Enter(dae::EnemyFSMComponent& fsm) override;
	void Update(dae::EnemyFSMComponent& fsm, float deltaTime) override;
	void Exit(dae::EnemyFSMComponent& fsm) override;
};
