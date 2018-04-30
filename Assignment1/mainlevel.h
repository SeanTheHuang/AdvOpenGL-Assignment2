#pragma once

#include "level.h"

class MainLevel : public Level
{
public:
	//Fill level with objects. Initialize all objects
	void Initialize();
	//Update all objects. Apply level logic
	void Update();

private:
};