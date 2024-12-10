#include "Pathfinding.h"

using namespace AIForGames;

void AIForGames::Node::ConnectTo(Node* other, float cost)
{
	connections.push_back(Edge(other, cost));
}
