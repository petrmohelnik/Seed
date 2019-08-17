#pragma once
#include "Script.h"

class GameScript : public Script
{
public:
    using Script::Script;

    void Update() override;

    glm::vec3 spherePosition;
    float xzPosition, yPosition;
    float increaseXZPosition = (2.0f * 3.14f) / 20.0f, increaseYPosition = (2.0f * 3.14f) / 10.0f;
};

void GameScript::Update()
{
    // Move sphere around the circle.
    transform->SetPosition(glm::vec3(6.0f * std::sin(xzPosition), 4.0f * std::sin(yPosition), 6.0f * std::cos(xzPosition)), Transform::Space::World);
    transform->Rotate(glm::vec3(3.0f * increaseXZPosition * time.DeltaTime(), 3.0f * increaseYPosition * time.DeltaTime(), 3.0f * increaseXZPosition * time.DeltaTime()), Transform::Space::World);

    // Update the rotating position.
    xzPosition += increaseXZPosition * time.DeltaTime();
    if (xzPosition > 2.0f * 3.14f)
    {
        xzPosition = xzPosition - 2.0f * 3.14f;
    }

    // Update the up/down position.
    yPosition += increaseYPosition * time.DeltaTime();
    if (yPosition > 3.14f)
    {
        yPosition = yPosition - 3.14f;
    }
}