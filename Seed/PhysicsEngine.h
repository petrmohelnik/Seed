#pragma once

class Collider;
class MeshCollider;
class Script;
struct ContactPoint;

class PhysicsEngine
{
public:
    struct RaycastHit
    {
        Collider* Collider;
        glm::vec3 Point;
        glm::vec3 Normal;
    };

    bool Raycast(glm::vec3 fromPosition, glm::vec3 direction, RaycastHit& hitInfo, float maxDistance = 1000.0f);
    bool Raycast(glm::vec3 fromPosition, glm::vec3 direction, float maxDistance = 1000.0f);

protected:
    friend class Components;
    friend class Collider;
    friend class DynamicCharacterController;
    friend class Camera;

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
    btTriangleIndexVertexArray* CreateTriangleArray(MeshCollider* meshCollider, int submeshIndex);
    btCollisionShape* CreateGImpactShape(MeshCollider* meshCollider, int submeshIndex);
    btCollisionShape* CreateConvexHullShape(MeshCollider* meshCollider, int submeshIndex);
    btCollisionShape* CreateTriangleMeshShape(MeshCollider* meshCollider, int submeshIndex);
    btCollisionShape* CreateMeshCollisionSubShape(MeshCollider* meshCollider, int submeshIndex);
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
    Collider* ClosestColliderUnderMouse();
    void Simulate();

    void CallScriptCallbackOnCollider(Collider* collider, std::function<void(Script*)> scriptCallback);

    std::vector<Collider*> colliders;
    Collider* previousColliderUnderMouse = nullptr;
    bool mouseDown = false;

    std::unique_ptr<btCollisionConfiguration> collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> dispatcher;
    std::unique_ptr<btBroadphaseInterface> overlappingPairCache;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
    std::unique_ptr<btSoftRigidDynamicsWorld> dynamicsWorld;
};
