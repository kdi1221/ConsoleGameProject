#pragma once

#include "Engine/Engine.h"
#include <memory>

class ConfigBase;
class Game : public Craft::Engine
{
public:
	Game();
	~Game() = default;

private:
	virtual std::unique_ptr<Craft::ConfigBase> CreateConfig() const override;
};

