#pragma once
#include "Component.h"
#include"functional"
#include <memory>
#include <glm/glm.hpp>
#include<vector>



struct Node
{
	int y;
	int x;
	int parentX;
	int parentY;
	float gCost;
	float hCost;
	float fCost;
};


namespace dae
{
	class SceneManager;
	class MapComponent;
	class RenderComponent;

	class AIComponent :public Component
	{

	public:

		AIComponent(std::shared_ptr<GameObject> Map, std::shared_ptr<GameObject> pTargetGameObejct);
		virtual void BeginPlay() override;
		virtual void Render() override;
		virtual void Update() override;
		bool m_dead = false;

		// Hooks used by the enemy FSM. When m_isFSMControlled is true,
		// AIComponent::Update() is a no-op and the FSM drives behaviour
		// by calling PerformChaseStep() / accessing the helpers below.
		void SetFSMControlled(bool v) { m_isFSMControlled = v; }
		void PerformChaseStep(float deltaTime);

		std::shared_ptr<MapComponent> GetMapComponent() const { return m_mapComponent; }
		std::shared_ptr<RenderComponent> GetRenderComponent() const { return m_renderComponent; }

	private:
		std::shared_ptr<GameObject> m_Map = nullptr;
		std::shared_ptr<GameObject> m_Target = nullptr;
		std::shared_ptr<MapComponent> m_mapComponent;
		std::shared_ptr<RenderComponent> m_renderComponent;
		dae::GameObject * m_Self = nullptr;
		float m_Speed;
		bool m_isFSMControlled = false;

		float distanceBetweenPoints();

		SceneManager * m_pSceneManager;



		std::vector<Node> makePath(std::vector<std::vector<Node>> map, Node dest);
		bool isValid(int x, int y);
		std::vector<Node> aStar(Node agent, Node dest);

		std::vector<Node> m_usablePath;
		
		float m_TileSize = 48.f;

		int m_currentPathIndex{};
		bool m_movingToTarget = false;

		float UpdatePathTimer = 0.2f;
		bool m_CanUpdateFirstNode = true;

		glm::vec2 m_targetNode{};

		//bool m_StopAi = false;
		bool m_startTimer = false;
		float m_TotalTimeElasped = 0;
		float m_TimerUntilMovingAgain = 4.f;

		void StunEnemy();

		float RandomFloatStep1(float min, float max);

		float m_Interval = 3.5;
		float m_TimerUpdateAi = m_Interval;

		bool m_shouldUpdateAi = true;
	


	};


};