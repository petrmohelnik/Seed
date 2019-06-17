#pragma once
#include "CameraObject.h"
#include "GameScript.h"
#include "RotateWorldScript.h"

void DefaultScene(Objects& objects, FileSystem& fileSystem)
{
	auto brickMaterial1 = fileSystem.LoadMaterials("brickMaterial.dae")[0];
	auto brickMaterial2 = fileSystem.LoadMaterials("brickMaterial2.dae")[0];
	auto brickMaterial3 = fileSystem.LoadMaterials("brickMaterial3.dae")[0];
    brickMaterial1->Specular->SetColor(glm::vec4(0.15f, 0.15f, 0.15f, 0.2f));
    brickMaterial2->Specular->SetColor(glm::vec4(0.15f, 0.15f, 0.15f, 0.2f));
    brickMaterial3->Diffuse->SetColor(glm::vec4(0.95, 0.72f, 0.61f, 1.0f));
    brickMaterial3->Specular->SetColor(glm::vec4(1.0f, 0.3f, 0.95f, 1.0f));
    brickMaterial3->SetMetallic();

    auto joker = objects.CreateObject("joker");
    joker->AddComponent<MeshRenderer>()->Load("cube.dae");
	joker->GetComponent<MeshRenderer>()->SetMaterial(0, brickMaterial1);

    auto joker2 = objects.CreateObject("joker2");
    joker2->AddComponent<MeshRenderer>()->Load("cube.dae");
	joker2->GetComponent<MeshRenderer>()->SetMaterial(0, brickMaterial2);
    joker2->GetComponent<Transform>()->TranslateX(3.0f);


    auto joker3 = objects.CreateObject("joker3");
    joker3->AddComponent<MeshRenderer>()->Load("cube.dae");
	joker3->GetComponent<MeshRenderer>()->SetMaterial(0, brickMaterial3);
    joker3->GetComponent<Transform>()->TranslateX(-3.0f, Transform::Space::World);
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
	light->GetComponent<Transform>()->TranslateZ(5.0, Transform::Space::World);
	light->GetComponent<Transform>()->TranslateY(4.0, Transform::Space::World);
	light->GetComponent<Transform>()->TranslateX(-2.0, Transform::Space::World);
	light->GetComponent<Transform>()->SetScale(glm::vec3(0.2f));
	light->AddComponent<MeshRenderer>()->Load("sphere.obj");
    light->GetComponent<MeshRenderer>()->GetMaterial()->Emission->SetColor(glm::vec3(1.0f, 1.0f, 0.4f));
	light->GetComponent<Light>()->SetColor(glm::vec3(5.0f, 5.0f, 5.0f));
	light->GetComponent<Light>()->SetAmbientColor(glm::vec3(0.1f));
	light->GetComponent<Light>()->SetRange(100);
	//light->GetComponent<Light>()->SetDirection(glm::vec3(0.0f, -1.0f, 0.0f));
	//light->GetComponent<Light>()->SetCutoffAngle(0.7f);
	light->AddComponent<GameScript>();

    auto ground = objects.CreateObjectWithMesh("ground", "plane.obj");
    ground->GetComponent<Transform>()->SetScale(glm::vec3(10));
    ground->GetComponent<Transform>()->TranslateY(-1);

	objects.SetSkybox(fileSystem.LoadCubeMapHDR("Newport_Loft.hdr"));
	//objects.SetSkybox(fileSystem.LoadCubeMapHDR("Bunker.hdr"));
	//objects.SetSkybox(fileSystem.LoadCubeMapHDR("Protospace.hdr"));
	//objects.SetSkybox(fileSystem.LoadCubeMap("skybox/", "jpg"));

    //objects.CreateObjectsFromScene("jaguar.dae");
    
    for (int i = 0; i < 5; i++)
    {
        float xPos = (i % 5) * 2 - 14.0f;
        float yPos = (i / 5) * 2;
        auto sphere = objects.CreateObjectWithMesh("sphere", "sphere.obj", glm::vec3(xPos, yPos, 1.0f));
        sphere->GetComponent<MeshRenderer>()->GetMaterial()->Specular->SetColor(
			glm::vec4(0.2f, 0.2f, 0.2f, i * (1.0f / 4.5f)));
        sphere->GetComponent<MeshRenderer>()->GetMaterial()->Diffuse->SetColor(glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));
    }
	for (int i = 0; i < 15; i++)
	{
		float xPos = (i % 5) * 2 - 14.0f;
		float yPos = (i / 5) * 2 + 2;
		auto sphere = objects.CreateObjectWithMesh("sphere", "sphere.obj", glm::vec3(xPos, yPos, 1.0f));
		sphere->GetComponent<MeshRenderer>()->GetMaterial()->Specular->SetColor(
			glm::vec3(1.0f, ((14 - i) / 5) * 0.4 + 0.1, (i % 5) * (1.0f / 4.5f)));
		sphere->GetComponent<MeshRenderer>()->GetMaterial()->SetMetallic();
		sphere->GetComponent<MeshRenderer>()->GetMaterial()->Diffuse->SetColor(glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));
	}
    
    //auto lamp = objects.CreateObjectWithMesh("lamp", "lamp/scene.gltf", glm::vec3(0.0f, 5.0f, 0.0f));
    //objects.CreateObjectsFromScene("tomons_lamp/lamp.dae");
    //auto spot = objects.GetObjectByName("Spot");
    //spot->AddComponent<MeshRenderer>()->Load("sphere.obj");
    //spot->GetComponent<MeshRenderer>()->GetMaterial()->Emission->SetColor(glm::vec3(1.0f, 1.0f, 0.4f));
    //spot->GetComponent<Light>()->SetRange(20);
    //spot->GetComponent<Light>()->SetCutoffAngle(0.5);

    RenderingPipeline::SetMainCamera(camera->GetComponent<Camera>());
    //RenderingPipeline::MainCamera()->GetObject()->AddComponent<CameraMovementScript>();
}
