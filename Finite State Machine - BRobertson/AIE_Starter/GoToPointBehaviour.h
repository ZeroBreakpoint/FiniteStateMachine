#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <raylib.h>
#include <cfloat>
#include "Behaviour.h"
#include "PathAgent.h"
#include "NodeMap.h"
#include "Pathfinding.h"
#include "Agent.h"
#include "WanderBehaviour.h"

class GoToPointBehaviour : public Behaviour
{
public:
    GoToPointBehaviour() : m_startSet(false), m_endSet(false), m_startNode(nullptr), m_endNode(nullptr) {}

    virtual void Update(Agent* agent, float deltaTime);

private:
    bool m_startSet;
    bool m_endSet;
    AIForGames::Node* m_startNode;
    AIForGames::Node* m_endNode;
};

