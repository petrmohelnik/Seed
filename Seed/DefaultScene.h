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

	auto brickMaterial1 = fileSystem.LoadMaterials("brickMaterial.dae")[0];
	auto brickMaterial2 = fileSystem.LoadMaterials("brickMaterial2.dae")[0];
	auto brickMaterial3 = fileSystem.LoadMaterials("brickMaterial3.dae")[0];

    auto joker = objects.CreateObject("joker");
    joker->AddComponent<MeshRenderer>()->Load("cube.dae");
	joker->GetComponent<MeshRenderer>()->SetMaterial(0, brickMaterial1);

    //oker->GetComponent<MeshRenderer>()->SetMaterials(fileSystem.LoadMaterials("statue.dae"));
    //joker->GetComponent<Transform>()->SetScale(glm::vec3(0.1f));
    //joker->GetComponent<Transform>()->TranslateX(1.0f);
    //joker->GetComponent<Transform>()->RotateX(-1.57f);
    //joker->AddComponent<GameScript>();

    auto joker2 = objects.CreateObject("joker2");
    joker2->AddComponent<MeshRenderer>()->Load("cube.dae");
	joker2->GetComponent<MeshRenderer>()->SetMaterial(0, brickMaterial2);
    //joker2->GetComponent<MeshRenderer>()->SetMaterials(fileSystem.LoadMaterials("LuxuryLivingRoomSofa.obj"));
    joker2->GetComponent<Transform>()->TranslateX(3.0f);
    //joker2->GetComponent<Transform>()->SetScale(glm::vec3(0.1f));
    //joker2->GetComponent<Transform>()->SetParent(joker);
    //joker2->AddComponent<GameScript>();
    //joker2->GetComponent<GameScript>()->increaseXZPosition = (2.0f * 3.14f) / 15.0f;

    auto joker3 = objects.CreateObject("joker3");
    //joker3->GetComponent<Transform>()->SetParent(joker2);
    joker3->AddComponent<MeshRenderer>()->Load("cube.dae");
	joker3->GetComponent<MeshRenderer>()->SetMaterial(0, brickMaterial3);
    //joker3->GetComponent<MeshRenderer>()->SetMaterials(fileSystem.LoadMaterials("venom joker.dae"));
    joker3->GetComponent<Transform>()->TranslateX(-3.0f, Transform::Space::World);
    //joker3->GetComponent<Transform>()->SetScale(glm::vec3(0.5f));
    joker3->GetComponent<Transform>()->RotateX(3.14f);
    joker3->AddComponent<RotateWorldScript>();
	joker2->AddComponent<RotateWorldScript>();
	joker->AddComponent<RotateWorldScript>();

	auto cyborg = objects.CreateObjectWithMesh("cyborg", "cyborg/cyborg.obj");
	cyborg->AddComponent<RotateWorldScript>();

	auto scifi = objects.CreateObjectWithMesh("scifi", "scifi/scene.gltf");
	scifi->GetComponent<Transform>()->SetScale(glm::vec3(0.01f));
	scifi->GetComponent<Transform>()->RotateX(-1.57f);

	//auto treepack = objects.CreateObjectWithMesh("treepack", "treepack/treepack.obj");
	//treepack->AddComponent<RotateWorldScript>();
	//treepack->GetComponent<Transform>()->SetScale(glm::vec3(0.01f));

	auto console = objects.CreateObjectWithMesh("console", "console/console.gltf", glm::vec3(0.0f, 0.0f, 3.0f));
	scifi->GetComponent<Transform>()->RotateY(-1.57f);
	console->AddComponent<RotateWorldScript>();

	//nyra->GetComponent<Transform>()->SetScale(glm::vec3(0.01f));
	//nyra->GetComponent<MeshRenderer>()->GetMaterial()->Normal->SetColor(glm::vec3(0.5f, 0.5f, 1.0f));
	//nyra->GetComponent<MeshRenderer>()->GetMaterial()->Diffuse->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));

	auto oldcar = objects.CreateObjectWithMesh("nyra", "oldcar/scene.gltf", glm::vec3(10.0f, -1.01f, 3.0f));
	oldcar->GetComponent<Transform>()->RotateX(-1.57f);
	oldcar->GetComponent<Transform>()->SetScale(glm::vec3(0.01f));

    auto chair = objects.CreateObjectWithMesh("chair", "chair/scene.gltf", glm::vec3(0.0f, -1.01f, 6.0f));
    chair->GetComponent<Transform>()->RotateX(-1.57f);
    chair->GetComponent<Transform>()->SetScale(glm::vec3(0.05f));

	auto mountain = objects.CreateObjectWithMesh("mountain", "mountain/mountain.obj", glm::vec3(0.0f, -13.0f, -60.0f));
	mountain->GetComponent<Transform>()->SetScale(glm::vec3(0.1f));
	mountain->GetComponent<Transform>()->RotateX(-1.57f);
	mountain->GetComponent<Transform>()->Translate(glm::vec3(0.0f, -12.0f, -60.0f), Transform::Space::World);

	auto nanosuit = objects.CreateObjectWithMesh("nanosuit", "nanosuit/nanosuit.obj");
	nanosuit->AddComponent<RotateWorldScript>();
	nanosuit->GetComponent<Transform>()->TranslateX(3.0f);

    auto camera = objects.CreateObject<CameraObject>("camera");
    camera->GetComponent<Transform>()->TranslateZ(10.0);
    camera->GetComponent<Transform>()->TranslateY(3.0);
    camera->GetComponent<Transform>()->TranslateX(-10.0);
	auto light = objects.CreateObject("light");
	light->AddComponent<Light>();
	light->GetComponent<Transform>()->TranslateZ(50.0, Transform::Space::World);
	light->GetComponent<Transform>()->TranslateY(40.0, Transform::Space::World);
	light->GetComponent<Transform>()->TranslateX(-20.0, Transform::Space::World);
	light->GetComponent<Transform>()->SetScale(glm::vec3(0.2f));
	light->AddComponent<MeshRenderer>()->Load("sphere.obj");
    light->GetComponent<MeshRenderer>()->GetMaterial()->Emission->SetColor(glm::vec3(1.0f, 1.0f, 0.4f));
	light->GetComponent<Light>()->SetColor(glm::vec3(1.0f, 1.0f, 0.95f));
	light->GetComponent<Light>()->SetAmbientColor(glm::vec3(0.03f));
	light->GetComponent<Light>()->SetRange(1000);
	//light->GetComponent<Light>()->SetDirection(glm::vec3(0.0f, -1.0f, 0.0f));
	//light->GetComponent<Light>()->SetCutoffAngle(0.7f);
	//light->AddComponent<GameScript>();

    auto ground = objects.CreateObjectWithMesh("ground", "plane.obj");
    ground->GetComponent<Transform>()->SetScale(glm::vec3(100));
    ground->GetComponent<Transform>()->TranslateY(-1);

	objects.SetSkybox(fileSystem.LoadCubeMap("skybox/", "jpg"));
    
    for (int i = 0; i < 20; i++)
    {
        float xPos = (i % 5) * 2 - 14.0f;
        float yPos = (i / 5) * 2;
        auto sphere = objects.CreateObjectWithMesh("sphere", "sphere.obj", glm::vec3(xPos, yPos, 1.0f));
        sphere->GetComponent<MeshRenderer>()->GetMaterial()->Specular->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, i * (1.0f / 19.0f)));
        sphere->GetComponent<MeshRenderer>()->GetMaterial()->Diffuse->SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }
}
