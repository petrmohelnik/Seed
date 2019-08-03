#include "FileSystem.h"
#include <FreeImage.h> //needs to be here because of clash with imgui
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "Engine.h"
#include <numeric>

FileSystem::FileSystem()
{
    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType::aiPrimitiveType_LINE | aiPrimitiveType::aiPrimitiveType_POINT);
}

std::string FileSystem::LoadTextFile(const std::string& path)
{
    std::ifstream file(path);

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

void FileSystem::LoadScene(const std::string& path)
{
    if (loadedScene == path)
        return;

    auto scene = importer.ReadFile(parentFolder + path
		, aiProcess_GenSmoothNormals
		| aiProcess_SortByPType
		| aiProcess_Triangulate
		| aiProcess_CalcTangentSpace
		| aiProcess_FixInfacingNormals
		| aiProcess_ImproveCacheLocality
		| aiProcess_OptimizeMeshes
		| aiProcess_OptimizeGraph
        | aiProcess_FindInvalidData
        | aiProcess_JoinIdenticalVertices
        | aiProcess_RemoveRedundantMaterials
        | aiProcess_ValidateDataStructure
    );

    if (!scene)
    {
        throw std::runtime_error("Failed to parse " + path + " file : " + importer.GetErrorString());
    }
    loadedScene = path;
}

const aiScene* FileSystem::GetScene(const std::string& path)
{
    LoadScene(path);
    return importer.GetScene();
}

std::vector<Object*> FileSystem::LoadObjects(const std::string& path)
{
    auto const scene = GetScene(path);
    std::vector<Object*> objects;

    auto const materials = LoadMaterialsData(scene->mMaterials, scene->mNumMaterials, path);
    auto const mesh = LoadMeshData(scene->mMeshes, scene->mNumMeshes);
    LoadNode(scene, scene->mRootNode, nullptr, objects, mesh->subMeshes, materials);

    UnloadScene();

    return objects;
}

void FileSystem::LoadNode(const aiScene* scene, aiNode* node, Object* parent, std::vector<Object*>& objects, 
    const std::vector<std::shared_ptr<SubMesh>>& subMeshes, const std::vector<std::shared_ptr<Material>>& materials)
{
    auto object = Engine::GetObjects().CreateObject<Object>(node->mName.C_Str());
    if(parent != nullptr)
        object->GetComponent<Transform>()->SetParent(parent);

    aiVector3D translation, rotation, scale;
    node->mTransformation.Decompose(scale, rotation, translation);
    object->GetComponent<Transform>()->Translate(ToGlmVec3(translation));
    object->GetComponent<Transform>()->Rotate(ToGlmVec3(rotation));
    object->GetComponent<Transform>()->SetScale(ToGlmVec3(scale));

    LoadMesh(scene, node, object, subMeshes, materials);
    LoadLight(scene, node, object);
    LoadCamera(scene, object);

    objects.push_back(object);
    for(unsigned int childIndex = 0; childIndex < node->mNumChildren; childIndex++)
    {
        LoadNode(scene, node->mChildren[childIndex], object, objects, subMeshes, materials);
    }
}

void FileSystem::LoadMesh(const aiScene* scene, aiNode* node, Object* object, const std::vector<std::shared_ptr<SubMesh>>& subMeshes, const std::vector<std::shared_ptr<Material>>& materials)
{
    if (node->mNumMeshes != 0)
    {
        object->AddComponent<MeshRenderer>();

        auto mesh = std::make_shared<Mesh>();
        std::vector<std::shared_ptr<Material>> orderedMaterials;
        mesh->subMeshes.reserve(node->mNumMeshes);
        orderedMaterials.reserve(node->mNumMeshes);

        for (unsigned int index = 0; index < node->mNumMeshes; index++)
        {
            mesh->subMeshes.push_back(subMeshes[node->mMeshes[index]]);
            orderedMaterials.push_back(materials[scene->mMeshes[node->mMeshes[index]]->mMaterialIndex]);
        }

        object->GetComponent<MeshRenderer>()->SetMesh(mesh);
        object->GetComponent<MeshRenderer>()->SetMaterials(orderedMaterials);
    }
}

void FileSystem::LoadLight(const aiScene* scene, aiNode* node, Object* object)
{
    for(unsigned int index = 0; index < scene->mNumLights; index++)
    {
        if(aiString(object->GetName()) == scene->mLights[index]->mName)
        {
            auto assimpLight = scene->mLights[index];
            auto light = object->AddComponent<Light>();

            //if I want to support delta transformations I have to create another transform in light that will be child to object
            //if (assimpLight->mType != aiLightSourceType::aiLightSource_POINT)
            //{
            //    if(ToGlmVec3(assimpLight->mUp) != glm::vec3(0.0f))
            //        light->GetTransform()->LookAtLocal(ToGlmVec3(assimpLight->mDirection), ToGlmVec3(assimpLight->mUp));
            //    else
            //        light->GetTransform()->LookAtLocal(ToGlmVec3(assimpLight->mDirection));
            //}
            //auto fwd = light->GetTransform()->GetForwardAxis();

            light->GetTransform()->Translate(ToGlmVec3(assimpLight->mPosition));
            //float quadraticRange = std::sqrt(75.0f / assimpLight->mAttenuationQuadratic);
            //float linearRange = 4.5f / assimpLight->mAttenuationQuadratic;
            //light->dataBlock.Range = (quadraticRange + linearRange) / 2.0f;
            if (assimpLight->mType == aiLightSourceType::aiLightSource_SPOT)
            {
                light->SetType(Light::Type::Spot);
                light->SetSpotAngle(assimpLight->mAngleOuterCone * 2.0f, 1.0f - (assimpLight->mAngleInnerCone / assimpLight->mAngleOuterCone));
            }
            else if(assimpLight->mType == aiLightSourceType::aiLightSource_POINT)
                light->SetType(Light::Type::Point);
            else
                light->SetType(Light::Type::Directional);
            
            if(node->mMetaData)
                node->mMetaData->Get("PBR_LightRange", light->dataBlock.Range);

            //adjust color so max value is 1.0, rest is intensity
            auto color = glm::vec3(assimpLight->mColorDiffuse.r, assimpLight->mColorDiffuse.g, assimpLight->mColorDiffuse.b);
            auto illuminationPower = glm::max(glm::max(color.x, color.y), color.z);
            light->dataBlock.Color = color / illuminationPower;
            light->dataBlock.Itensity = illuminationPower / static_cast<float>(4.0f * M_PI); //convert from power to intensity
        }
    }
}

void FileSystem::LoadCamera(const aiScene* scene, Object* object)
{
    for (unsigned int index = 0; index < scene->mNumCameras; index++)
    {
        if (aiString(object->GetName()) == scene->mCameras[index]->mName)
        {
            auto assimpCamera = scene->mCameras[index];
            auto camera = object->AddComponent<Camera>();
            if (index == 0)
                RenderingPipeline::SetMainCamera(camera);

            //if I want to support delta transformations I have to create another transform in camera that will be child to object
            //camera->GetTransform()->LookAtLocal(ToGlmVec3(assimpCamera->mLookAt), ToGlmVec3(assimpCamera->mUp));
            camera->GetTransform()->Translate(ToGlmVec3(assimpCamera->mPosition));
            camera->SetProjectionMatrix(assimpCamera->mHorizontalFOV, assimpCamera->mAspect, assimpCamera->mClipPlaneNear, assimpCamera->mClipPlaneFar);
        }
    }
}

void FileSystem::UnloadScene()
{
    importer.FreeScene();
}

std::shared_ptr<Mesh> FileSystem::LoadMesh(const std::string& path)
{
    auto const result = loadedMeshes.find(path);
    if (result != loadedMeshes.end() && !result->second.expired())
        return result->second.lock();

    auto const scene = GetScene(path);

    if(scene->mNumMeshes == 0)
        throw std::runtime_error("File: " + path + " does not contain any meshes");

    auto mesh = LoadMeshData(scene->mMeshes, scene->mNumMeshes);
    loadedMeshes.insert_or_assign(path, mesh);

    return mesh;
}

std::shared_ptr<Mesh> FileSystem::LoadMeshData(aiMesh** assimpMeshes, unsigned int numMeshes)
{
    auto mesh = std::make_shared<Mesh>();
    mesh->subMeshes.reserve(numMeshes);

    for (unsigned int index = 0; index < numMeshes; index++)
    {
        mesh->subMeshes.push_back(LoadSubMeshData(assimpMeshes[index]));
    }

    return mesh;
}

std::shared_ptr<SubMesh> FileSystem::LoadSubMeshData(aiMesh* assimpMesh)
{
    auto subMesh = std::make_shared<SubMesh>();
    subMesh->vertices.reserve(assimpMesh->mNumVertices);
    subMesh->normals.reserve(assimpMesh->mNumVertices);
    subMesh->tangents.reserve(assimpMesh->mNumVertices);
    subMesh->texCoords.reserve(assimpMesh->mNumVertices);
    subMesh->indices.reserve(assimpMesh->mNumFaces);

    for (unsigned int index = 0; index < assimpMesh->mNumVertices; index++)
    {
        subMesh->vertices.emplace_back(glm::vec3(
            assimpMesh->mVertices[index].x,
            assimpMesh->mVertices[index].y,
            assimpMesh->mVertices[index].z));

        subMesh->normals.emplace_back(glm::normalize(glm::vec3(
            assimpMesh->mNormals[index].x,
            assimpMesh->mNormals[index].y,
            assimpMesh->mNormals[index].z)));

        if (assimpMesh->mTangents)
        {
            subMesh->tangents.emplace_back(glm::vec3(
                assimpMesh->mTangents[index].x,
                assimpMesh->mTangents[index].y,
                assimpMesh->mTangents[index].z));

            subMesh->bitangents.emplace_back(glm::vec3(
                assimpMesh->mBitangents[index].x,
                assimpMesh->mBitangents[index].y,
                assimpMesh->mBitangents[index].z));
        }
        else
        {
            glm::vec3 tangent = glm::cross(subMesh->normals.back(), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::vec3 bitangent = glm::cross(tangent, subMesh->normals.back());
            subMesh->tangents.emplace_back(tangent);
            subMesh->bitangents.emplace_back(bitangent);
        }

        if (assimpMesh->mTextureCoords[0])
        {
            subMesh->texCoords.emplace_back(glm::vec2(
                assimpMesh->mTextureCoords[0][index].x,
                assimpMesh->mTextureCoords[0][index].y));
        }
        else
        {
            subMesh->texCoords.emplace_back(glm::vec2(0.0f));
        }
    }

    for (unsigned int index = 0; index < assimpMesh->mNumFaces; index++)
    {
        subMesh->indices.emplace_back(glm::uvec3(
            assimpMesh->mFaces[index].mIndices[0],
            assimpMesh->mFaces[index].mIndices[1],
            assimpMesh->mFaces[index].mIndices[2]));
    }

    return subMesh;
}

std::vector<std::shared_ptr<Material>> FileSystem::LoadMaterials(const std::string& path)
{
    auto result = loadedMaterials.find(path);
    if (result != loadedMaterials.end())
    {
        auto expired = std::any_of(std::begin(result->second), std::end(result->second), [](const auto& weakResult)
        {
            return weakResult.expired();
        });
        if (!expired)
        {
            std::vector<std::shared_ptr<Material>> results;
            for (const auto& weakResult : result->second)
            {
                results.push_back(weakResult.lock());
            }
            return results;
        }
    }

    auto scene = GetScene(path);

    if (scene->mNumMaterials == 0)
        throw std::runtime_error("File: " + path + " does not contain any materials");

    auto materials = LoadMaterialsData(scene->mMaterials, scene->mNumMaterials, path);

    std::vector<std::shared_ptr<Material>> orderedMaterials;
    orderedMaterials.reserve(scene->mNumMeshes);
    for (unsigned int index = 0; index < scene->mNumMeshes; index++)
    {
        orderedMaterials.push_back(materials[scene->mMeshes[index]->mMaterialIndex]);
    }

    loadedMaterials.insert_or_assign(path, std::vector<std::weak_ptr<Material>>());
    for (const auto& material : orderedMaterials)
        loadedMaterials[path].push_back(material);

    return orderedMaterials;
}

std::unique_ptr<TextureCubeMap> FileSystem::LoadCubeMap(const std::string& path, const std::string& format)
{
	std::string prefixPath = (*(path.end() - 1) == '/' || *(path.end() - 1) == '\\' ? path : path + '_');

	auto cubeMap = std::make_unique<TextureCubeMap>();

    cubeMap->faces[0] = LoadTexture(prefixPath + "right." + format, 24, 24, true); cubeMap->faces[0]->SetIsSRGB();
	cubeMap->faces[1] = LoadTexture(prefixPath + "left." + format, 24, 24, true); cubeMap->faces[1]->SetIsSRGB();
	cubeMap->faces[2] = LoadTexture(prefixPath + "top." + format, 24, 24, true); cubeMap->faces[2]->SetIsSRGB();
	cubeMap->faces[3] = LoadTexture(prefixPath + "bottom." + format, 24, 24, true); cubeMap->faces[3]->SetIsSRGB();
	cubeMap->faces[4] = LoadTexture(prefixPath + "front." + format, 24, 24, true); cubeMap->faces[4]->SetIsSRGB();
	cubeMap->faces[5] = LoadTexture(prefixPath + "back." + format, 24, 24, true); cubeMap->faces[5]->SetIsSRGB();

	return cubeMap;
}

std::unique_ptr<TextureCubeMap> FileSystem::LoadCubeMapHDR(const std::string& path)
{
    auto cubeMapHDR = std::make_unique<TextureCubeMap>();

    stbi_set_flip_vertically_on_load(true);
    int width, height, dummy;
    float *data = stbi_loadf((parentFolder + path).c_str(), &width, &height, &dummy, 0);
    if (!data)
        throw std::runtime_error("Failed to load texture (" + path + ")");

    cubeMapHDR->LoadFromEquirectangular(data, width, height);

    stbi_image_free(data);
    
    return cubeMapHDR;
}

std::vector<std::shared_ptr<Material>> FileSystem::LoadMaterialsData(aiMaterial** assimpMaterials, unsigned int numMaterials, const std::string& path)
{
    auto folderEnd = path.find_last_of("\\/");
    std::string folder;
    if (folderEnd != std::string::npos)
        folder = path.substr(0, path.find_last_of("\\/") + 1);

    auto materials = std::vector<std::shared_ptr<Material>>();

    for (unsigned int index = 0; index < numMaterials; index++)
    {
        materials.push_back(std::make_shared<Material>(LoadMaterialData(assimpMaterials[index], folder)));
    }

    return materials;
}

Material FileSystem::LoadMaterialData(aiMaterial* assimpMaterial, const std::string& folder)
{
    Material material;

    if (!LoadMaterialTexture(assimpMaterial, aiTextureType_DIFFUSE, material.Albedo, folder, 24, 32))
    {
        LoadMaterialColor(assimpMaterial, AI_MATKEY_COLOR_DIFFUSE, material.Albedo, aiColor4D(1.0f));
        LoadMaterialAlpha(assimpMaterial, AI_MATKEY_OPACITY, material.Albedo, 1.0f);
    }

    if(!LoadMaterialTexture(assimpMaterial, aiTextureType_NORMALS, material.Normal, folder, 24, 24))
		material.Normal->SetColor(glm::vec3(0.5f, 0.5f, 1.0f));
    
    if (LoadMaterialTexture(assimpMaterial, aiTextureType_SPECULAR, material.Metallic, folder, 24, 32))
    {
        material.SetSpecularWorkflow();
        if (material.Metallic->bytesPerPixel == 3)
            material.Metallic->AddChannel(GetMaterialFloat(assimpMaterial, AI_MATKEY_SHININESS, 0.0f));
    }
    else if (!LoadMaterialTexture(assimpMaterial, aiTextureType_UNKNOWN, material.Metallic, folder, 24, 24))
    {
        unsigned int isSpecular;
        assimpMaterial->Get(AI_MATKEY_GLTF_PBRSPECULARGLOSSINESS, isSpecular);
        if (isSpecular == 1)
        {
            LoadMaterialColor(assimpMaterial, AI_MATKEY_COLOR_SPECULAR, material.Metallic, aiColor4D(0.0f));
            LoadMaterialAlpha(assimpMaterial, AI_MATKEY_SHININESS, material.Metallic, 0.0f);
            material.SetSpecularWorkflow();
        }
        else
        {
            material.Metallic->SetColor(1.0f); //ambient occlusion
            material.Metallic->AddChannel(GetMaterialFloat(assimpMaterial, AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR, 1.0f));
            material.Metallic->AddChannel(GetMaterialFloat(assimpMaterial, AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR, 1.0f));
        }
    }

    if(!LoadMaterialTexture(assimpMaterial, aiTextureType_EMISSIVE, material.Emission, folder, 24, 24))
        LoadMaterialColor(assimpMaterial, AI_MATKEY_COLOR_EMISSIVE, material.Emission, aiColor4D(0.0f));

    if(!LoadMaterialTexture(assimpMaterial, aiTextureType_HEIGHT, material.Height, folder, 8, 8))
        material.Height->SetColor(1.0f);
	else
		material.dataBlock.ParallaxStrength = GetMaterialFloat(assimpMaterial, AI_MATKEY_BUMPSCALING, 0.05f);

    if (LoadMaterialTexture(assimpMaterial, aiTextureType_LIGHTMAP, material.Occlusion, folder, 8, 8))
        material.UseOcclusionMap();
    else
        material.Occlusion->SetColor(1.0f);

    return material;
}

bool FileSystem::LoadMaterialTexture(aiMaterial* assimpMaterial, aiTextureType textureType, std::shared_ptr<Texture>& textureData, const std::string& folder, int bitsMinimum, int bitsMaximum)
{
    if (assimpMaterial->GetTextureCount(textureType) != 0)
    {
        aiString texturePath;
        if (assimpMaterial->GetTexture(textureType, 0, &texturePath, nullptr, nullptr, nullptr, nullptr, nullptr) != AI_SUCCESS)
            throw std::runtime_error("Failed to load texture from material");

        textureData = LoadTexture(folder + texturePath.C_Str(), bitsMinimum, bitsMaximum);

        return true;
    }

    return false;
}

void FileSystem::LoadMaterialColor(aiMaterial* assimpMaterial, const char* pKey, unsigned int type, unsigned int index, std::shared_ptr<Texture>& textureData, aiColor4D defaultColor)
{
    aiColor4D color;
	if (aiGetMaterialColor(assimpMaterial, pKey, type, index, &color) != AI_SUCCESS)
		color = defaultColor;
     
    textureData->SetColor(glm::vec4(color.r, color.g, color.b, color.a));
}

void FileSystem::LoadMaterialAlpha(aiMaterial* assimpMaterial, const char* pKey, unsigned int type, unsigned int index, std::shared_ptr<Texture>& textureData, float defaultAlpha)
{
    textureData->SetAlphaColor(GetMaterialFloat(assimpMaterial, pKey, type, index, defaultAlpha));
}

float FileSystem::GetMaterialFloat(aiMaterial * assimpMaterial, const char * pKey, unsigned int type, unsigned int index, float defaultAlpha)
{
    float alpha;
    if (aiGetMaterialFloat(assimpMaterial, pKey, type, index, &alpha) == AI_SUCCESS)
    {
        if (std::strcmp(pKey, "$mat.shininess") == 0)
            alpha = log2(alpha) / 13;
    }
    else
        alpha = defaultAlpha;

    return alpha;
}

std::shared_ptr<Texture> FileSystem::LoadTexture(const std::string& path, int bitsMinimum, int bitsMaximum, bool flipHorizontal)
{
    auto const result = loadedTextures.find(path);
    if (result != loadedTextures.end() && !result->second.expired())
        return result->second.lock();

    auto const absolutePath = parentFolder + path;
    auto texture = std::make_shared<Texture>();

    auto textureFormat = FreeImage_GetFileType(absolutePath.c_str(), 0);
    if (textureFormat == FIF_UNKNOWN)
        textureFormat = FreeImage_GetFIFFromFilename(absolutePath.c_str());
    if (!textureFormat)
        throw std::runtime_error("Failed to load texture (" + absolutePath + "): unknown format");
    if (!FreeImage_FIFSupportsReading(textureFormat))
        throw std::runtime_error("Failed to load texture (" + absolutePath + "): texture format is not supported");

    auto loadedTexture = FreeImage_Load(textureFormat, absolutePath.c_str());

    if (bitsMaximum == 8 && FreeImage_GetBPP(loadedTexture) > 8)
        loadedTexture = FreeImage_ConvertTo8Bits(loadedTexture);
    if (bitsMaximum == 24 && FreeImage_GetBPP(loadedTexture) > 24)
        loadedTexture = FreeImage_ConvertTo24Bits(loadedTexture);
    if (bitsMinimum == 24 && FreeImage_GetBPP(loadedTexture) < 24)
        loadedTexture = FreeImage_ConvertTo24Bits(loadedTexture);
    if (flipHorizontal && !FreeImage_FlipVertical(loadedTexture))
        throw std::runtime_error("Could not flip texture");

    if (!loadedTexture)
        throw std::runtime_error("Failed to load texture (" + absolutePath + ")");

    auto textureData = FreeImage_GetBits(loadedTexture);
    texture->width = FreeImage_GetWidth(loadedTexture);
    texture->height = FreeImage_GetHeight(loadedTexture);
    texture->bytesPerPixel = FreeImage_GetBPP(loadedTexture) / 8;
    texture->data.assign(textureData, textureData + texture->bytesPerPixel * texture->width * texture->height);

    FreeImage_Unload(loadedTexture);

    loadedTextures.insert_or_assign(path, texture);

    return texture;
}

glm::vec3 FileSystem::ToGlmVec3(const aiVector3D& aiVector)
{
    return glm::vec3(aiVector.x, aiVector.y, aiVector.z);
}
