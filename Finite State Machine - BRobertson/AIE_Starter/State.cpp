#include "State.h"

State::~State()
{
    // Clean up the behaviours assigned to this state
    for (Behaviour* b : m_behaviours)
        delete b;

    // Clean up the Conditions in each Transition
    for (Transition t : m_transitions)
        delete t.condition;
}

void State::Enter(Agent* agent)
{
    // You can implement additional logic when entering this state

    // Check the type of behavior to update the agent's color accordingly
    if (dynamic_cast<WanderBehaviour*>(m_behaviours[0]))
    {
        // Yellow when wandering
        agent->SetColor({ 255, 255, 0, 255 });
    }
    else if (dynamic_cast<FollowBehaviour*>(m_behaviours[0]))
    {
        // Red when following
        agent->SetColor({ 255, 0, 0, 255 });
    }
}

void State::Update(Agent* agent, float deltaTime)
{
    // Update all behaviours attached to this state
    for (Behaviour* b : m_behaviours)
        b->Update(agent, deltaTime);
}

void State::Exit(Agent* agent)
{
    // You can implement additional logic when exiting this state
}