#include "FiniteStateMachine.h"
#include "State.h"

FiniteStateMachine::~FiniteStateMachine()
{
    for (State* state : m_states)
    {
        delete state;
    }
}

void FiniteStateMachine::Update(Agent* agent, float deltaTime)
{
    State* newState = nullptr;

    // Check for transitions in the current state
    for (const State::Transition& transition : m_currentState->GetTransitions())
    {
        if (transition.condition->IsTrue(agent))
        {
            newState = transition.targetState;
            break;  // If a transition condition is met, break out of the loop
        }
    }

    // If a new state is found, transition to the new state
    if (newState != nullptr && newState != m_currentState)
    {
        m_currentState->Exit(agent);      // Exit the current state
        m_currentState = newState;        // Change to the new state
        m_currentState->Enter(agent);     // Enter the new state (this will change color)
    }

    // Update the current state
    m_currentState->Update(agent, deltaTime);
}

void FiniteStateMachine::Enter(Agent* agent)
{
    if (m_currentState)
    {
        m_currentState->Enter(agent);
    }
}