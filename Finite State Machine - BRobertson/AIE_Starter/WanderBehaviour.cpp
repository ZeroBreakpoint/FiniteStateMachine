#include "WanderBehaviour.h"
#include <cstdlib>
#include <ctime>

using namespace AIForGames;

void WanderBehaviour::Update(Agent* agent, float deltaTime)
{
    if (agent->PathComplete()) {
        AIForGames::Node* randomNode = agent->GetNodeMap()->GetRandomNode();
        agent->GoToNode(randomNode);
    }

    agent->GetPathAgent().Update(deltaTime);
}

void WanderBehaviour::Enter(Agent* agent)
{
    // Yellow when wandering
    agent->SetColor({ 255, 255, 0, 255 });
    agent->Reset();
}