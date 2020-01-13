#pragma once

class PhysicsObject
{
public:
    virtual ~PhysicsObject() = default;

    virtual void InitializePhysics() = 0;
    virtual void BeforeSimulationUpdate() = 0;
    virtual void AfterSimulationUpdate() = 0;
};
