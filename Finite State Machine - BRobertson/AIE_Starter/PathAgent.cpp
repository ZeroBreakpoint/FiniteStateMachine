#include "NodeMap.h"
#include "PathAgent.h"
#include "raylib.h"
#include <algorithm>
#include <cfloat>
#include <iostream>

using namespace AIForGames;

void PathAgent::SetNode(AIForGames::Node* node)
{
    if (node == nullptr) {
        std::cerr << "Error: Attempted to set a null node." << std::endl;
        return;
    }

    m_currentNode = node;
    m_position = node->position;
}

void PathAgent::SetSpeed(float speed)
{
    m_speed = speed;
}

void PathAgent::Update(float deltaTime)
{
    if (m_path.empty()) return; // If path is empty, return

    Node* nextNode = m_path[m_currentIndex];
    if (nextNode == nullptr) {
        std::cerr << "Error: Next node in the path is null." << std::endl;
        return;
    }

    glm::vec2 direction = nextNode->position - m_position;
    float distance = glm::length(direction);

    if (distance == 0)
    {
        m_currentIndex++;
        return;
    }

    // Normalise the direction vector (unit vector)
    glm::vec2 unitDirection = glm::normalize(direction);

    // Subtract the distance covered this frame
    distance -= m_speed * deltaTime;

    if (distance > 0) {
        // If still moving towards the target node
        m_position += unitDirection * m_speed * deltaTime;
    }
    else {
        // Agent has overshot or reached the node
        m_currentIndex++;

        if (m_currentIndex >= m_path.size()) {
            // Agent has reached the end of the path, snap to the final node and clear the path
            m_position = nextNode->position;

            // Update the current node to the last node in the path
            m_currentNode = nextNode;

            m_path.clear();
        }
        else {
            // Move to the next node in the path
            m_position = nextNode->position;

            // Get the next target node
            Node* newNextNode = m_path[m_currentIndex];
            if (newNextNode == nullptr) {
                std::cerr << "Error: New next node is null." << std::endl;
                return;
            }

            // Calculate how far the Agent has overshot the node (invert the distance)
            float overshootDistance = -distance;

            // Calculate the new direction vector to the new next node
            glm::vec2 newDirection = newNextNode->position - nextNode->position;
            glm::vec2 newUnitDirection = glm::normalize(newDirection);

            // Move along this vector by the overshoot distance
            m_position += newUnitDirection * overshootDistance;
        }
    }
}

void PathAgent::GoToNode(AIForGames::Node* start, AIForGames::Node* end, NodeMap& nodeMap)
{
    if (end == nullptr) {
        std::cerr << "Error: Destination node is null." << std::endl;
        return;
    }

    if (start == end)
    {
        return;
    }

    // Use the position of the `start` node as the first argument to `DijkstrasSearch`
    glm::vec2 startPosition = start->position;

    // Calculate the path from the start position to the target node
    m_path = nodeMap.DijkstrasSearch(startPosition, end);

    // If the path is empty, there might be a problem with the connection between the nodes
    if (m_path.empty()) {
        std::cerr << "Error: No path found from start to end." << std::endl;
        std::cerr << "Start Node: (" << start->position.x << ", " << start->position.y << ")" << std::endl;
        std::cerr << "End Node: (" << end->position.x << ", " << end->position.y << ")" << std::endl;
        return;
    }

    m_currentIndex = 0;  // Reset the current index for following the new path
}

void PathAgent::Draw() const
{
    // Draw Agent - set size and color to yellow
    DrawCircle(static_cast<int>(m_position.x), static_cast<int>(m_position.y), 8, { 255, 255, 0, 255 });
}

void PathAgent::ClearPath()
{
    m_path.clear();  // Clear the path vector
}