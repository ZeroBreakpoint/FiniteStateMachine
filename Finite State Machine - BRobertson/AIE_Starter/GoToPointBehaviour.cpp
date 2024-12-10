#include "GoToPointBehaviour.h"

using namespace AIForGames;

void GoToPointBehaviour::Update(Agent* agent, float deltaTime)
{
    // Handle left click to set the start node
    if (IsMouseButtonPressed(0))
    {
        Vector2 mousePos = GetMousePosition();
        m_startNode = agent->GetNodeMap()->GetClosestNode(glm::vec2(mousePos.x, mousePos.y));

        // If a valid start node is found, set it
        if (m_startNode)
        {
            m_startSet = true;
            agent->SetNode(m_startNode); // Set agent's current node to the start node
        }
    }

    // Handle right click to set the end node
    if (IsMouseButtonPressed(1))
    {
        Vector2 mousePos = GetMousePosition();
        m_endNode = agent->GetNodeMap()->GetClosestNode(glm::vec2(mousePos.x, mousePos.y));

        // If both the start and end nodes are valid, move the agent
        if (m_startSet && m_endNode)
        {
            agent->GoToNode(m_endNode);
            m_endSet = true;
        }
    }

    // Let the agent’s path agent handle movement frame by frame
    agent->GetPathAgent().Update(deltaTime);
}