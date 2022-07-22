#pragma once
#include "ObjectRotationScript.h"
#include "CameraAroundObjectScript.h"

void HeightMapScene(Objects& objects, FileSystem& fileSystem)
{
    auto camera = objects.CreateObject("camera");
    camera->AddComponent<Camera>();
    camera->GetComponent<Transform>()->SetPosition(glm::vec3(0.0f, 5.0f, 5.0f));
    camera->AddComponent<CameraAroundObjectScript>();
    RenderingPipeline::SetMainCamera(camera->GetComponent<Camera>());

    auto light = objects.CreateObject("light");
    light->AddComponent<Light>();
    light->GetComponent<Light>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
    light->GetComponent<Light>()->SetPointLight(100.0f, 1000.0f);
    light->GetComponent<Light>()->SetShadowNearPlaneDistance(0.2f);
    light->GetComponent<Light>()->SetShadowFarPlaneDistance(30.0f);
    light->GetComponent<Transform>()->Translate(glm::vec3(5.0, 5.0, 5.0), Transform::Space::World);
    //light->GetComponent<Transform>()->SetScale(0.2f);
    //light->AddComponent<MeshRenderer>()->Load("sphere.obj");
    //light->GetComponent<MeshRenderer>()->GetMaterial()->Emission->SetColor(glm::vec3(1.0f, 1.0f, 0.95f));
    //light->AddComponent<SphereCollider>(glm::vec3(0.0f), 1.0f);
    //light->GetComponent<SphereCollider>()->SetIsKinematic(true);

    auto heightMap = objects.CreateObjectWithMesh("heightMap", "cube.dae", glm::vec3(0.0f, 0.0f, 0.0f));
    heightMap->GetComponent<MeshRenderer>()->GetSharedMaterial()->Albedo->SetColor(glm::vec4(1.0f));
    heightMap->GetComponent<MeshRenderer>()->GetSharedMaterial()->Emission->SetColor(glm::vec4(0.1f));
    //heightMap->GetComponent<Transform>()->SetScale(0.5f);
    heightMap->AddComponent<BoxCollider>(glm::vec3(0.0f), glm::vec3(1.0f));
    heightMap->GetComponent<BoxCollider>()->SetIsKinematic(true);
    heightMap->AddComponent<ObjectRotationScript>();
}