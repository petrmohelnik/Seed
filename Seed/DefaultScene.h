#pragma once
#include "CameraObject.h"
#include "GameScript.h"
#include "RotateWorldScript.h"

void DefaultScene(Objects& objects, FileSystem& fileSystem)
{
 	//auto scifi = objects.CreateObjectWithMesh("scifi", "chair2/scene.gltf", glm::vec3(0.0f, 3.0f, 0.0f));
	//scifi->GetComponent<Transform>()->SetScale(glm::vec3(1.0f));
	//scifi->GetComponent<Transform>()->RotateY(3.14f);

	////auto treepack = objects.CreateObjectWithMesh("treepack", "treepack/treepack.obj");
	////treepack->AddComponent<RotateWorldScript>();
	////treepack->GetComponent<Transform>()->SetScale(glm::vec3(0.01f));

	//auto console = objects.CreateObjectWithMesh("console", "console/console.gltf", glm::vec3(0.0f, 0.0f, 3.0f));
 //   console->GetComponent<Transform>()->RotateY(-1.57f);
	//console->AddComponent<RotateWorldScript>();

	//auto oldcar = objects.CreateObjectWithMesh("nyra", "oldcar/scene.gltf", glm::vec3(10.0f, -1.01f, 3.0f));
	//oldcar->GetComponent<Transform>()->RotateX(-1.57f);
	//oldcar->GetComponent<Transform>()->SetScale(glm::vec3(0.01f));

 //   auto chair = objects.CreateObjectWithMesh("chair", "holotech_bench/scene.gltf", glm::vec3(0.0f, -1.01f, 6.0f));
 //   //chair->GetComponent<Transform>()->RotateX(-1.57f);
 //   chair->GetComponent<Transform>()->SetScale(glm::vec3(0.005f));

	//auto datsun = objects.CreateObjectWithMesh("datsun", "datsun/scene.gltf", glm::vec3(-6.0f, 0.1f, 6.0f));
 //   datsun->GetComponent<Transform>()->SetScale(glm::vec3(0.1f));
 //   datsun->GetComponent<Transform>()->RotateX(-3.14f);

	//auto nanosuit = objects.CreateObjectWithMesh("nanosuit", "nanosuit/nanosuit.obj");
	//nanosuit->AddComponent<RotateWorldScript>();
	//nanosuit->GetComponent<Transform>()->TranslateX(3.0f);

    auto camera = objects.CreateObject<CameraObject>("camera");
    //camera->GetComponent<Transform>()->TranslateZ(10.0);
    //camera->GetComponent<Transform>()->TranslateY(3.0);
    //camera->GetComponent<Transform>()->TranslateX(-10.0);
	auto light = objects.CreateObject("light");
	light->AddComponent<Light>();
	light->GetComponent<Transform>()->TranslateZ(5.0, Transform::Space::World);
	light->GetComponent<Transform>()->TranslateY(10.0, Transform::Space::World);
	light->GetComponent<Transform>()->TranslateX(-2.0, Transform::Space::World);
	light->GetComponent<Transform>()->SetScale(glm::vec3(0.2f));
	light->AddComponent<MeshRenderer>()->Load("sphere.obj");
    light->GetComponent<MeshRenderer>()->GetMaterial()->Emission->SetColor(glm::vec3(1.0f, 1.0f, 0.4f));
	light->GetComponent<Light>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	light->GetComponent<Light>()->SetRange(1);
	light->GetComponent<Light>()->SetIntensity(1);
	light->GetComponent<Light>()->SetDirection(glm::vec3(0.0f, -1.0f, 0.0f));
	//light->GetComponent<Light>()->SetSpotAngle(0.5f);
	//light->AddComponent<GameScript>();

    //auto scifiCube = objects.CreateObjectWithMesh("scifiCube", "scifi_cube/scene.gltf", glm::vec3(-3.0f, 0.0f, 0.0f));
    //scifiCube->AddComponent<RotateWorldScript>();

    //auto ground = objects.CreateObjectWithMesh("ground", "plane.obj");
    //ground->GetComponent<Transform>()->SetScale(glm::vec3(1000));
    //ground->GetComponent<Transform>()->TranslateY(-1);
    //ground->GetComponent<MeshRenderer>()->GetMaterial()->Normal = brickMaterial1->Normal;
    //ground->GetComponent<MeshRenderer>()->GetMaterial()->Height = brickMaterial1->Height;

	//objects.SetSkybox(fileSystem.LoadCubeMapHDR("Newport_Loft.hdr"));
	objects.SetSkybox(fileSystem.LoadCubeMapHDR("sunrise.hdr"));
	//objects.SetSkybox(fileSystem.LoadCubeMapHDR("Space.hdr"));
	//objects.SetSkybox(fileSystem.LoadCubeMapHDR("Bunker.hdr"));
	//objects.SetSkybox(fileSystem.LoadCubeMapHDR("Protospace.hdr"));
	//objects.SetSkybox(fileSystem.LoadCubeMap("skybox/", "jpg"));
	//objects.SetSkybox(std::make_unique<TextureCubeMap>(glm::vec4(0.4f, 0.4f, 0.7f, 1.0f)));

    //objects.CreateObjectsFromScene("jaguar.dae");
    
 //   for (int i = 0; i < 5; i++)
 //   {
 //       float xPos = (i % 5) * 2 - 14.0f;
 //       float yPos = (i / 5) * 2;
 //       auto sphere = objects.CreateObjectWithMesh("sphere", "sphere.obj", glm::vec3(xPos, yPos, 1.0f));
	//	sphere->GetComponent<MeshRenderer>()->GetMaterial()->SetSpecularWorkflow();
 //       sphere->GetComponent<MeshRenderer>()->GetMaterial()->Metallic->SetColor(
	//		glm::vec4(0.2f, 0.2f, 0.2f, i * (1.0f / 4.5f)));
 //       sphere->GetComponent<MeshRenderer>()->GetMaterial()->Albedo->SetColor(glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));
 //   }
	//for (int i = 0; i < 15; i++)
	//{
	//	float xPos = (i % 5) * 2 - 14.0f;
	//	float yPos = (i / 5) * 2 + 2;
	//	auto sphere = objects.CreateObjectWithMesh("sphere", "sphere.obj", glm::vec3(xPos, yPos, 1.0f));
	//	sphere->GetComponent<MeshRenderer>()->GetMaterial()->SetMetallicWorkflow();
	//	sphere->GetComponent<MeshRenderer>()->GetMaterial()->Metallic->SetColor(
	//		glm::vec3(1.0f, ((14 - i) / 5) * 0.4 + 0.1, (i % 5) * (1.0f / 4.5f)));
	//	sphere->GetComponent<MeshRenderer>()->GetMaterial()->Albedo->SetColor(glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));
	//}
    
    //auto lamp = objects.CreateObjectWithMesh("lamp", "lamp/scene.gltf", glm::vec3(0.0f, 5.0f, 0.0f));
    //objects.CreateObjectsFromScene("tomons_lamp/lamp.dae");
    //auto spot = objects.GetObjectByName("Spot");
    //spot->AddComponent<MeshRenderer>()->Load("sphere.obj");
    //spot->GetComponent<MeshRenderer>()->GetMaterial()->Emission->SetColor(glm::vec3(1.0f, 1.0f, 0.4f));
    //spot->GetComponent<Light>()->SetRange(20);
    //spot->GetComponent<Light>()->SetCutoffAngle(0.5);

    objects.CreateObjectsFromScene("scene/scene.gltf");

    auto spheres = objects.CreateObjectWithMesh("spheres", "MetalRoughSpheres/scene.gltf", glm::vec3(5.0f, 0.0f, 10.0f));
    spheres->GetComponent<MeshRenderer>()->GetMaterial()->UseOcclusionMap();

	auto cobblestoneMaterial = std::make_shared<Material>();
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
	cobblestone->AddComponent<RotateWorldScript>();

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
	redbricks->GetComponent<Transform>()->TranslateX(-3.0f);
	redbricks->AddComponent<RotateWorldScript>();

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
    roughblock->GetComponent<Transform>()->TranslateX(3.0f);
    roughblock->AddComponent<RotateWorldScript>();

    RenderingPipeline::SetMainCamera(camera->GetComponent<Camera>());
    //RenderingPipeline::MainCamera()->GetObject()->AddComponent<CameraMovementScript>();
}
