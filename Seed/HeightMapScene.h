#pragma once
#include "ObjectRotationScript.h"
#include "CameraAroundObjectScript.h"
#include "HeightMapSettingsScript.h"

void HeightMapScene(Objects& objects, FileSystem& fileSystem)
{
    auto camera = objects.CreateObject("camera");
    camera->AddComponent<Camera>();
    camera->GetComponent<Transform>()->SetPosition(glm::vec3(0.0f, 3.0f, 1.5f));
    camera->AddComponent<CameraAroundObjectScript>();
    RenderingPipeline::SetMainCamera(camera->GetComponent<Camera>());

    auto light = objects.CreateObject("light");
    light->AddComponent<Light>();
    light->GetComponent<Light>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
    light->GetComponent<Light>()->SetPointLight(100.0f, 5000.0f);
    light->GetComponent<Light>()->SetShadowNearPlaneDistance(0.2f);
    light->GetComponent<Light>()->SetShadowFarPlaneDistance(30.0f);
    //light->GetComponent<Light>()->SetIsShadowCaster(false);
    light->GetComponent<Transform>()->Translate(glm::vec3(5.0, 5.0, 5.0), Transform::Space::World);
    //light->GetComponent<Transform>()->SetScale(0.2f);
    //light->AddComponent<MeshRenderer>()->Load("sphere.obj");
    //light->GetComponent<MeshRenderer>()->GetMaterial()->Emission->SetColor(glm::vec3(1.0f, 1.0f, 0.95f));
    //light->AddComponent<SphereCollider>(glm::vec3(0.0f), 1.0f);
    //light->GetComponent<SphereCollider>()->SetIsKinematic(true);

    //auto heightMapTes = objects.CreateObjectWithMesh("heightMapTes", "plane.obj", glm::vec3(2.0f, 0.0f, 0.0f), false);
    //heightMapTes->GetComponent<MeshRenderer>()->GetSharedMesh()->Tessellation = true;
    //heightMapTes->GetComponent<MeshRenderer>()->GetSharedMesh()->Unload();
    //heightMapTes->GetComponent<MeshRenderer>()->GetSharedMesh()->Load();
    //heightMapTes->GetComponent<MeshRenderer>()->GetSharedMaterial()->Albedo = fileSystem.LoadTexture("HeightMap/texture.png");
    //heightMapTes->GetComponent<MeshRenderer>()->GetSharedMaterial()->Metallic->SetColor(glm::vec4(1.0f));
    //heightMapTes->GetComponent<MeshRenderer>()->GetSharedMaterial()->Height = fileSystem.LoadTexture("HeightMap/heightmap.png");
    ////heightMap->GetComponent<MeshRenderer>()->GetSharedMaterial()->SetParallaxStrength(0.1f);
    //heightMapTes->GetComponent<MeshRenderer>()->GetSharedMaterial()->SetShader(ShaderFactory::Type::HeightMapTessellation);
    //heightMapTes->GetComponent<MeshRenderer>()->SetCastsShadow(false);
    //heightMapTes->AddComponent<MeshCollider>(heightMapTes->GetComponent<MeshRenderer>()->GetSharedMesh(), false);
    //heightMapTes->GetComponent<MeshCollider>()->SetIsKinematic(true);
    //heightMapTes->AddComponent<HeightMapSettingsScript>()->UseTessellation = true;
    //heightMapTes->GetComponent<HeightMapSettingsScript>()->Name = "heightMapTes";
    //heightMapTes->AddComponent<ObjectRotationScript>();

    auto heightMap = objects.CreateObject("heightMap");
    //heightMap->GetComponent<Transform>()->TranslateX(-1.0);
    heightMap->AddComponent<MeshRenderer>()->SetMesh(fileSystem.LoadHeightMap("HeightMap/heightmap.png"));
    //heightMap->GetComponent<MeshRenderer>()->GetSharedMesh()->Tessellation = false;
    //heightMap->GetComponent<MeshRenderer>()->GetSharedMesh()->Unload();
    //heightMap->GetComponent<MeshRenderer>()->GetSharedMesh()->Load();
    heightMap->GetComponent<MeshRenderer>()->GetSharedMaterial()->Albedo = fileSystem.LoadTexture("HeightMap/texture.png");
    //heightMap->GetComponent<MeshRenderer>()->GetSharedMaterial()->Albedo->SetColor(glm::vec4(0.5f));
    heightMap->GetComponent<MeshRenderer>()->GetMaterial()->Albedo->SetIsSRGB();
    //heightMap->GetComponent<MeshRenderer>()->GetSharedMaterial()->Emission = fileSystem.LoadTexture("HeightMap/texture.png");
    //heightMap->GetComponent<MeshRenderer>()->GetMaterial()->Emission->SetIsSRGB();
    heightMap->GetComponent<MeshRenderer>()->GetSharedMaterial()->Metallic->SetColor(glm::vec4(1.0f));
    //heightMap->GetComponent<MeshRenderer>()->GetSharedMaterial()->Height = fileSystem.LoadTexture("HeightMap/heightmap.png");
    //heightMap->GetComponent<MeshRenderer>()->GetSharedMaterial()->SetParallaxStrength(0.1f);
    //heightMap->GetComponent<MeshRenderer>()->GetSharedMaterial()->SetShader(ShaderFactory::Type::HeightMapTessellation);
    heightMap->GetComponent<MeshRenderer>()->SetCastsShadow(false);
    heightMap->AddComponent<MeshCollider>(fileSystem.LoadMesh("plane.obj"), false);
    heightMap->GetComponent<MeshCollider>()->SetIsKinematic(true);
    heightMap->AddComponent<HeightMapSettingsScript>();
    heightMap->GetComponent<HeightMapSettingsScript>()->Name = "heightMap";
    heightMap->AddComponent<ObjectRotationScript>();
}