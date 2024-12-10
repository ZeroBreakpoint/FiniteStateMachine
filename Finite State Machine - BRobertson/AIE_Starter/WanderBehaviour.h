#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <raylib.h>
#include <cfloat>
#include "Behaviour.h"
#include "Agent.h"
#include "NodeMap.h"
#include "PathAgent.h"
#include "Pathfinding.h"

class WanderBehaviour : public Behaviour
{
public:
    WanderBehaviour() : m_lastNode(nullptr) {}

    virtual void Update(Agent* agent, float deltaTime);

    void Enter(Agent* agent);

private:
    AIForGames::Node* m_lastNode; // Track the last node visited
};