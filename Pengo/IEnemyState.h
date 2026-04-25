#pragma once

namespace dae
{
	class EnemyFSMComponent;
}

class IEnemyState
{
public:
	virtual ~IEnemyState() = default;

	virtual void Enter(dae::EnemyFSMComponent& fsm) = 0;
	virtual void Update(dae::EnemyFSMComponent& fsm, float deltaTime) = 0;
	virtual void Exit(dae::EnemyFSMComponent& fsm) = 0;
};
