#pragma once

#include "Component.h"
#include <memory>

class IEnemyState;

namespace dae
{
	class GameObject;
	class AIComponent;
	class MapComponent;
	class RenderComponent;
	class SceneManager;

	class EnemyFSMComponent : public Component
	{
	public:
		explicit EnemyFSMComponent(std::shared_ptr<GameObject> map);

		virtual void BeginPlay() override;
		virtual void Update() override;
		virtual void Render() override;

		void ChangeState(std::unique_ptr<IEnemyState> newState);

		AIComponent* GetAI() const { return m_pAI; }
		RenderComponent* GetRenderComp() const { return m_pRender; }
		MapComponent* GetMapComp() const { return m_pMap; }
		SceneManager* GetSceneManager() const { return m_pSceneManager; }

		void Kill();
		bool IsDead() const { return m_dead; }

	private:
		void OnTilesMatched();

		std::shared_ptr<GameObject> m_mapObject;
		AIComponent* m_pAI{ nullptr };
		RenderComponent* m_pRender{ nullptr };
		MapComponent* m_pMap{ nullptr };
		SceneManager* m_pSceneManager{ nullptr };

		std::unique_ptr<IEnemyState> m_pCurrentState;
		bool m_dead{ false };
	};
}
