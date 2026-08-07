#include "Game.h"
#include "Config/Config.h"

Game::Game()
{

}

std::unique_ptr<Craft::ConfigBase> Game::CreateConfig() const
{
	return std::make_unique<Config>();
}