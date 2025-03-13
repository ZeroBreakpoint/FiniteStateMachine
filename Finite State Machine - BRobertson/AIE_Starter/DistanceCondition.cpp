#include "state.h"

bool DistanceCondition::IsTrue(Agent* agent)
{
    if (agent->GetTarget() == nullptr) return false;

    float distance = glm::distance(agent->GetPosition(), agent->GetTarget()->GetPosition());

    if (m_lessThan) {
        return distance < m_distance;
    }
    else {
        return distance > m_distance;
    }
}