#include "StdAfx.h"
#include "AssimpLoader.h"

#include "Graphics/Mesh/Mesh.h"

AssimpLoader::Meshes AssimpLoader::Load(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace |
		aiProcess_GenNormals |
		aiProcess_GenUVCoords);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		_Log("", importer.GetErrorString());
		return AssimpLoader::Meshes();
	}

	_Log("The vertices file was successfully opened");

	ProcessNode(scene->mRootNode, scene);

	_Log("The vertices file was successfully loaded");

	auto t = m_meshes;
	m_meshes.clear();

	return t;
}

void AssimpLoader::ProcessNode(aiNode* node, const aiScene* scene)
{
	

	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(*ProcessMesh(mesh, scene));
	}

	for (uint32_t i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Graphics::Mesh* AssimpLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	Graphics::Mesh* meshPol = new Graphics::Mesh();

	std::vector<Graphics::Vertex> vertices;
	std::vector<uint64_t> indices;

	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		Graphics::Vertex vertex;
		glm::vec3 vector; 
		
		if (mesh->mVertices != nullptr)
		{
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;
		}

		if (mesh->mNormals != nullptr)
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}

		if (mesh->mTangents != nullptr)
		{
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.tangent = vector;
		}

		if (mesh->mBitangents != nullptr)
		{
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.bitangent = vector;
		}

		vertices.push_back(vertex);
	}
	
	for (uint64_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint64_t j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	meshPol->SetIndices(indices);

	/*aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	meshPol->SetMaterialName(material->GetName().C_Str());

	std::vector<Texture2D> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	std::vector<Texture2D> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	std::vector<Texture2D> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	std::vector<Texture2D> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT);
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());*/

	meshPol->SetPolygonVertices(vertices);

	return meshPol;
}