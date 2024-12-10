#pragma once
#include <vector>
#include "raylib.h"
#include <glm/glm.hpp>
#include "Agent.h"
#include "NodeMap.h"
#include "Behaviour.h"
#include "PathAgent.h"

class FollowBehaviour : public Behaviour
{
private:
	glm::vec2 lastTargetPosition{ 0.0f, 0.0f };

public:
	void Update(Agent* agent, float deltaTime);
	void Enter(Agent* agent);
};

