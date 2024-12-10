#pragma once
#include <glm/glm.hpp>
#include "raylib.h"


class Agent;

class Behaviour
{
	public:
		virtual void Update(Agent* agent, float deltaTime) = 0;
};

