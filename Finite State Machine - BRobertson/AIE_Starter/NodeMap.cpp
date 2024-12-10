#include <glm/glm.hpp>
#include "NodeMap.h"
#include "Pathfinding.h"
#include <iostream>
#include <algorithm>
#include <cfloat>
#include "raylib.h"

using namespace AIForGames;

// Constructor definition (with member initialisations)
NodeMap::NodeMap() : m_width(0), m_height(0), m_cellSize(0), m_nodes(nullptr) {}

NodeMap::~NodeMap() {
    // Clean up allocated memory
    if (m_nodes) {
        for (int i = 0; i < m_width * m_height; i++) {
            delete m_nodes[i];
        }
        delete[] m_nodes;
    }
}

void NodeMap::Initialise(std::vector<std::string> asciiMap, int cellSize)
{
    m_cellSize = static_cast<float>(cellSize);  // Ensure m_cellSize is a float
    const char emptySquare = '0';

    // Set map dimensions based on the ASCII input
    m_height = static_cast<int>(asciiMap.size());
    m_width = static_cast<int>(asciiMap[0].size());

    // Allocate memory for nodes
    m_nodes = new Node * [m_width * m_height]();

    for (int y = 0; y < m_height; y++) {
        std::string& line = asciiMap[y];

        // Check for mismatched line lengths
        if (line.size() != static_cast<size_t>(m_width)) {
            std::cout << "Mismatched line #" << y << " in ASCII map ("
                << line.size() << " instead of " << m_width << ")" << std::endl;
        }

        for (int x = 0; x < m_width; x++) {
            char tile = x < static_cast<int>(line.size()) ? line[x] : emptySquare;

            // Create nodes for tiles that are not empty
            m_nodes[x + m_width * y] = tile == emptySquare ? nullptr
                : new Node((static_cast<float>(x) + 0.5f) * m_cellSize,
                    (static_cast<float>(y) + 0.5f) * m_cellSize);
        }
    }

    // Establish connections between adjacent nodes
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            Node* node = GetNode(x, y);
            if (node) {
                Node* nodeWest = x == 0 ? nullptr : GetNode(x - 1, y);
                if (nodeWest) {
                    node->ConnectTo(nodeWest, 1);
                    nodeWest->ConnectTo(node, 1);
                }

                Node* nodeSouth = y == 0 ? nullptr : GetNode(x, y - 1);
                if (nodeSouth) {
                    node->ConnectTo(nodeSouth, 1);
                    nodeSouth->ConnectTo(node, 1);
                }
            }
        }
    }
}

Node* NodeMap::GetNode(int x, int y)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return nullptr;
    return m_nodes[x + m_width * y];
}

void NodeMap::Draw()
{
    // Colours for nodes and connections
    Color cellColor{ 255, 0, 0, 255 };    // Red for blocks
    Color lineColor{ 128, 128, 128, 255 };  // Grey for connections

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            Node* node = GetNode(x, y);
            if (node == nullptr) {
                DrawRectangle(
                    static_cast<int>(x * m_cellSize),
                    static_cast<int>(y * m_cellSize),
                    static_cast<int>(m_cellSize - 1),
                    static_cast<int>(m_cellSize - 1),
                    cellColor
                );
            }
            else {
                // Draw connections between nodes
                for (size_t i = 0; i < node->connections.size(); i++) {
                    Node* other = node->connections[i].target;
                    DrawLine(
                        static_cast<int>(node->position.x),
                        static_cast<int>(node->position.y),
                        static_cast<int>(other->position.x),
                        static_cast<int>(other->position.y),
                        lineColor
                    );
                }
            }
        }
    }
}

std::vector<Node*> NodeMap::DijkstrasSearch(glm::vec2 startPos, Node* endNode)
{
    Node* startNode = GetClosestNode(startPos);
    if (startNode == nullptr || endNode == nullptr) {
        std::cerr << "Error: Start or End node is null." << std::endl;
        return std::vector<Node*>();
    }

    if (startNode == endNode) {
        return std::vector<Node*>(); // Return empty path if start == end
    }

    startNode->gScore = 0;
    startNode->previous = nullptr;

    std::vector<Node*> openList;
    std::vector<Node*> closedList;

    openList.push_back(startNode);

    while (!openList.empty()) {
        // Sort openList by gScore
        std::sort(openList.begin(), openList.end(), [](Node* a, Node* b) {
            return a->gScore < b->gScore;
            });

        Node* currentNode = openList.front();
        if (currentNode == endNode) break;

        openList.erase(openList.begin());
        closedList.push_back(currentNode);

        for (Edge& connection : currentNode->connections) {
            Node* targetNode = connection.target;

            if (std::find(closedList.begin(), closedList.end(), targetNode) == closedList.end()) {
                float gScore = currentNode->gScore + connection.cost;

                if (std::find(openList.begin(), openList.end(), targetNode) == openList.end()) {
                    targetNode->gScore = gScore;
                    targetNode->previous = currentNode;
                    openList.push_back(targetNode);
                }
                else if (gScore < targetNode->gScore) {
                    targetNode->gScore = gScore;
                    targetNode->previous = currentNode;
                }
            }
        }
    }

    std::vector<Node*> path;
    Node* currentNode = endNode;

    while (currentNode != nullptr) {
        path.insert(path.begin(), currentNode);
        currentNode = currentNode->previous;
    }

    // Remove redundant first node if it doubles back
    if (path.size() > 1) {
        glm::vec2 agentDirection = glm::normalize(startPos - path[0]->position);
        glm::vec2 nodeDirection = glm::normalize(path[1]->position - path[0]->position);

        if (glm::dot(agentDirection, nodeDirection) > 0.99f) { // Check direction similarity
            path.erase(path.begin());
        }
    }

    return path;
}

void NodeMap::DrawPath(const std::vector<Node*>& path, Color lineColor)
{
    for (size_t i = 1; i < path.size(); i++) {
        Node* nodeA = path[i - 1];
        Node* nodeB = path[i];

        if (nodeA && nodeB) {
            DrawLine(
                static_cast<int>(nodeA->position.x),
                static_cast<int>(nodeA->position.y),
                static_cast<int>(nodeB->position.x),
                static_cast<int>(nodeB->position.y),
                lineColor
            );
        }
    }
}

Node* NodeMap::GetClosestNode(glm::vec2 worldPos)
{
    int i = static_cast<int>(worldPos.x / m_cellSize);
    int j = static_cast<int>(worldPos.y / m_cellSize);

    if (i < 0 || i >= m_width || j < 0 || j >= m_height) {
        std::cerr << "Error: Clicked position is out of bounds: ("
            << worldPos.x << ", " << worldPos.y << ")" << std::endl;
        return nullptr;
    }

    return GetNode(i, j);
}

Node* NodeMap::GetRandomNode()
{
    Node* node = nullptr;
    while (node == nullptr) {
        int x = rand() % m_width;
        int y = rand() % m_height;
        node = GetNode(x, y);
    }
    return node;
}