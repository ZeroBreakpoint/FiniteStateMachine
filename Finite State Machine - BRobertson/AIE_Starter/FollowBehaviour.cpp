#include "FollowBehaviour.h"

using namespace AIForGames;

void FollowBehaviour::Update(Agent* agent, float deltaTime)
{
    Agent* target = agent->GetTarget();

    if (target)
    {
        glm::vec2 targetPosition = target->GetPosition();
        AIForGames::Node* closestNode = agent->GetNodeMap()->GetClosestNode(targetPosition);

        if (closestNode != nullptr) {
            const std::vector<AIForGames::Node*>& path = agent->GetPathAgent().GetPath();

            if (path.empty() || closestNode != path.back()) {
                agent->GoTo(closestNode->position);
            }
        }

        agent->GetPathAgent().Update(deltaTime);
    }
}

void FollowBehaviour::Enter(Agent* agent)
{
    // Red when following
    agent->SetColor({ 255, 0, 0, 255 });
    agent->Reset();
}