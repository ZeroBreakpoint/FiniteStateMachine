#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <raylib.h>
#include <cfloat>
#include "PathAgent.h"
#include "NodeMap.h"
#include "Behaviour.h"
#include "FollowBehaviour.h"
#include "FiniteStateMachine.h"

class Agent
{
public:
    Agent();
    Agent(NodeMap* _nodeMap, Behaviour* _behaviour) : m_current(_behaviour), m_nodeMap(_nodeMap), m_target(nullptr), m_color({ 255, 255, 0, 255 }) {};
    ~Agent();
    bool PathComplete() const;
    void Update(float deltaTime);
    void Draw();
    void GoTo(glm::vec2 point);
    void SetNode(AIForGames::Node* node) { m_pathAgent.SetNode(node); }
    void SetSpeed(float speed) { m_pathAgent.SetSpeed(speed); }
    void GoToNode(AIForGames::Node* node) { GoTo(node->position); }
    PathAgent& GetPathAgent() { return m_pathAgent; }
    NodeMap* GetNodeMap() const { return m_nodeMap; }
    Agent* GetTarget() const { return m_target; }
    glm::vec2 GetPosition() const { return m_pathAgent.GetPosition(); }
    void SetTarget(Agent* target) { m_target = target; }
    void SetColor(Color color) { m_color = color; }
    void Reset();

private:
    PathAgent m_pathAgent;
    Behaviour* m_current;
    NodeMap* m_nodeMap;
    Color m_color;
    Agent* m_target;
};

