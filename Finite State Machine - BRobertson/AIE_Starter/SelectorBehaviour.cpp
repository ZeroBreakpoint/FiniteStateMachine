#include "SelectorBehaviour.h"

using namespace AIForGames;


void SelectorBehaviour::Update(Agent* agent, float deltaTime)
{
    if (agent->GetTarget() == nullptr) {
        std::cout << "No target set for agent.\n";
        return; // Ensure there's a target to follow
    }

    // Calculate the distance between the agent and its target (player agent)
    glm::vec2 distanceVec = agent->GetPosition() - agent->GetTarget()->GetPosition();
    float distanceLength = glm::length(distanceVec);

    // Print the current distance for debugging
    std::cout << "Distance to target: " << distanceLength << std::endl;

    // Define proximity thresholds for switching behavior
    float followThreshold = agent->GetNodeMap()->GetCellSize() * 2.0f;  // Closer than this -> follow
    float wanderThreshold = agent->GetNodeMap()->GetCellSize() * 4.0f;  // Farther than this -> wander

    // Check if the agent should follow the player
    if (distanceLength < followThreshold) {
        std::cout << "Switching to FollowBehaviour. Distance: " << distanceLength << "\n";

        // Only switch if the agent is not already following
        if (m_selected != m_b1) {
            SetBehaviour(m_b1, agent);  // Switch to follow behavior
            agent->SetColor({ 255, 0, 0, 255 }); // Red when close
        }
    }
    // Check if the agent should wander
    else if (distanceLength > wanderThreshold) {
        std::cout << "Switching to WanderBehaviour. Distance: " << distanceLength << "\n";

        // Only switch if the agent is not already wandering
        if (m_selected != m_b2) {
            SetBehaviour(m_b2, agent);  // Switch to wander behavior
            agent->SetColor({ 0, 255, 255, 255 }); // Cyan when far
        }
    }

    // Debugging: print which behavior is currently active
    if (m_selected == m_b1) {
        std::cout << "Currently in FollowBehaviour.\n";
    }
    else if (m_selected == m_b2) {
        std::cout << "Currently in WanderBehaviour.\n";
    }

    // Let the selected behavior handle updates
    if (m_selected) {
        m_selected->Update(agent, deltaTime);
    }
}

void SelectorBehaviour::SetBehaviour(Behaviour* b, Agent* agent)
{
    if (m_selected != b) {

        // Switch the behaviour
        std::cout << "Changing behaviour...\n";
        m_selected = b;
        // Clear any existing path to prevent sudden teleporting between behaviors
        agent->Reset();
    }
}

SelectorBehaviour::~SelectorBehaviour()
{
    delete m_b1;
    delete m_b2;
}
