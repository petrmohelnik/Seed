#pragma once

class Collider;
class MeshCollider;
struct ContactPoint;

class PhysicsEngine
{
public:
    void Raycast() {};

protected:
    friend class Components;
    friend class Collider;
    friend class DynamicCharacterController;

    void AddCollider(Collider* collider);

    void Initialize();
    void RunSimulationStep();
    void OnCollisionUpdate();
    void RigidbodyUpdate();
    void OnMouseUpdate();

    void CleanComponents();

    static btVector3 ToBtVector3(glm::vec3 const& vec3);
    static btQuaternion ToBtQuaternion(glm::quat const& quat);
    static glm::vec3 ToGlmVec3(btVector3 const& vector3);
    static glm::quat ToGlmQuat(btQuaternion const& quaternion);

private:
    btCollisionShape* CreateConvexHullShape(MeshCollider* meshCollider);
    btCollisionShape* CreateTriangleMeshShape(MeshCollider* meshCollider);
    btCollisionShape* CreateMeshCollisionShape(MeshCollider* meshCollider);
    btCollisionShape* CreateCollisionShape(Collider* collider);
    btVector3 CalculateLocalInertia(Collider* collider, btCollisionShape* btShape);
    btVector3 GetLocalScaling(Collider* collider);
    btTransform GetBtTransform(Collider* collider);
    void InitializeRigidbodyMaterial(Collider* collider);
    void CreateRigidbody(Collider* collider);
    void RefreshRigidbody(Collider* collider);
    void UpdateSimulationState();

    void InsertContactPoint(Collider* thisCollider, Collider* otherCollider, ContactPoint contactPoint);
    void AddContactManifold(btPersistentManifold* contactManifold);
    void ProcessCollisionExit(Collider* collider);
    void ProcessCollisionEnterAndStay(Collider* collider);
    void Simulate();

    std::vector<Collider*> colliders;

    std::unique_ptr<btCollisionConfiguration> collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> dispatcher;
    std::unique_ptr<btBroadphaseInterface> overlappingPairCache;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
    std::unique_ptr<btSoftRigidDynamicsWorld> dynamicsWorld;
};
