#pragma once
#include "CameraObject.h"
#include "GameScript.h"
#include "RotateWorldScript.h"

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

	auto brickMaterial = fileSystem.LoadMaterials("brickMaterial.dae")[0];

    auto joker = objects.CreateObject<Object>("joker");
    joker->AddComponent<MeshRenderer>()->Load("cube.dae");
	joker->GetComponent<MeshRenderer>()->SetMaterial(0, brickMaterial);

    //oker->GetComponent<MeshRenderer>()->SetMaterials(fileSystem.LoadMaterials("statue.dae"));
    //joker->GetComponent<Transform>()->SetScale(glm::vec3(0.1f));
    //joker->GetComponent<Transform>()->TranslateX(1.0f);
    //joker->GetComponent<Transform>()->RotateX(-1.57f);
    //joker->AddComponent<GameScript>();

    auto joker2 = objects.CreateObject<Object>("joker2");
    joker2->AddComponent<MeshRenderer>()->Load("sphere.dae");
	joker2->GetComponent<MeshRenderer>()->SetMaterial(0, brickMaterial);
    //joker2->GetComponent<MeshRenderer>()->SetMaterials(fileSystem.LoadMaterials("LuxuryLivingRoomSofa.obj"));
    joker2->GetComponent<Transform>()->TranslateX(3.0f);
    //joker2->GetComponent<Transform>()->SetScale(glm::vec3(0.1f));
    //joker2->GetComponent<Transform>()->SetParent(joker);
    //joker2->AddComponent<GameScript>();
    //joker2->GetComponent<GameScript>()->increaseXZPosition = (2.0f * 3.14f) / 15.0f;

    auto joker3 = objects.CreateObject<Object>("joker3");
    //joker3->GetComponent<Transform>()->SetParent(joker2);
    joker3->AddComponent<MeshRenderer>()->Load("cylinder.dae");
	joker3->GetComponent<MeshRenderer>()->SetMaterial(0, brickMaterial);
    //joker3->GetComponent<MeshRenderer>()->SetMaterials(fileSystem.LoadMaterials("venom joker.dae"));
    joker3->GetComponent<Transform>()->TranslateX(-3.0f, Transform::Space::World);
    //joker3->GetComponent<Transform>()->SetScale(glm::vec3(0.5f));
    joker3->GetComponent<Transform>()->RotateX(3.14f);
    ////joker3->AddComponent<RotateWorldScript>();

    auto camera = objects.CreateObject<CameraObject>("camera");
    camera->GetComponent<Transform>()->TranslateZ(10.0);
    auto light = objects.CreateObject<Object>("light")->AddComponent<Light>();
}
