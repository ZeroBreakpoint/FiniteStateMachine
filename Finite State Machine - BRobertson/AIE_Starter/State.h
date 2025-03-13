#pragma once
#include <vector>
#include "Behaviour.h"
#include "Agent.h"
#include "WanderBehaviour.h"

class Condition
{
public:
    virtual bool IsTrue(Agent* agent) = 0;
};

class DistanceCondition : public Condition
{
private:
    float m_distance;
    bool m_lessThan;

public:
    DistanceCondition(float d, bool lt) : m_distance(d), m_lessThan(lt) {}
    virtual bool IsTrue(Agent* agent);
};


class State
{
public:
    // Struct for a state transition, which contains a condition and a target state
    struct Transition
    {
        Condition* condition;  // Pointer to the condition that triggers the transition
        State* targetState;    // Pointer to the target state if the condition is true
    };

private:
    std::vector<Behaviour*> m_behaviours;  // A list of behaviors associated with this state
    std::vector<Transition> m_transitions; // A list of transitions to other states

public:
    // Modified constructor to accept a Behaviour as an argument
    State(Behaviour* behaviour)
    {
        m_behaviours.push_back(behaviour);  // Add the provided behaviour to the state
    }

    ~State();

    // Function to add transitions to the state
    void AddTransition(Condition* condition, State* targetState)
    {
        m_transitions.push_back({ condition, targetState });
    }

    // Accessor for transitions
    const std::vector<Transition>& GetTransitions() const { return m_transitions; }

    // Virtual methods for entering, updating, and exiting the state
    virtual void Enter(Agent* agent);
    virtual void Update(Agent* agent, float deltaTime);
    virtual void Exit(Agent* agent);
};