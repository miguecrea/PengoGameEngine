#pragma once

#include"InputManager.h"
#include"GameObject.h"
#include"RenderComponent.h"
#include"SetPositionCommand.h"
#include"QbertDirection.h"
#include"AddInput.h"




void AddControllerKeyboardPlayer1(std::shared_ptr<dae::GameObject>& Qbert, std::shared_ptr<dae::RenderComponent>& QbertRenderer)
{

	auto& input = dae::InputManager::GetInstance();

	float setSpeedX{ 12.f };
	float setSpeedy{ 18.f };
	auto controller{ input.AddController() };

	glm::vec3 direction{ -setSpeedX,-setSpeedy,0.f };
	std::unique_ptr<dae::Command> pMoveCommand{ std::make_unique<dae::SetPositionCommand>(Qbert.get(), direction,QbertRenderer,static_cast<int>(QBertDirection::LEFT_UP)) };  //player 1 and direction
	controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonY, std::move(pMoveCommand), dae::ButtonState::Down);
	
	direction = { setSpeedX,-setSpeedy,0.f };
	pMoveCommand = { std::make_unique<dae::SetPositionCommand>(Qbert.get(), direction,QbertRenderer,static_cast<int>(QBertDirection::RIGH_UP)) };
	controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonB, std::move(pMoveCommand), dae::ButtonState::Down);


	direction = { -setSpeedX,setSpeedy,0.f };
	pMoveCommand = std::make_unique<dae::SetPositionCommand>(Qbert.get(), direction, QbertRenderer, static_cast<int>(QBertDirection::LEFT_DOWN));  //player 1 and direction
	controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonX, std::move(pMoveCommand), dae::ButtonState::Down);


	//y positive
	direction = { setSpeedX,setSpeedy,0.f };
	pMoveCommand = { std::make_unique<dae::SetPositionCommand>(Qbert.get(), direction,QbertRenderer,static_cast<int>(QBertDirection::RIGHT_DOWN)) };
	controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonA, std::move(pMoveCommand), dae::ButtonState::Down);

	auto keyboard{ input.GetKeyboard() };   //second uses keyboard 


	direction = { -setSpeedX,-setSpeedy,0.f };
	pMoveCommand = { std::make_unique<dae::SetPositionCommand>(Qbert.get(), direction,QbertRenderer,static_cast<int>(QBertDirection::LEFT_UP)) };
	keyboard->MapCommandToButton(SDL_SCANCODE_W, std::move(pMoveCommand), dae::ButtonState::Down);

	direction = { setSpeedX,-setSpeedy,0.f };
	pMoveCommand = { std::make_unique<dae::SetPositionCommand>(Qbert.get(), direction,QbertRenderer,static_cast<int>(QBertDirection::RIGH_UP)) };
	keyboard->MapCommandToButton(SDL_SCANCODE_R, std::move(pMoveCommand), dae::ButtonState::Down);

	direction = { -setSpeedX,setSpeedy,0.f };
	pMoveCommand = { std::make_unique<dae::SetPositionCommand>(Qbert.get(), direction,QbertRenderer,static_cast<int>(QBertDirection::LEFT_DOWN)) };
	keyboard->MapCommandToButton(SDL_SCANCODE_S, std::move(pMoveCommand), dae::ButtonState::Down);

	direction = { setSpeedX,setSpeedy,0.f };
	pMoveCommand = { std::make_unique<dae::SetPositionCommand>(Qbert.get(),direction,QbertRenderer,static_cast<int>(QBertDirection::RIGHT_DOWN)) };
	keyboard->MapCommandToButton(SDL_SCANCODE_F, std::move(pMoveCommand), dae::ButtonState::Down);







}

void AddControllerKeyboardPlayer2(std::shared_ptr<dae::GameObject>& snake, std::shared_ptr<dae::RenderComponent>& snakeRenderer)
{
	auto& input = dae::InputManager::GetInstance();

	float setSpeedX{ 12.f };
	float setSpeedy{ 18.f };
	auto controller{ input.AddController() };

	glm::vec3 direction{ -setSpeedX,-setSpeedy,0.f };
	std::unique_ptr<dae::Command> pMoveCommand{ std::make_unique<dae::SetPositionCommand>(snake.get(), direction,snakeRenderer,static_cast<int>(QBertDirection::LEFT_UP)) };  //player 1 and direction
	controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonY, std::move(pMoveCommand), dae::ButtonState::Down);


	//
	direction = { setSpeedX,-setSpeedy,0.f };
	pMoveCommand = { std::make_unique<dae::SetPositionCommand>(snake.get(), direction,snakeRenderer,static_cast<int>(QBertDirection::RIGH_UP)) };
	controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonB, std::move(pMoveCommand), dae::ButtonState::Down);


	direction = { -setSpeedX,setSpeedy,0.f };
	pMoveCommand = std::make_unique<dae::SetPositionCommand>(snake.get(), direction, snakeRenderer, static_cast<int>(QBertDirection::LEFT_DOWN));  //player 1 and direction
	controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonX, std::move(pMoveCommand), dae::ButtonState::Down);


	//y positive
	direction = { setSpeedX,setSpeedy,0.f };
	pMoveCommand = { std::make_unique<dae::SetPositionCommand>(snake.get(), direction,snakeRenderer,static_cast<int>(QBertDirection::RIGHT_DOWN)) };
	controller->MapCommandToButton(dae::Controller::ControllerButtons::ButtonA, std::move(pMoveCommand), dae::ButtonState::Down);



	auto keyboard{ input.GetKeyboard() };   //second uses keyboard 


	direction = { -setSpeedX,-setSpeedy,0.f };
	pMoveCommand = { std::make_unique<dae::SetPositionCommand>(snake.get(), direction,snakeRenderer,static_cast<int>(QBertDirection::LEFT_UP)) };
	keyboard->MapCommandToButton(SDL_SCANCODE_U, std::move(pMoveCommand), dae::ButtonState::Down);

	direction = { setSpeedX,-setSpeedy,0.f };
	pMoveCommand = { std::make_unique<dae::SetPositionCommand>(snake.get(), direction,snakeRenderer,static_cast<int>(QBertDirection::RIGH_UP)) };
	keyboard->MapCommandToButton(SDL_SCANCODE_O, std::move(pMoveCommand), dae::ButtonState::Down);

	direction = { -setSpeedX,setSpeedy,0.f };
	pMoveCommand = { std::make_unique<dae::SetPositionCommand>(snake.get(), direction,snakeRenderer,static_cast<int>(QBertDirection::LEFT_DOWN)) };
	keyboard->MapCommandToButton(SDL_SCANCODE_J, std::move(pMoveCommand), dae::ButtonState::Down);

	direction = { setSpeedX,setSpeedy,0.f };
	pMoveCommand = { std::make_unique<dae::SetPositionCommand>(snake.get(),direction,snakeRenderer,static_cast<int>(QBertDirection::RIGHT_DOWN)) };
	keyboard->MapCommandToButton(SDL_SCANCODE_L, std::move(pMoveCommand), dae::ButtonState::Down);



}
