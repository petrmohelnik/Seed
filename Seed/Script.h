#pragma once
#include "Component.h"

class Input;
class Time;
class Objects;
class PhysicsEngine;
class FileSystem;

struct Collision;

class Script : public Component
{
public:
    Script(Object* object, PhysicsEngine& physics);
    Script(Script& script);
    virtual ~Script() = default;

protected:
    friend class Components;
    friend class PhysicsEngine;
    friend class Object;

    virtual void OnCreate() {};
    virtual void FixedUpdate() {};
    virtual void OnCollisionEnter(Collision const& collision) {};
    virtual void OnCollisionStay(Collision const& collision) {};
    virtual void OnCollisionExit(Collision const& collision) {};
    virtual void OnMouseEnter() {};
    virtual void OnMouseOver() {};
    virtual void OnMouseExit() {};
    virtual void OnMouseDown() {};
    virtual void OnMouseDrag() {};
    virtual void OnMouseUp() {};
    virtual void Update() {};
    virtual void OnEnable() {};
    virtual void OnDisable() {};
    virtual void OnDestroy() {};

    Input& input;
    Time& time;
    Objects& objects;
    PhysicsEngine& physics;
    FileSystem& fileSystem;
    Transform* transform;

private:
    bool initialized = false;
};