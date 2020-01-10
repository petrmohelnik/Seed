#pragma once
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"

class Collider;
class BoxCollider;
class CapsuleCollider;
class SphereCollider;
class MeshCollider;

class PhysicsEngine
{
public:
    void Raycast() {};

protected:
    friend class Components;

    void AddCollider(Collider* collider);

    void Initialize();
    void RunSimulationStep();
    void OnCollisionUpdate();
    void RigidbodyUpdate();

    void CleanComponents();

private:
    btCollisionShape* CreateMeshCollisionShape(MeshCollider* meshCollider);
    void UpdateSimulationState();
    void Simulate();

    std::vector<Collider*> colliders;

    std::unique_ptr<btCollisionConfiguration> collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> dispatcher;
    std::unique_ptr<btBroadphaseInterface> overlappingPairCache;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
    std::unique_ptr<btSoftRigidDynamicsWorld> dynamicsWorld;
};
