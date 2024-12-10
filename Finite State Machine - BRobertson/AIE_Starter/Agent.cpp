#include "Agent.h"
#include "Behaviour.h"

using namespace AIForGames;

void Agent::Update(float deltaTime)
{
    if (m_current)
        m_current->Update(this, deltaTime);

    m_pathAgent.Update(deltaTime);
}

void Agent::Draw()
{
    // Get the agent's current position from the PathAgent
    glm::vec2 position = m_pathAgent.GetPosition();

    // Set the radius of the agent's representation
    float radius = 8.0f;

    // Draw the agent as a circle on the screen using raylib's DrawCircle function
    DrawCircle(static_cast<int>(position.x), static_cast<int>(position.y), radius, m_color);
}

void Agent::GoTo(glm::vec2 point)
{
    // Find the closest node to the agent's current position (start node)
    AIForGames::Node* start = m_nodeMap->GetClosestNode(m_pathAgent.GetPosition());

    // Find the closest node to the clicked position (end node)
    AIForGames::Node* end = m_nodeMap->GetClosestNode(point);

    // If valid nodes are found for both start and end, initiate pathfinding
    if (start != nullptr && end != nullptr)
    {
        // Tell the PathAgent to move from its current node to the target node
        m_pathAgent.GoToNode(start, end, *m_nodeMap);
    }
}

void Agent::Reset()
{
    m_pathAgent.ClearPath();
}

Agent::Agent()
{

}

Agent::~Agent()
{
    delete m_current;
}

bool Agent::PathComplete() const
{
    return m_pathAgent.GetPath().empty();
}