#pragma once
#include "CameraObject.h"

void DefaultScene(Objects& objects, FileSystem& fileSystem)
{
    /*auto cameraObject = objects.CreateObject<CameraObject>("camera");
    auto cameraObject1 = objects.CreateObject<CameraObject>("camera1");
    auto cameraObject2 = objects.CreateObject<CameraObject>("camera2");
    auto cameraObject3 = objects.CreateObject<CameraObject>("camera3");
    auto cameraObject4 = objects.CreateObject<CameraObject>("camera4");
    cameraObject1->GetComponent<Transform>()->SetParent(cameraObject);
    cameraObject2->GetComponent<Transform>()->SetParent(cameraObject->GetComponent<Transform>());
    cameraObject3->GetComponent<Transform>()->SetParent(cameraObject2->GetComponent<Transform>());
    cameraObject4->GetComponent<Transform>()->SetParent(cameraObject2->GetComponent<Transform>());
    cameraObject->AddTag("tag");
    cameraObject->AddComponent<Light>()->Destroy(16000);
    cameraObject4->AddComponent<Camera>();
    cameraObject->AddComponent<MeshRenderer>();
    cameraObject4->AddComponent<CameraScript>();
    cameraObject2->AddComponent<Audio>();
    cameraObject3->AddComponent<Rigidbody>();
    cameraObject->AddComponent<Collider>();
    cameraObject->AddComponent<Audio>();
    cameraObject->GetComponent<MeshRenderer>();
    cameraObject->GetComponent<Light>();
    cameraObject4->GetComponent<Camera>()->Destroy();
    cameraObject->GetComponent<Transform>();
    cameraObject->GetComponent<Audio>();
    cameraObject->GetComponent<Collider>();
    cameraObject3->GetComponent<Rigidbody>()->Destroy(15000);
    cameraObject->Destroy(5000);
    objects.GetObjectsByName("camera");
    objects.GetObjects<CameraObject>();
    objects.GetObjectsByTag("tag");
    
    cameraObject->GetComponent<MeshRenderer>()->SetMesh(fileSystem.LoadMesh("venom joker.dae"));
    cameraObject->GetComponent<MeshRenderer>()->SetMaterials(fileSystem.LoadMaterials("venom joker.dae"));
    fileSystem.LoadCameras();
    auto cameras = objects.GetObjectsByName("camera1");
    cameraObject->GetComponent<MeshRenderer>()->GetSharedMesh()->Load();
    cameraObject->GetComponent<MeshRenderer>()->GetSharedMesh()->Unload();
    cameraObject->GetComponent<MeshRenderer>()->GetSharedMaterial()->Load();
    cameraObject->GetComponent<MeshRenderer>()->GetSharedMaterial()->SetShader(ShaderFactory::Type::Basic);
    cameraObject->GetComponent<MeshRenderer>()->GetSharedMaterial()->Unload();

    ShaderFactory shaders;
    shaders.GetShader(ShaderFactory::Type::Basic);*/

    auto joker = objects.CreateObject<Object>("joker");
    joker->AddComponent<MeshRenderer>()->SetMesh(fileSystem.LoadMesh("venom joker.dae"));
    joker->GetComponent<MeshRenderer>()->SetMaterials(fileSystem.LoadMaterials("venom joker.dae"));
    joker->GetComponent<Transform>()->SetScale(glm::vec3(0.01));
    joker->GetComponent<Transform>()->RotateZ(1.5);
    joker->GetComponent<Transform>()->TranslateX(2.0);

    auto joker2 = objects.CreateObject<Object>("joker");
    joker2->AddComponent<MeshRenderer>()->SetMesh(fileSystem.LoadMesh("venom joker.dae"));
    joker2->GetComponent<MeshRenderer>()->SetMaterials(fileSystem.LoadMaterials("venom joker.dae"));
    joker2->GetComponent<Transform>()->TranslateX(-400.0);
    joker2->GetComponent<Transform>()->SetParent(joker);

    auto camera = objects.CreateObject<Object>("camera")->AddComponent<Camera>();
    auto light = objects.CreateObject<Object>("light")->AddComponent<Light>();
}
