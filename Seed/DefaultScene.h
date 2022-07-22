#pragma once
#include "Object.h"
#include "PlayerObject.h"
#include "SkyboxSwitcherScript.h"
#include "LightScript.h"
#include "TargetScript.h"

void DefaultScene(Objects& objects, FileSystem& fileSystem)
{
    auto player = objects.CreateObject<PlayerObject>("player");

    auto light = objects.CreateObject("light");
    light->AddComponent<Light>();
    light->GetComponent<Light>()->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
    light->GetComponent<Light>()->SetPointLight(100.0f, 10.0f);
    light->GetComponent<Light>()->SetShadowNearPlaneDistance(0.2f);
    light->GetComponent<Light>()->SetShadowFarPlaneDistance(30.0f);
    light->GetComponent<Transform>()->Translate(glm::vec3(-2.0, 2.0, 5.0), Transform::Space::World);
    light->GetComponent<Transform>()->SetScale(0.2f);
    light->AddComponent<MeshRenderer>()->Load("sphere.obj");
    light->GetComponent<MeshRenderer>()->GetMaterial()->Emission->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
    light->AddComponent<SphereCollider>(glm::vec3(0.0f), 1.0f);
    light->GetComponent<Collider>()->SetBounciness(2.0f);
    light->GetComponent<Collider>()->SetFriction(0.9f);
    light->GetComponent<Collider>()->SetMass(10.0f);

    objects.SetSkybox(fileSystem.LoadCubeMapHDR("Newport_Loft.hdr"));
    auto skyboxSwitcher = objects.CreateObject("skyboxSwitcher")->AddComponent<SkyboxSwitcherScript>();
    skyboxSwitcher->AddSkybox("Newport_Loft.hdr");
    skyboxSwitcher->AddSkybox("sunrise.hdr");
    skyboxSwitcher->AddSkybox("Space.hdr");
    skyboxSwitcher->AddSkybox("Bunker.hdr");
    skyboxSwitcher->AddSkybox("Protospace.hdr");
    skyboxSwitcher->AddSkybox("Milkyway.hdr");

    objects.CreateObjectsFromScene("scene/scene.gltf", "Scene");
    
    auto spheres = objects.CreateObjectWithMesh("spheres", "MetalRoughSpheres/scene.gltf", glm::vec3(5.0f, 0.0f, 10.0f));
    spheres->GetComponent<MeshRenderer>()->GetSharedMaterial()->UseOcclusionMap();

 /*   auto cobblestoneMaterial = std::make_shared<Material>();
    cobblestoneMaterial->Albedo = fileSystem.LoadTexture("cobblestone/albedo.png", 24, 24);
    cobblestoneMaterial->Normal = fileSystem.LoadTexture("cobblestone/normal.png", 24, 24);
    cobblestoneMaterial->Height = fileSystem.LoadTexture("cobblestone/height.png", 8, 8);
    cobblestoneMaterial->Metallic = fileSystem.LoadTexture("cobblestone/specular.png", 24, 24);
    cobblestoneMaterial->Metallic->AddChannelFromTexture(fileSystem.LoadTexture("cobblestone/shininess.png", 8, 8), 0);
    cobblestoneMaterial->SetSpecularWorkflow();
    cobblestoneMaterial->SetParallaxStrength(0.1f);

    auto cobblestone = objects.CreateObject("cobblestone");
    cobblestone->AddComponent<MeshRenderer>()->Load("cube.dae");
    cobblestone->GetComponent<MeshRenderer>()->SetMaterial(0, cobblestoneMaterial);
    cobblestone->GetComponent<Transform>()->SetScale(glm::vec3(0.5f));
    cobblestone->GetComponent<Transform>()->Translate(glm::vec3(3.0f, 0.5f, 7.0f));
    cobblestone->AddComponent<BoxCollider>(glm::vec3(0.0f), glm::vec3(1.0f));
    cobblestone->GetComponent<BoxCollider>()->SetBounciness(0.1f);
    cobblestone->GetComponent<BoxCollider>()->SetFriction(0.55f);
    cobblestone->GetComponent<BoxCollider>()->SetMass(100.0f);

    auto redbricksMaterial = std::make_shared<Material>();
    redbricksMaterial->Albedo = fileSystem.LoadTexture("redbricks/albedo.png", 24, 24);
    redbricksMaterial->Normal = fileSystem.LoadTexture("redbricks/normal.png", 24, 24);
    redbricksMaterial->Height = fileSystem.LoadTexture("redbricks/height.png", 8, 8);
    redbricksMaterial->Metallic = fileSystem.LoadTexture("redbricks/ao.png", 8, 8);
    redbricksMaterial->Metallic->AddChannelFromTexture(fileSystem.LoadTexture("redbricks/roughness.png", 8, 8), 0);
    redbricksMaterial->Metallic->AddChannel(0.0);
    redbricksMaterial->Metallic->SetIsRGBOrder();
    redbricksMaterial->SetParallaxStrength(0.1f);

    auto redbricks = objects.CreateObject("redbricks");
    redbricks->AddComponent<MeshRenderer>()->Load("cube.dae");
    redbricks->GetComponent<MeshRenderer>()->SetMaterial(0, redbricksMaterial);
    redbricks->GetComponent<Transform>()->Translate(glm::vec3(1.9f, 0.5f, 7.0f));
    redbricks->GetComponent<Transform>()->SetScale(glm::vec3(0.5f));
    redbricks->AddComponent<BoxCollider>(glm::vec3(0.0f), glm::vec3(1.0f));
    redbricks->GetComponent<BoxCollider>()->SetBounciness(0.1f);
    redbricks->GetComponent<BoxCollider>()->SetFriction(0.55f);
    redbricks->GetComponent<BoxCollider>()->SetMass(100.0f);

    auto roughblockMaterial = std::make_shared<Material>();
    roughblockMaterial->Albedo = fileSystem.LoadTexture("roughblock/albedo.png", 24, 24);
    roughblockMaterial->Normal = fileSystem.LoadTexture("roughblock/normal.png", 24, 24);
    roughblockMaterial->Height = fileSystem.LoadTexture("roughblock/height.png", 8, 8);
    roughblockMaterial->Metallic = fileSystem.LoadTexture("roughblock/ao.png", 8, 8);
    roughblockMaterial->Metallic->AddChannelFromTexture(fileSystem.LoadTexture("roughblock/roughness.png", 8, 8), 0);
    roughblockMaterial->Metallic->AddChannel(0.0);
    roughblockMaterial->Metallic->SetIsRGBOrder();
    roughblockMaterial->SetParallaxStrength(0.1f);

    auto roughblock = objects.CreateObject("roughblock");
    roughblock->AddComponent<MeshRenderer>()->Load("cube.dae");
    roughblock->GetComponent<MeshRenderer>()->SetMaterial(0, roughblockMaterial);
    roughblock->GetComponent<Transform>()->Translate(glm::vec3(2.5f, 1.6f, 7.0f));
    roughblock->GetComponent<Transform>()->SetScale(glm::vec3(0.5f));
    roughblock->AddComponent<BoxCollider>(glm::vec3(0.0f), glm::vec3(1.0f));
    roughblock->GetComponent<BoxCollider>()->SetBounciness(0.1f);
    roughblock->GetComponent<BoxCollider>()->SetFriction(0.55f);
    roughblock->GetComponent<BoxCollider>()->SetMass(100.0f);*/

    auto crashDummies = objects.GetObjectsByName([](std::string const& name) { return name.rfind("CrashDummy", 0) == 0;});
    for (auto& crashDummy : crashDummies)
    {
        crashDummy->AddComponent<TargetScript>();
        crashDummy->GetComponent<TargetScript>()->TargetPath = "crash_dummy/crash_dummy_broken.gltf";
        crashDummy->GetComponent<TargetScript>()->TargetName = "CrashDummyBroken";
    }

    for (auto& light : objects.GetComponents<Light>())
    {
        if (light->GetObject()->GetName() == "FlashLight")
            continue;
        if (!light->GetObject()->GetComponent<Collider>())
        {
            light->GetObject()->AddComponent<SphereCollider>(glm::vec3(0.0f), 0.4f / glm::length(light->GetTransform()->GetScale()));
            light->GetObject()->GetComponent<Collider>()->SetIsTrigger(true);
            light->GetObject()->GetComponent<Collider>()->SetIsKinematic(true);
        }
        light->GetObject()->AddComponent<LightScript>();
    }

    for (int i = 0; i < 5; i++)
    {
        float xPos = (i % 5) * 0.4f - 3.0f;
        float yPos = (i / 5) * 0.4f + 0.4f;
        auto sphere = objects.CreateObjectWithMesh("sphere", "sphere_flat.obj", glm::vec3(xPos, yPos, 1.0f));
        sphere->GetComponent<MeshRenderer>()->SetMaterial(0, sphere->GetComponent<MeshRenderer>()->GetSharedMaterial()->Copy());
        sphere->GetComponent<MeshRenderer>()->GetSharedMaterial()->SetSpecularWorkflow();
        sphere->GetComponent<MeshRenderer>()->GetSharedMaterial()->Metallic->SetColor(
            glm::vec4(0.2f, 0.2f, 0.2f, i * 0.25f));
        sphere->GetComponent<MeshRenderer>()->GetSharedMaterial()->Albedo->SetColor(glm::vec4(1.0f));
        sphere->GetComponent<Transform>()->SetScale(0.2f);
        sphere->AddComponent<SphereCollider>(glm::vec3(0.0f), 1.0f);
        sphere->GetComponent<SphereCollider>()->SetBounciness(0.8f);
        sphere->GetComponent<SphereCollider>()->SetFriction(0.9f);
        sphere->GetComponent<Collider>()->SetMass(10.0f);
    }
    for (int i = 0; i < 15; i++)
    {
        float xPos = (i % 5) * 0.4f - 3.0f;
        float yPos = (i / 5) * 0.4f + 0.8f;
        auto sphere = objects.CreateObjectWithMesh("sphere", "sphere.obj", glm::vec3(xPos, yPos, 1.0f));
        sphere->GetComponent<MeshRenderer>()->SetMaterial(0, sphere->GetComponent<MeshRenderer>()->GetSharedMaterial()->Copy());
        sphere->GetComponent<MeshRenderer>()->GetSharedMaterial()->SetMetallicWorkflow();
        sphere->GetComponent<MeshRenderer>()->GetSharedMaterial()->Metallic->SetColor(
            glm::vec3(1.0f, ((14 - i) / 5) * 0.5, (i % 5) * 0.25f));
        sphere->GetComponent<MeshRenderer>()->GetSharedMaterial()->Albedo->SetColor(glm::vec4(1.0f));
        sphere->GetComponent<Transform>()->SetScale(0.2f);
        sphere->AddComponent<SphereCollider>(glm::vec3(0.0f), 1.0f);
        sphere->GetComponent<SphereCollider>()->SetBounciness(0.8f);
        sphere->GetComponent<SphereCollider>()->SetFriction(0.9f);
        sphere->GetComponent<Collider>()->SetMass(10.0f);
    }

    RenderingPipeline::SetMainCamera(objects.GetObjectByName("PlayerCamera")->GetComponent<Camera>());
}
