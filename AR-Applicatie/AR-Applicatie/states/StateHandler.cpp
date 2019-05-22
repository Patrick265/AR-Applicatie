#include "StateHandler.h"
#include "GameState.h"
#include "DeathState.h"

StateHandler::StateHandler()
{
	current = nullptr;
}

StateHandler::~StateHandler()
{
	delete current;
}

void StateHandler::update(float elapsedTime) const
{
	current->update(elapsedTime);
}

void StateHandler::draw(std::map<std::string, Graphics::mesh>& meshes, std::map<std::string, uint16_t>& textures) const
{
	current->draw(meshes, textures);
}

void StateHandler::setState(const States state)
{
	switch (state)
	{
	case States::MENU:
		// TODO: MenuState
		break;
	case States::GAME:
		current = new GameState();
		break;
	case States::DEATH:
		current = new DeathState();
		break;
	}
}

State &StateHandler::getState() const
{
	return *current;
}
