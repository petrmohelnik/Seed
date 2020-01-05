#pragma once
#include "Object.h"
#include "PlayerObject.h"
#include "GameScript.h"
#include "RotateWorldScript.h"
#include "SkyboxSwitcherScript.h"

void DefaultScene(Objects& objects, FileSystem& fileSystem)
{
    auto player = objects.CreateObject<PlayerObject>("player");

	auto light = objects.CreateObject("light");
	light->AddComponent<Light>();
	light->GetComponent<Light>()->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
    light->GetComponent<Light>()->SetSpotLight(glm::vec3(0.0f, -1.0f, -0.4f), 0.0f, 100.0f, 2.4f, 0.2f);
    light->GetComponent<Light>()->SetShadowNearPlaneDistance(0.2f);
    light->GetComponent<Light>()->SetShadowFarPlaneDistance(30.0f);
	light->GetComponent<Transform>()->Translate(glm::vec3(-2.0, 2.0, 5.0), Transform::Space::World);
	light->GetComponent<Transform>()->SetScale(0.2f);
    //light->GetComponent<Transform>()->LookAt(glm::vec3(0.0f));
	light->AddComponent<MeshRenderer>()->Load("sphere.obj");
    light->GetComponent<MeshRenderer>()->GetSharedMaterial()->Emission->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
	light->AddComponent<GameScript>();

    auto ground = objects.CreateObjectWithMesh("ground", "plane.obj");
    ground->GetComponent<Transform>()->SetScale(glm::vec3(1000));
    ground->GetComponent<Transform>()->TranslateY(-0.1f);
    ground->AddComponent<MeshCollider>()->SetMesh(ground->GetComponent<MeshRenderer>()->GetSharedMesh());
    ground->AddComponent<Rigidbody>();

	objects.SetSkybox(fileSystem.LoadCubeMapHDR("Newport_Loft.hdr"));
	auto skyboxSwitcher = objects.CreateObject("skyboxSwitcher")->AddComponent<SkyboxSwitcherScript>();
	skyboxSwitcher->AddSkybox("Newport_Loft.hdr");
	skyboxSwitcher->AddSkybox("sunrise.hdr");
	skyboxSwitcher->AddSkybox("Space.hdr");
	skyboxSwitcher->AddSkybox("Bunker.hdr");
	skyboxSwitcher->AddSkybox("Protospace.hdr");
	skyboxSwitcher->AddSkybox("Milkyway.hdr");
	//objects.SetSkybox(fileSystem.LoadCubeMapHDR("sunrise.hdr"));
    //objects.SetSkybox(fileSystem.LoadCubeMapHDR("Space.hdr"));
	//objects.SetSkybox(fileSystem.LoadCubeMapHDR("Bunker.hdr"));
	//objects.SetSkybox(fileSystem.LoadCubeMapHDR("Protospace.hdr"));
	//objects.SetSkybox(fileSystem.LoadCubeMap("skybox/", "jpg"));
	//objects.SetSkybox(std::make_unique<TextureCubeMap>(glm::vec4(0.4f, 0.4f, 0.7f, 1.0f)));

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
    
    objects.CreateObjectsFromScene("scene/scene.gltf");

    auto spheres = objects.CreateObjectWithMesh("spheres", "MetalRoughSpheres/scene.gltf", glm::vec3(5.0f, 0.0f, 10.0f));
    spheres->GetComponent<MeshRenderer>()->GetSharedMaterial()->UseOcclusionMap();

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
    cobblestone->GetComponent<Transform>()->SetScale(glm::vec3(0.5f));
	//cobblestone->AddComponent<RotateWorldScript>();
    cobblestone->AddComponent<BoxCollider>()->SetBox(glm::vec3(0.0f), glm::vec3(1.0f));
    cobblestone->AddComponent<Rigidbody>();

	//auto redbricksMaterial = std::make_shared<Material>();
	//redbricksMaterial->Albedo = fileSystem.LoadTexture("redbricks/albedo.png", 24, 24);
	//redbricksMaterial->Normal = fileSystem.LoadTexture("redbricks/normal.png", 24, 24);
	//redbricksMaterial->Height = fileSystem.LoadTexture("redbricks/height.png", 8, 8);
	//redbricksMaterial->Metallic = fileSystem.LoadTexture("redbricks/ao.png", 8, 8);
	//redbricksMaterial->Metallic->AddChannelFromTexture(fileSystem.LoadTexture("redbricks/roughness.png", 8, 8), 0);
	//redbricksMaterial->Metallic->AddChannel(0.0);
 //   redbricksMaterial->Metallic->SetIsRGBOrder();
	//redbricksMaterial->SetParallaxStrength(0.1f);

	//auto redbricks = objects.CreateObject("redbricks");
	//redbricks->AddComponent<MeshRenderer>()->Load("cube.dae");
	//redbricks->GetComponent<MeshRenderer>()->SetMaterial(0, redbricksMaterial);
	//redbricks->GetComponent<Transform>()->TranslateX(-1.5f);
 //   redbricks->GetComponent<Transform>()->SetScale(glm::vec3(0.5f));
	////redbricks->AddComponent<RotateWorldScript>();
 //   redbricks->AddComponent<BoxCollider>()->SetBox(glm::vec3(0.0f), glm::vec3(1.0f));
 //   redbricks->AddComponent<Rigidbody>();

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
    roughblock->GetComponent<Transform>()->TranslateX(1.5f);
    roughblock->GetComponent<Transform>()->SetScale(glm::vec3(0.5f));
    //roughblock->AddComponent<RotateWorldScript>();
    roughblock->AddComponent<BoxCollider>()->SetBox(glm::vec3(0.0f), glm::vec3(1.0f));
    roughblock->AddComponent<Rigidbody>();

    auto targets = objects.GetObjectsByName("TargetDummy");
    auto targetMesh = fileSystem.LoadMesh("TargetDummy/TargetDummy.gltf");
    for (auto& target : targets)
    {
        target->AddComponent<MeshCollider>()->SetMesh(targetMesh);
        target->AddComponent<Rigidbody>();
    }

    //auto camerasphere = objects.CreateObject("camerasphere");
    //camerasphere->AddComponent<MeshRenderer>()->Load("sphere.obj");
    //camerasphere->GetComponent<Transform>()->SetScale(0.2f);
    //camerasphere->GetComponent<Transform>()->TranslateZ(-1.0f);
    //camerasphere->GetComponent<Transform>()->SetParent(camera);
    RenderingPipeline::SetMainCamera(objects.GetObjectByName("PlayerCamera")->GetComponent<Camera>());
    //RenderingPipeline::MainCamera()->GetObject()->AddComponent<CameraMovementScript>();
}
