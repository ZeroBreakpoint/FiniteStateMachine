#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raylib.h"
#include "Pathfinding.h"
#include "NodeMap.h"
#include "PathAgent.h"
#include <string>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include "Agent.h"
#include "Behaviour.h"
#include "WanderBehaviour.h"
#include "FollowBehaviour.h"
#include "FiniteStateMachine.h"
#include "State.h"
#include <cstdlib>
#include <ctime>

using namespace AIForGames;

int main(int argc, char* argv[])
{
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Finite State Machine - Assessment 3 - Bradley Robertson");

    SetTargetFPS(60);
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(0)));

    std::vector<std::string> asciiMap = {
        "000000000000",
        "010111011100",
        "010101110110",
        "011100000010",
        "010111111110",
        "010000001000",
        "011111111110",
        "000000000000"
    };

    NodeMap nodeMap;
    nodeMap.Initialise(asciiMap, 50);

    // Create first agent with GoToPointBehaviour (player controlled)
    Agent agent1(&nodeMap, nullptr);
    agent1.SetSpeed(128);
    agent1.SetColor({ 0, 255, 0, 255 });  // Set to green

    // Create second agent with WanderBehaviour (random movement)
    Agent agent2(&nodeMap, new WanderBehaviour());
    agent2.SetNode(nodeMap.GetRandomNode());  // Start at a random node
    agent2.SetSpeed(64);
    agent2.SetColor({ 0, 0, 255, 255 });  // Set to blue

    DistanceCondition* closerThanThreshold = new DistanceCondition(90.0f, true);  // Follow if closer than 150 units
    DistanceCondition* furtherThanThreshold = new DistanceCondition(120.0f, false);  // Wander if farther than 200 units

    // register these states with the FSM, so its responsible for deleting them now
    State* wanderState = new State(new WanderBehaviour());
    State* followState = new State(new FollowBehaviour());
    wanderState->AddTransition(closerThanThreshold, followState);
    followState->AddTransition(furtherThanThreshold, wanderState);

    // make a finite state machine that starts off wandering
    FiniteStateMachine* fsm = new FiniteStateMachine(wanderState);
    fsm->AddState(wanderState);
    fsm->AddState(followState);



    Agent agent3(&nodeMap, fsm);
    agent3.SetNode(nodeMap.GetRandomNode());
    agent3.SetTarget(&agent1);
    agent3.SetSpeed(32);
    agent3.SetColor({ 255, 255, 0, 255 });  // Initially set to yellow for wandering

    while (!WindowShouldClose())
    {
        if (IsMouseButtonPressed(0))  // Left click to set start node
        {
            Vector2 mousePos = GetMousePosition();
            AIForGames::Node* startNode = nodeMap.GetClosestNode(glm::vec2(mousePos.x, mousePos.y));

            if (startNode) {
                agent1.SetNode(startNode); // Update agent1's starting node
            }
            else {
                std::cerr << "Failed to find a valid start node." << std::endl;
            }
        }

        if (IsMouseButtonPressed(1))  // Right click to set goal node
        {
            Vector2 mousePos = GetMousePosition();
            AIForGames::Node* endNode = nodeMap.GetClosestNode(glm::vec2(mousePos.x, mousePos.y));

            if (endNode) {
                agent1.GoTo(endNode->position); // Move agent1 to the target node
            }
            else {
                std::cerr << "Failed to find a valid end node." << std::endl;
            }
        }



        float deltaTime = GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);

        nodeMap.Draw();  // Draw the Node Map

        agent1.Update(deltaTime);
        agent1.Draw();   // Draw the first agent (player controlled)

        agent2.Update(deltaTime);
        agent2.Draw();   // Draw the second agent (wandering)

        agent3.Update(deltaTime);
        agent3.Draw();  // Draw the third agent (following)


        EndDrawing();
    }

    CloseWindow();

    return 0;
}