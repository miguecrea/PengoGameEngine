#include "AIComponent.h"
#include "GameObject.h"
#include"SceneManager.h"
#include "Renderer.h"
#include"DebugRenderComponent.h"
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include<set>
#include <stack>
#include <array>
#include"staticHeader.h"
#include"RenderComponent.h"

#include <random>

#define X_MAX 720
#define X_STEP 48
#define Y_MAX 816
#define Y_STEP 48
#define COLUMNS 1
#define ROWS 1

// Use std::enable_shared_from_this<T> when your class needs to return shared_ptr<T> to itself.

//Always create the object via std::make_shared<T>().

//Prevents ownership bugs and double - deletion.


dae::AIComponent::AIComponent(std::shared_ptr<GameObject> Map, std::shared_ptr<GameObject> pTargetGameObejct) :
	m_Map{ Map }, m_Target{ pTargetGameObejct }, m_pSceneManager{ &SceneManager::GetInstance() }
{

	m_Speed = RandomFloatStep1(25.f, 60.f);
}

void dae::AIComponent::BeginPlay()
{
	m_Self = this->GetOwner();
	m_mapComponent = m_Map->GetComponent<dae::MapComponent>();
	m_mapComponent->m_On3TilesMatched.Add(std::bind(&AIComponent::StunEnemy, this));

	m_renderComponent = this->GetOwner()->GetComponent<dae::RenderComponent>();

	if (!m_Self)
	{
		std::cout << "Owner is null\n";
	}
}




inline bool operator < (const Node& lhs, const Node& rhs)
{//We need to overload "<" to put our struct into a set
	return lhs.fCost < rhs.fCost;
}

bool isDestination(int x, int y, Node dest)
{

	//row and columns of the tow are the same 
	if (x == dest.x && y == dest.y) {
		return true;
	}
	return false;
}


double calculateH(int x, int y, Node dest) {
	double H = (sqrt((x - dest.x) * (x - dest.x)
		+ (y - dest.y) * (y - dest.y)));
	return H;
}

bool dae::AIComponent::isValid(int x, int y)
{
							   // 15 columns             //rows 17 
	if (x < 0 || y < 0 || x >= (X_MAX / X_STEP) || y >= (Y_MAX / Y_STEP))
	{
		return false;
	}

	//
	if (m_mapComponent->MapArray[y][x] == 9 || m_mapComponent->MapArray[y][x] == 8 || m_mapComponent->MapArray[y][x] == 5)
	{
		return false;
	}
	return true;
}
std::vector<Node> dae::AIComponent::aStar(Node agent, Node dest)

{
	std::vector<Node> empty;

	if (!isValid(dest.x, dest.y))
	{
		//std::cout << "Destination is an obstacle" << std::endl;
		return empty;
	}

	if (isDestination(agent.x, agent.y, dest)) {
		//std::cout << "You are the destination" << std::endl;
		return empty;
	}

	// Dynamically allocate memory for the closedList


	  //2d array of bools   //15 columns
	bool** closedList = new bool* [X_MAX / X_STEP];

	// rows 
	for (int i = 0; i < X_MAX / X_STEP; ++i)
	{
		//for each  of column ,fill them with 17 rows 
		closedList[i] = new bool[Y_MAX / Y_STEP];
		std::fill_n(closedList[i], Y_MAX / Y_STEP, false);
	}




	//2d array as well    // 15 vectores ,each of the has size of 17
	std::vector<std::vector<Node>> allMap(X_MAX / X_STEP, std::vector<Node>(Y_MAX / Y_STEP));

	// Initialize whole map   //columns 
	for (int x = 0; x < X_MAX / X_STEP; x++)
	{
		for (int y = 0; y < Y_MAX / Y_STEP; y++) {
			allMap[x][y].fCost = FLT_MAX;
			allMap[x][y].gCost = FLT_MAX;
			allMap[x][y].hCost = FLT_MAX;
			allMap[x][y].parentX = -1;
			allMap[x][y].parentY = -1;
			allMap[x][y].x = x;
			allMap[x][y].y = y;

			closedList[x][y] = false;
		}
	}

	// Initialize starting point
	int x = agent.x;  //column
	int y = agent.y;  //row 
	allMap[x][y].fCost = 0.0;
	allMap[x][y].gCost = 0.0;
	allMap[x][y].hCost = 0.0;
	allMap[x][y].parentX = x;
	allMap[x][y].parentY = y;

	std::vector<Node> openList;
	openList.emplace_back(allMap[x][y]);

	bool destinationFound = false;

	while (!openList.empty() && openList.size() < (X_MAX / X_STEP) * (Y_MAX / Y_STEP))
	{
		Node node;
		//it = next(it)
		do {
			float temp = FLT_MAX;
			std::vector<Node>::iterator itNode;
			for (std::vector<Node>::iterator it = openList.begin();
				it != openList.end(); ++it) {
				Node n = *it;
				if (n.fCost < temp)
				{
					temp = n.fCost;
					itNode = it;
				}
			}
			node = *itNode;
			openList.erase(itNode);
		} while (!isValid(node.x, node.y) && !openList.empty());

		x = node.x;
		y = node.y;
		closedList[x][y] = true;

		for (int newX = -1; newX <= 1; newX++)
		{
			for (int newY = -1; newY <= 1; newY++) {
				double gNew, hNew, fNew;
				if (isValid(x + newX, y + newY)) {
					if (isDestination(x + newX, y + newY, dest))
					{
						//Lines block diagonals //before checking if it is destinantion
						if (newX != 0 && newY != 0) {
							continue;
						}


						allMap[x + newX][y + newY].parentX = x;
						allMap[x + newX][y + newY].parentY = y;
						destinationFound = true;

						// Clean up dynamically allocated memory
						for (int i = 0; i < X_MAX / X_STEP; ++i) {
							delete[] closedList[i];
						}
						delete[] closedList;

						return makePath(allMap, dest);
					}
					else if (!closedList[x + newX][y + newY]) {
						gNew = node.gCost + 1.0;
						hNew = calculateH(x + newX, y + newY, dest);
						fNew = gNew + hNew;
						if (allMap[x + newX][y + newY].fCost == FLT_MAX ||
							allMap[x + newX][y + newY].fCost > fNew)
						{

							//lines blocks diagonals too 
							if (newX != 0 && newY != 0) {
								fNew = FLT_MAX;
								gNew = FLT_MAX;
								hNew = FLT_MAX;
							}


							allMap[x + newX][y + newY].fCost = float(fNew);
							allMap[x + newX][y + newY].gCost = float(gNew);
							allMap[x + newX][y + newY].hCost = float(hNew);
							allMap[x + newX][y + newY].parentX = x;
							allMap[x + newX][y + newY].parentY = y;
							openList.emplace_back(allMap[x + newX][y + newY]);
						}
					}
				}
			}
		}
	}


	if (!destinationFound)
	{
		//std::cout << "Destination not found" << std::endl;
		for (int i = 0; i < X_MAX / X_STEP; ++i) {
			delete[] closedList[i];
		}
		delete[] closedList;
		return empty;
	}
	return empty;
}

void dae::AIComponent::StunEnemy()
{
	// When the FSM owns this enemy, leave stun handling to the StunnedState.
	if (m_isFSMControlled) return;

	s_PauseEnemy = true;
	m_startTimer = true;
	m_renderComponent->m_state = 1;
}

float dae::AIComponent::RandomFloatStep1(float min, float max)
{


	if (min > max) std::swap(min, max);

	int minInt = static_cast<int>(std::ceil(min));
	int maxInt = static_cast<int>(std::floor(max));

	if (minInt > maxInt) return minInt * 1.0f; // no valid integer step in range

	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(minInt, maxInt);

	return static_cast<float>(dist(gen));



}


std::vector<Node> dae::AIComponent::makePath(std::vector<std::vector<Node>> map, Node dest) {
	try {
		//std::cout << "Found a path" << std::endl;
		int x = dest.x;
		int y = dest.y;
		std::stack<Node> path;
		std::vector<Node> usablePath;

		while (!(map[x][y].parentX == x && map[x][y].parentY == y)
			&& map[x][y].x != -1 && map[x][y].y != -1)
		{
			path.push(map[x][y]);
			int tempX = map[x][y].parentX;
			int tempY = map[x][y].parentY;
			x = tempX;
			y = tempY;

		}
		path.push(map[x][y]);

		while (!path.empty()) {
			Node top = path.top();
			path.pop();
			usablePath.emplace_back(top);
		}
		//std::cout << "Path full, " << usablePath.size() << std::endl;
		return usablePath;
	}
	catch (const std::exception& e)
	{
		std::cout << "Path empty, error " << e.what() << std::endl;
		return std::vector<Node>();
	}
}




void dae::AIComponent::Render()
{


}

void dae::AIComponent::Update()
{
	if (m_dead) return;

	// When the FSM owns this enemy, the FSM drives behaviour every tick.
	if (m_isFSMControlled) return;

	if (m_startTimer)
	{
		m_TotalTimeElasped += m_pSceneManager->GetDeltaTime();

		if (m_TotalTimeElasped >= m_TimerUntilMovingAgain)
		{
			m_TotalTimeElasped = 0;
			m_startTimer = false;
			s_PauseEnemy = false;
			m_renderComponent->m_state = 0;
		}
	}

	if (s_PauseEnemy) return;

	PerformChaseStep(m_pSceneManager->GetDeltaTime());
}

void dae::AIComponent::PerformChaseStep(float deltaTime)
{
	if (!m_Self || !m_Target) return;

	Node currentPos;
	currentPos.x = int(m_Self->GetWorldPosition().x) / X_STEP;
	currentPos.y = int(m_Self->GetWorldPosition().y) / Y_STEP;

	Node targetPos;
	targetPos.x = int(m_Target->GetWorldPosition().x) / X_STEP;
	targetPos.y = int(m_Target->GetWorldPosition().y) / Y_STEP;

	m_usablePath = aStar(currentPos, targetPos);

	if (m_usablePath.size() > 1)
	{
		glm::vec2 targetNode =
		{
			float(m_usablePath[1].x * X_STEP),
			float(m_usablePath[1].y * Y_STEP)
		};

		glm::vec2 currentPosLocation = m_Self->GetLocalPosition();
		glm::vec2 toTarget = targetNode - currentPosLocation;
		float distance = glm::length(toTarget);

		const float snapThreshold = 0.05f;

		if (distance < snapThreshold)
		{
			m_Self->SetPosition(targetNode.x, targetNode.y);
		}
		else
		{
			glm::vec2 moveDir = toTarget / distance;
			float speed = m_Speed * deltaTime;
			glm::vec2 moveVec = moveDir * speed;

			m_Self->SetPosition(
				currentPosLocation.x + moveVec.x,
				currentPosLocation.y + moveVec.y
			);
		}
	}
}

float dae::AIComponent::distanceBetweenPoints()
{
	float targetX = m_Target->GetWorldPosition().x;
	float targetY = m_Target->GetWorldPosition().y;
	float selfX = m_Self->GetWorldPosition().x;
	float selfY = m_Self->GetWorldPosition().y;

	float dx = targetX - selfX;
	float dy = targetY - selfY;

	// Calculate the sum of squares
	float sumOfSquares = dx * dx + dy * dy;

	float distance = std::sqrt(sumOfSquares);

	return distance;
}





















