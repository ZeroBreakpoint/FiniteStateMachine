#pragma once
#include <iostream>
#include <algorithm>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "raylib.h"
#include "Pathfinding.h"
#include "NodeMap.h"
#include <cfloat>

class PathAgent
{
private:
    glm::vec2 m_position{ 0.0f, 0.0f };
    int m_currentIndex{ 0 };
    AIForGames::Node* m_currentNode{ nullptr };
    float m_speed{ 0.0f };

public:
    // Getter function for the path
    const std::vector<AIForGames::Node*>& GetPath() const { return m_path; }
    std::vector<AIForGames::Node*> m_path;
    void Update(float deltaTime);
    void GoToNode(AIForGames::Node* start, AIForGames::Node* end, NodeMap& nodeMap);
    void Draw() const;
    void ClearPath();
    void SetNode(AIForGames::Node* node);
    void SetSpeed(float speed);
    glm::vec2 GetPosition() const { return m_position; }
 };

